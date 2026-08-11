#include "UI/APStrategyHUD.h"

#include "APPlayerController.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Simulation/APSimulationSubsystem.h"

void AAPStrategyHUD::DrawHUD()
{
    Super::DrawHUD();
    const UAPSimulationSubsystem* Simulation = GetWorld()->GetSubsystem<UAPSimulationSubsystem>();
    if (!Simulation || !Canvas || !GEngine)
    {
        return;
    }

    const FAPResources Resources = Simulation->GetResources();
    const TArray<FAPHouseholdState> Households = Simulation->GetHouseholds();
    const TArray<FAPArmyState> Companies = Simulation->GetCompanies();
    const TArray<FAPProvinceState> Provinces = Simulation->GetProvinces();
    const AAPPlayerController* StrategyController = Cast<AAPPlayerController>(GetOwningPlayerController());
    int32 Alive = 0;
    int32 Working = 0;
    int32 Mustered = 0;
    int32 PlayerCompanies = 0;
    int32 ControlledProvinces = 0;
    for (const FAPHouseholdState& Household : Households)
    {
        Alive += Household.bAlive ? 1 : 0;
        Mustered += Household.bAlive && Household.bMustered ? 1 : 0;
        Working += Household.bAlive && !Household.bMustered && Household.Job != EAPHouseholdJob::Idle ? 1 : 0;
    }
    for (const FAPArmyState& Company : Companies)
    {
        PlayerCompanies += Company.OwnerId == 0 && !Company.HouseholdIds.IsEmpty() ? 1 : 0;
    }
    for (const FAPProvinceState& Province : Provinces)
    {
        ControlledProvinces += Province.OwnerId == 0 ? 1 : 0;
    }

    FString SelectedCompanyText = TEXT("None");
    int32 SelectedStrength = 0;
    int32 TargetProvince = StrategyController ? StrategyController->GetTargetProvinceId() : 0;
    if (StrategyController)
    {
        const int32 SelectedCompanyId = StrategyController->GetSelectedCompanyId();
        const FAPArmyState* SelectedCompany = Companies.FindByPredicate(
            [SelectedCompanyId](const FAPArmyState& Company) { return Company.CompanyId == SelectedCompanyId; });
        if (SelectedCompany)
        {
            SelectedCompanyText = FString::Printf(TEXT("ID %d"), SelectedCompany->CompanyId);
            SelectedStrength = SelectedCompany->HouseholdIds.Num();
        }
    }

    const FString EventText = StrategyController ? StrategyController->GetLastEvent() : TEXT("Ready");
    const TArray<FString> Lines = {
        TEXT("ASHFALL PROVINCES - GREYBOX"),
        FString::Printf(TEXT("Tick: %lld"), Simulation->GetSimulationTick()),
        FString::Printf(TEXT("Food: %d"), Resources.Food),
        FString::Printf(TEXT("Timber: %d"), Resources.Timber),
        FString::Printf(TEXT("Metal: %d"), Resources.Metal),
        FString::Printf(TEXT("Authority: %d"), Resources.Authority),
        FString::Printf(TEXT("Households alive: %d / %d"), Alive, Households.Num()),
        FString::Printf(TEXT("Available workers: %d"), Working),
        FString::Printf(TEXT("Mustered households: %d"), Mustered),
        FString::Printf(TEXT("Casualties / dead: %d"), Households.Num() - Alive),
        FString::Printf(TEXT("Companies: %d / 4"), PlayerCompanies),
        FString::Printf(TEXT("Selected Company: %s"), *SelectedCompanyText),
        FString::Printf(TEXT("Company Strength: %d"), SelectedStrength),
        FString::Printf(TEXT("Target Province: %d"), TargetProvince + 1),
        FString::Printf(TEXT("Controlled provinces: %d / 6"), ControlledProvinces),
        TEXT("M/R/C muster | 1-4 select | Tab target | G move | B battle | F5/F9 save/load"),
        TEXT("WASD / Arrow Keys pan | Mouse Wheel zoom"),
        FString::Printf(TEXT("EVENT: %s"), *EventText)
    };

    float Y = 36.0f;
    for (int32 Index = 0; Index < Lines.Num(); ++Index)
    {
        const FLinearColor Color = Index == 0 ? FLinearColor(1.0f, 0.72f, 0.22f) : FLinearColor::White;
        DrawText(Lines[Index], Color, 36.0f, Y, GEngine->GetSmallFont(), Index == 0 ? 1.35f : 1.0f, false);
        Y += Index == 0 ? 34.0f : 24.0f;
    }
}
