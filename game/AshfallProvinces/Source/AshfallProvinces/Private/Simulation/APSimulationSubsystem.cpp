#include "Simulation/APSimulationSubsystem.h"
#include "Simulation/APSaveGame.h"
#include "Simulation/APSimulationModel.h"
#include "Kismet/GameplayStatics.h"

namespace
{
AshfallSim::Job ToModelJob(EAPHouseholdJob Job)
{
    switch (Job)
    {
    case EAPHouseholdJob::Food: return AshfallSim::Job::Food;
    case EAPHouseholdJob::Timber: return AshfallSim::Job::Timber;
    case EAPHouseholdJob::Metal: return AshfallSim::Job::Metal;
    default: return AshfallSim::Job::Idle;
    }
}

AshfallSim::SoldierType ToModelSoldierType(EAPSoldierType Type)
{
    switch (Type)
    {
    case EAPSoldierType::Ranged: return AshfallSim::SoldierType::Ranged;
    case EAPSoldierType::Scout: return AshfallSim::SoldierType::Scout;
    default: return AshfallSim::SoldierType::Spear;
    }
}

std::vector<AshfallSim::Household> ToModelHouseholds(const TArray<FAPHouseholdState>& Source)
{
    std::vector<AshfallSim::Household> Result;
    Result.reserve(Source.Num());
    for (const FAPHouseholdState& Entry : Source)
    {
        Result.push_back({Entry.HouseholdId, ToModelJob(Entry.Job), Entry.bAlive, Entry.bMustered});
    }
    return Result;
}

AshfallSim::Army ToModelArmy(const FAPArmyState& Source)
{
    AshfallSim::Army Result;
    Result.Id = Source.CompanyId;
    Result.Type = ToModelSoldierType(Source.SoldierType);
    Result.ProvinceId = Source.ProvinceId;
    Result.DestinationProvinceId = Source.DestinationProvinceId;
    Result.TravelTicksRemaining = Source.TravelTicksRemaining;
    for (int32 Id : Source.HouseholdIds)
    {
        Result.HouseholdIds.push_back(Id);
    }
    return Result;
}
}

void UAPSimulationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    StartNewGame(1);
}

void UAPSimulationSubsystem::Tick(float DeltaTime)
{
    AccumulatedSeconds += DeltaTime;
    while (AccumulatedSeconds >= FixedTickSeconds)
    {
        AccumulatedSeconds -= FixedTickSeconds;
        StepSimulation();
    }
}

TStatId UAPSimulationSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UAPSimulationSubsystem, STATGROUP_Tickables);
}

void UAPSimulationSubsystem::StartNewGame(int32 Seed)
{
    AccumulatedSeconds = 0.0f;
    SimulationTick = 0;
    RandomSeed = static_cast<uint32>(FMath::Max(1, Seed));
    SettlementResources = FAPResources();
    SettlementResources.Food = 20;
    SettlementResources.Timber = 20;
    SettlementResources.Metal = 12;
    SettlementResources.Authority = 20;
    SettlementBuildingSlots = 8;
    Households.Reset(InitialHouseholdCount);
    Provinces.Reset(ProvinceCount);
    Companies.Reset(MaxCompanies);

    for (int32 Id = 0; Id < InitialHouseholdCount; ++Id)
    {
        FAPHouseholdState State;
        State.HouseholdId = Id;
        State.Job = Id < 8 ? EAPHouseholdJob::Food : (Id < 16 ? EAPHouseholdJob::Timber : EAPHouseholdJob::Metal);
        Households.Add(State);
    }

    for (int32 Id = 0; Id < ProvinceCount; ++Id)
    {
        FAPProvinceState State;
        State.ProvinceId = Id;
        State.OwnerId = Id == 0 ? 0 : (Id == ProvinceCount - 1 ? 1 : INDEX_NONE);
        if (Id > 0) State.AdjacentProvinceIds.Add(Id - 1);
        if (Id + 1 < ProvinceCount) State.AdjacentProvinceIds.Add(Id + 1);
        Provinces.Add(State);
    }

    FAPArmyState RivalCompany;
    RivalCompany.CompanyId = 100;
    RivalCompany.OwnerId = 1;
    RivalCompany.SoldierType = EAPSoldierType::Spear;
    RivalCompany.ProvinceId = ProvinceCount - 1;
    RivalCompany.HouseholdIds = {-1, -2, -3, -4};
    Companies.Add(RivalCompany);
}

