#include "APPlayerController.h"
#include "Simulation/APSimulationSubsystem.h"

namespace
{
constexpr int32 HouseholdsPerCompany = 3;

FString SoldierTypeName(EAPSoldierType SoldierType)
{
    switch (SoldierType)
    {
    case EAPSoldierType::Ranged: return TEXT("Ranged");
    case EAPSoldierType::Scout: return TEXT("Scout");
    default: return TEXT("Spear");
    }
}
}

AAPPlayerController::AAPPlayerController()
{
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}

void AAPPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction(TEXT("MusterSpear"), IE_Pressed, this, &AAPPlayerController::MusterSpear);
    InputComponent->BindAction(TEXT("MusterRanged"), IE_Pressed, this, &AAPPlayerController::MusterRanged);
    InputComponent->BindAction(TEXT("MusterScout"), IE_Pressed, this, &AAPPlayerController::MusterScout);
    InputComponent->BindAction(TEXT("SelectCompany1"), IE_Pressed, this, &AAPPlayerController::SelectCompany1);
    InputComponent->BindAction(TEXT("SelectCompany2"), IE_Pressed, this, &AAPPlayerController::SelectCompany2);
    InputComponent->BindAction(TEXT("SelectCompany3"), IE_Pressed, this, &AAPPlayerController::SelectCompany3);
    InputComponent->BindAction(TEXT("SelectCompany4"), IE_Pressed, this, &AAPPlayerController::SelectCompany4);
    InputComponent->BindAction(TEXT("CycleTarget"), IE_Pressed, this, &AAPPlayerController::CycleTargetProvince);
    InputComponent->BindAction(TEXT("MoveCompany"), IE_Pressed, this, &AAPPlayerController::MoveSelectedCompany);
    InputComponent->BindAction(TEXT("ResolveBattle"), IE_Pressed, this, &AAPPlayerController::ResolveSelectedBattle);
    InputComponent->BindAction(TEXT("SaveSimulation"), IE_Pressed, this, &AAPPlayerController::SaveGame);
    InputComponent->BindAction(TEXT("LoadSimulation"), IE_Pressed, this, &AAPPlayerController::LoadGame);
}

void AAPPlayerController::MusterSpear() { MusterCompany(EAPSoldierType::Spear); }
void AAPPlayerController::MusterRanged() { MusterCompany(EAPSoldierType::Ranged); }
void AAPPlayerController::MusterScout() { MusterCompany(EAPSoldierType::Scout); }

void AAPPlayerController::MusterCompany(EAPSoldierType SoldierType)
{
    UAPSimulationSubsystem* Simulation = GetWorld()->GetSubsystem<UAPSimulationSubsystem>();
    if (!Simulation)
    {
        LastEvent = TEXT("Simulation unavailable");
        return;
    }

    const TArray<FAPArmyState> BeforeCompanies = Simulation->GetCompanies();
    TSet<int32> ExistingCompanyIds;
    for (const FAPArmyState& Company : BeforeCompanies)
    {
        ExistingCompanyIds.Add(Company.CompanyId);
    }

    TArray<int32> Recruits;
    const TArray<FAPHouseholdState> Households = Simulation->GetHouseholds();
    const EAPHouseholdJob PreferredJobs[] = {
        EAPHouseholdJob::Food, EAPHouseholdJob::Timber, EAPHouseholdJob::Metal
    };
    for (EAPHouseholdJob PreferredJob : PreferredJobs)
    {
        const FAPHouseholdState* Match = Households.FindByPredicate(
            [PreferredJob, &Recruits](const FAPHouseholdState& Household)
            {
                return Household.bAlive && !Household.bMustered && Household.Job == PreferredJob
                    && !Recruits.Contains(Household.HouseholdId);
            });
        if (Match)
        {
            Recruits.Add(Match->HouseholdId);
        }
    }
    for (const FAPHouseholdState& Household : Households)
    {
        if (Recruits.Num() >= HouseholdsPerCompany)
        {
            break;
        }
        if (Household.bAlive && !Household.bMustered && !Recruits.Contains(Household.HouseholdId))
        {
            Recruits.Add(Household.HouseholdId);
        }
    }

    if (Recruits.Num() != HouseholdsPerCompany || !Simulation->MusterCompany(SoldierType, Recruits))
    {
        LastEvent = FString::Printf(TEXT("%s muster rejected - check workers/resources/company limit"),
            *SoldierTypeName(SoldierType));
        return;
    }

    for (const FAPArmyState& Company : Simulation->GetCompanies())
    {
        if (Company.OwnerId == 0 && !ExistingCompanyIds.Contains(Company.CompanyId))
        {
            SelectedCompanyId = Company.CompanyId;
            break;
        }
    }
    LastEvent = FString::Printf(TEXT("%d households mustered as %s company"),
        Recruits.Num(), *SoldierTypeName(SoldierType));
}

void AAPPlayerController::SelectCompany1() { SelectCompanySlot(0); }
void AAPPlayerController::SelectCompany2() { SelectCompanySlot(1); }
void AAPPlayerController::SelectCompany3() { SelectCompanySlot(2); }
void AAPPlayerController::SelectCompany4() { SelectCompanySlot(3); }