void UAPSimulationSubsystem::StepSimulation()
{
    const AshfallSim::Resources Produced = AshfallSim::ProductionForTick(ToModelHouseholds(Households));
    SettlementResources.Food += Produced.Food;
    SettlementResources.Timber += Produced.Timber;
    SettlementResources.Metal += Produced.Metal;

    for (FAPArmyState& State : Companies)
    {
        AshfallSim::Army Model = ToModelArmy(State);
        AshfallSim::AdvanceArmy(Model);
        State.ProvinceId = Model.ProvinceId;
        State.DestinationProvinceId = Model.DestinationProvinceId;
        State.TravelTicksRemaining = Model.TravelTicksRemaining;
    }
    ++SimulationTick;
}

bool UAPSimulationSubsystem::MusterCompany(EAPSoldierType Type, const TArray<int32>& HouseholdIds)
{
    const int32 PlayerCompanyCount = Companies.CountByPredicate(
        [](const FAPArmyState& Entry) { return Entry.OwnerId == 0 && !Entry.HouseholdIds.IsEmpty(); });
    if (PlayerCompanyCount >= MaxCompanies)
    {
        return false;
    }
    std::vector<int32> Requested;
    Requested.reserve(HouseholdIds.Num());
    for (int32 Id : HouseholdIds)
    {
        Requested.push_back(Id);
    }
    std::vector<AshfallSim::Household> ModelHouseholds = ToModelHouseholds(Households);
    if (!AshfallSim::CanMuster(ModelHouseholds, Requested))
    {
        return false;
    }

    const int32 AuthorityCost = HouseholdIds.Num() * 2;
    const int32 MetalCost = HouseholdIds.Num();
    if (SettlementResources.Authority < AuthorityCost || SettlementResources.Metal < MetalCost)
    {
        return false;
    }
    SettlementResources.Authority -= AuthorityCost;
    SettlementResources.Metal -= MetalCost;

    for (FAPHouseholdState& Household : Households)
    {
        if (HouseholdIds.Contains(Household.HouseholdId))
        {
            Household.bMustered = true;
        }
    }

    FAPArmyState Company;
    int32 NextCompanyId = 0;
    while (Companies.ContainsByPredicate(
        [NextCompanyId](const FAPArmyState& Entry) { return Entry.CompanyId == NextCompanyId; }))
    {
        ++NextCompanyId;
    }
    Company.CompanyId = NextCompanyId;
    Company.OwnerId = 0;
    Company.SoldierType = Type;
    Company.ProvinceId = 0;
    Company.HouseholdIds = HouseholdIds;
    Companies.Add(Company);
    return true;
}

bool UAPSimulationSubsystem::OrderMove(int32 CompanyId, int32 DestinationProvinceId)
{
    FAPArmyState* Company = FindCompany(CompanyId);
    const FAPProvinceState* Current = Company ? FindProvince(Company->ProvinceId) : nullptr;
    if (!Company || !Current || Company->TravelTicksRemaining > 0 ||
        !Current->AdjacentProvinceIds.Contains(DestinationProvinceId))
    {
        return false;
    }
    Company->DestinationProvinceId = DestinationProvinceId;
    Company->TravelTicksRemaining = Company->SoldierType == EAPSoldierType::Scout ? 1 : 2;
    return true;
}

int32 UAPSimulationSubsystem::ApplyCasualties(int32 CompanyId, int32 CasualtyCount)
{
    FAPArmyState* Company = FindCompany(CompanyId);
    if (!Company)
    {
        return 0;
    }
    std::vector<AshfallSim::Household> ModelHouseholds = ToModelHouseholds(Households);
    AshfallSim::Army ModelArmy = ToModelArmy(*Company);
    const std::vector<int32> Lost = AshfallSim::ApplyCasualties(ModelHouseholds, ModelArmy, CasualtyCount);

    Company->HouseholdIds.Reset();
    for (int32 Id : ModelArmy.HouseholdIds) Company->HouseholdIds.Add(Id);
    for (int32 LostId : Lost)
    {
        if (FAPHouseholdState* Household = Households.FindByPredicate(
            [LostId](const FAPHouseholdState& Entry) { return Entry.HouseholdId == LostId; }))
        {
            Household->bAlive = false;
            Household->bMustered = false;
        }
    }
    return static_cast<int32>(Lost.size());
}