void AAPPlayerController::SelectCompanySlot(int32 SlotIndex)
{
    const UAPSimulationSubsystem* Simulation = GetWorld()->GetSubsystem<UAPSimulationSubsystem>();
    if (!Simulation)
    {
        return;
    }
    TArray<FAPArmyState> PlayerCompanies = Simulation->GetCompanies().FilterByPredicate(
        [](const FAPArmyState& Company)
        {
            return Company.OwnerId == 0 && !Company.HouseholdIds.IsEmpty();
        });
    if (!PlayerCompanies.IsValidIndex(SlotIndex))
    {
        LastEvent = FString::Printf(TEXT("Company slot %d is empty"), SlotIndex + 1);
        return;
    }
    SelectedCompanyId = PlayerCompanies[SlotIndex].CompanyId;
    LastEvent = FString::Printf(TEXT("Selected Company %d (%s)"), SlotIndex + 1,
        *SoldierTypeName(PlayerCompanies[SlotIndex].SoldierType));
}

void AAPPlayerController::CycleTargetProvince()
{
    TargetProvinceId = (TargetProvinceId + 1) % UAPSimulationSubsystem::ProvinceCount;
    LastEvent = FString::Printf(TEXT("Target Province %d"), TargetProvinceId + 1);
}

void AAPPlayerController::MoveSelectedCompany()
{
    UAPSimulationSubsystem* Simulation = GetWorld()->GetSubsystem<UAPSimulationSubsystem>();
    if (!Simulation || SelectedCompanyId == INDEX_NONE)
    {
        LastEvent = TEXT("Select a company before moving");
        return;
    }
    if (Simulation->OrderMove(SelectedCompanyId, TargetProvinceId))
    {
        LastEvent = FString::Printf(TEXT("Company moving to Province %d"), TargetProvinceId + 1);
    }
    else
    {
        LastEvent = TEXT("Move rejected - target must be adjacent and company idle");
    }
}

void AAPPlayerController::ResolveSelectedBattle()
{
    UAPSimulationSubsystem* Simulation = GetWorld()->GetSubsystem<UAPSimulationSubsystem>();
    if (!Simulation || SelectedCompanyId == INDEX_NONE)
    {
        LastEvent = TEXT("Select a company before battle");
        return;
    }

    const TArray<FAPArmyState> Before = Simulation->GetCompanies();
    const FAPArmyState* AttackerBefore = Before.FindByPredicate(
        [this](const FAPArmyState& Company) { return Company.CompanyId == SelectedCompanyId; });
    if (!AttackerBefore || AttackerBefore->TravelTicksRemaining > 0)
    {
        LastEvent = TEXT("Battle rejected - company is unavailable or moving");
        return;
    }
    const FAPArmyState* DefenderBefore = Before.FindByPredicate(
        [AttackerBefore](const FAPArmyState& Company)
        {
            return Company.OwnerId != AttackerBefore->OwnerId
                && Company.ProvinceId == AttackerBefore->ProvinceId && !Company.HouseholdIds.IsEmpty();
        });

    const int32 StrengthBefore = AttackerBefore->HouseholdIds.Num();
    if (DefenderBefore)
    {
        const int32 DefenderId = DefenderBefore->CompanyId;
        if (!Simulation->ResolveBattle(SelectedCompanyId, DefenderId))
        {
            LastEvent = TEXT("Battle could not be resolved");
            return;
        }
        const TArray<FAPArmyState> After = Simulation->GetCompanies();
        const FAPArmyState* AttackerAfter = After.FindByPredicate(
            [this](const FAPArmyState& Company) { return Company.CompanyId == SelectedCompanyId; });
        const FAPArmyState* DefenderAfter = After.FindByPredicate(
            [DefenderId](const FAPArmyState& Company) { return Company.CompanyId == DefenderId; });
        const int32 StrengthAfter = AttackerAfter ? AttackerAfter->HouseholdIds.Num() : 0;
        const int32 Casualties = StrengthBefore - StrengthAfter;
        if (!DefenderAfter || DefenderAfter->HouseholdIds.IsEmpty())
        {
            const bool bCaptured = Simulation->CaptureProvince(SelectedCompanyId);
            LastEvent = FString::Printf(TEXT("Battle won - %d casualties%s"), Casualties,
                bCaptured ? TEXT(" - province captured") : TEXT(""));
        }
        else
        {
            LastEvent = FString::Printf(TEXT("Battle resolved - %d casualties - enemy strength %d"),
                Casualties, DefenderAfter->HouseholdIds.Num());
        }
        return;
    }

    if (Simulation->CaptureProvince(SelectedCompanyId))
    {
        LastEvent = FString::Printf(TEXT("Province %d captured"), AttackerBefore->ProvinceId + 1);
    }
    else
    {
        LastEvent = TEXT("No enemy or capturable province here");
    }
}

void AAPPlayerController::SaveGame()
{
    UAPSimulationSubsystem* Simulation = GetWorld()->GetSubsystem<UAPSimulationSubsystem>();
    LastEvent = Simulation && Simulation->SaveSimulation(TEXT("AshfallSlice"))
        ? TEXT("Game saved") : TEXT("Save failed");
}

void AAPPlayerController::LoadGame()
{
    UAPSimulationSubsystem* Simulation = GetWorld()->GetSubsystem<UAPSimulationSubsystem>();
    if (!Simulation || !Simulation->LoadSimulation(TEXT("AshfallSlice")))
    {
        LastEvent = TEXT("Load failed - no save found");
        return;
    }
    const TArray<FAPArmyState> LoadedCompanies = Simulation->GetCompanies();
    const FAPArmyState* Selected = LoadedCompanies.FindByPredicate(
        [this](const FAPArmyState& Company)
        {
            return Company.CompanyId == SelectedCompanyId && Company.OwnerId == 0 && !Company.HouseholdIds.IsEmpty();
        });
    if (!Selected)
    {
        SelectedCompanyId = INDEX_NONE;
    }
    LastEvent = TEXT("Game loaded");
}