bool UAPSimulationSubsystem::ResolveBattle(int32 AttackerCompanyId, int32 DefenderCompanyId)
{
    FAPArmyState* Attacker = FindCompany(AttackerCompanyId);
    FAPArmyState* Defender = FindCompany(DefenderCompanyId);
    if (!Attacker || !Defender || Attacker->ProvinceId != Defender->ProvinceId)
    {
        return false;
    }
    const int32 AttackerLosses = AshfallSim::ResolveCasualties(
        Attacker->HouseholdIds.Num(), Defender->HouseholdIds.Num(), RandomSeed);
    const int32 DefenderLosses = AshfallSim::ResolveCasualties(
        Defender->HouseholdIds.Num(), Attacker->HouseholdIds.Num(), RandomSeed);
    ApplyCasualties(AttackerCompanyId, AttackerLosses);
    ApplyCasualties(DefenderCompanyId, DefenderLosses);
    return true;
}

bool UAPSimulationSubsystem::CaptureProvince(int32 CompanyId)
{
    FAPArmyState* Company = FindCompany(CompanyId);
    if (!Company || Company->TravelTicksRemaining > 0 || Company->HouseholdIds.IsEmpty())
    {
        return false;
    }
    FAPProvinceState* Province = Provinces.FindByPredicate(
        [Company](const FAPProvinceState& Entry) { return Entry.ProvinceId == Company->ProvinceId; });
    if (!Province)
    {
        return false;
    }
    const bool bEnemyPresent = Companies.ContainsByPredicate(
        [Company](const FAPArmyState& Other)
        {
            return Other.OwnerId != Company->OwnerId && Other.ProvinceId == Company->ProvinceId && !Other.HouseholdIds.IsEmpty();
        });
    if (bEnemyPresent)
    {
        return false;
    }
    Province->OwnerId = Company->OwnerId;
    return true;
}

bool UAPSimulationSubsystem::SaveSimulation(const FString& SlotName)
{
    UAPSaveGame* Save = Cast<UAPSaveGame>(UGameplayStatics::CreateSaveGameObject(UAPSaveGame::StaticClass()));
    Save->SimulationTick = SimulationTick;
    Save->RandomSeed = RandomSeed;
    Save->SettlementResources = SettlementResources;
    Save->SettlementBuildingSlots = SettlementBuildingSlots;
    Save->Households = Households;
    Save->Provinces = Provinces;
    Save->Companies = Companies;
    return UGameplayStatics::SaveGameToSlot(Save, SlotName, 0);
}

bool UAPSimulationSubsystem::LoadSimulation(const FString& SlotName)
{
    const UAPSaveGame* Save = Cast<UAPSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
    if (!Save)
    {
        return false;
    }
    SimulationTick = Save->SimulationTick;
    RandomSeed = Save->RandomSeed;
    SettlementResources = Save->SettlementResources;
    SettlementBuildingSlots = Save->SettlementBuildingSlots;
    Households = Save->Households;
    Provinces = Save->Provinces;
    Companies = Save->Companies;
    AccumulatedSeconds = 0.0f;
    return true;
}

FAPArmyState* UAPSimulationSubsystem::FindCompany(int32 CompanyId)
{
    return Companies.FindByPredicate(
        [CompanyId](const FAPArmyState& Entry) { return Entry.CompanyId == CompanyId; });
}

const FAPProvinceState* UAPSimulationSubsystem::FindProvince(int32 ProvinceId) const
{
    return Provinces.FindByPredicate(
        [ProvinceId](const FAPProvinceState& Entry) { return Entry.ProvinceId == ProvinceId; });
}
