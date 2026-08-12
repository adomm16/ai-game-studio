#include "UI/APStrategyHUD.h"

#include "APPlayerController.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Simulation/APSimulationSubsystem.h"

namespace
{
FString SoldierTypeName(EAPSoldierType Type)
{
    switch (Type)
    {
    case EAPSoldierType::Ranged: return TEXT("Ranged");
    case EAPSoldierType::Scout: return TEXT("Scout");
    default: return TEXT("Spear");
    }
}

FString OwnerName(int32 OwnerId)
{
    return OwnerId == 0 ? TEXT("Player") : (OwnerId == 1 ? TEXT("AI") : TEXT("Neutral"));
}
}

void AAPStrategyHUD::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Display, TEXT("APStrategyHUD active - interactive HUD enabled"));
}

void AAPStrategyHUD::DrawPanel(float X, float Y, float Width, float Height, const FLinearColor& Color) const
{
    const_cast<AAPStrategyHUD*>(this)->DrawRect(Color, X, Y, Width, Height);
}

void AAPStrategyHUD::DrawButton(const FString& Label, const FBox2D& Bounds, bool bEnabled) const
{
    const FLinearColor Fill = bEnabled ? FLinearColor(0.12f, 0.28f, 0.38f, 0.94f)
        : FLinearColor(0.12f, 0.12f, 0.12f, 0.82f);
    DrawPanel(Bounds.Min.X, Bounds.Min.Y, Bounds.GetSize().X, Bounds.GetSize().Y, Fill);
    const_cast<AAPStrategyHUD*>(this)->DrawText(Label,
        bEnabled ? FLinearColor::White : FLinearColor(0.45f, 0.45f, 0.45f),
        Bounds.Min.X + 12.0f, Bounds.Min.Y + 9.0f, GEngine->GetSmallFont(), 1.0f, false);
}

bool AAPStrategyHUD::HandlePointerInput(const FVector2D& ScreenPosition, bool bRightClick)
{
    if (bRightClick) return false;
    AAPPlayerController* Controller = Cast<AAPPlayerController>(GetOwningPlayerController());
    if (!Controller) return false;
    if (SpearButton.IsInside(ScreenPosition)) { Controller->RequestMuster(EAPSoldierType::Spear); return true; }
    if (RangedButton.IsInside(ScreenPosition)) { Controller->RequestMuster(EAPSoldierType::Ranged); return true; }
    if (ScoutButton.IsInside(ScreenPosition)) { Controller->RequestMuster(EAPSoldierType::Scout); return true; }
    if (AttackButton.IsInside(ScreenPosition))
    {
        if (bAttackEnabled) Controller->RequestAttack();
        return true;
    }
    return false;
}

void AAPStrategyHUD::DrawHUD()
{
    Super::DrawHUD();
    const UAPSimulationSubsystem* Simulation = GetWorld()->GetSubsystem<UAPSimulationSubsystem>();
    AAPPlayerController* Controller = Cast<AAPPlayerController>(GetOwningPlayerController());
    if (!Simulation || !Controller || !Canvas || !GEngine) return;

    const float ViewWidth = Canvas->ClipX > 0.0f ? Canvas->ClipX : static_cast<float>(Canvas->SizeX);
    const float ViewHeight = Canvas->ClipY > 0.0f ? Canvas->ClipY : static_cast<float>(Canvas->SizeY);
    if (ViewWidth <= 0.0f || ViewHeight <= 0.0f) return;

    const FAPResources Resources = Simulation->GetResources();
    const TArray<FAPHouseholdState> Households = Simulation->GetHouseholds();
    const TArray<FAPArmyState> Companies = Simulation->GetCompanies();
    const TArray<FAPProvinceState> Provinces = Simulation->GetProvinces();
    int32 Alive = 0, Working = 0, Mustered = 0, PlayerCompanyCount = 0, Controlled = 0;
    for (const FAPHouseholdState& Household : Households)
    {
        Alive += Household.bAlive;
        Working += Household.bAlive && !Household.bMustered && Household.Job != EAPHouseholdJob::Idle;
        Mustered += Household.bAlive && Household.bMustered;
    }
    for (const FAPArmyState& Company : Companies)
        PlayerCompanyCount += Company.OwnerId == 0 && !Company.HouseholdIds.IsEmpty();
    for (const FAPProvinceState& Province : Provinces) Controlled += Province.OwnerId == 0;

    DrawPanel(20.0f, 20.0f, 270.0f, 330.0f, FLinearColor(0.025f, 0.035f, 0.045f, 0.88f));
    float Y = 36.0f;
    auto Line = [this, &Y](const FString& Text, const FLinearColor& Color = FLinearColor::White, float Scale = 1.0f)
    {
        DrawText(Text, Color, 36.0f, Y, GEngine->GetSmallFont(), Scale, false);
        Y += 23.0f;
    };
    Line(TEXT("ASHFALL PROVINCES"), FLinearColor(1.0f, 0.72f, 0.22f), 1.2f);
    Line(TEXT("RESOURCES"), FLinearColor(0.55f, 0.82f, 1.0f));
    Line(FString::Printf(TEXT("Food %d   Timber %d"), Resources.Food, Resources.Timber));
    Line(FString::Printf(TEXT("Metal %d   Authority %d"), Resources.Metal, Resources.Authority));
    Line(TEXT("POPULATION"), FLinearColor(0.55f, 0.82f, 1.0f));
    Line(FString::Printf(TEXT("Alive %d   Working %d"), Alive, Working));
    Line(FString::Printf(TEXT("Mustered %d   Dead %d"), Mustered, Households.Num() - Alive));
    Line(TEXT("ARMY"), FLinearColor(0.55f, 0.82f, 1.0f));
    Line(FString::Printf(TEXT("Companies %d / %d"), PlayerCompanyCount, UAPSimulationSubsystem::MaxCompanies));
    Line(FString::Printf(TEXT("Controlled Provinces %d / %d"), Controlled, UAPSimulationSubsystem::ProvinceCount));
    Line(TEXT("WASD / arrows pan | wheel zoom"), FLinearColor(0.65f, 0.65f, 0.65f), 0.85f);

    const float RightX = FMath::Max(20.0f, ViewWidth - 340.0f);
    DrawPanel(RightX, 20.0f, 320.0f, 330.0f, FLinearColor(0.025f, 0.035f, 0.045f, 0.88f));
    float RightY = 38.0f;
    auto RightLine = [this, RightX, &RightY](const FString& Text, const FLinearColor& Color = FLinearColor::White)
    {
        DrawText(Text, Color, RightX + 18.0f, RightY, GEngine->GetSmallFont(), 1.0f, false);
        RightY += 23.0f;
    };

    RightLine(TEXT("SELECTED PROVINCE"), FLinearColor(1.0f, 0.72f, 0.22f));
    const int32 ProvinceId = Controller->GetSelectedProvinceId();
    const FAPProvinceState* Province = Provinces.FindByPredicate(
        [ProvinceId](const FAPProvinceState& Entry) { return Entry.ProvinceId == ProvinceId; });
    if (Province)
    {
        int32 FriendlyHere = 0, EnemyHere = 0;
        for (const FAPArmyState& Company : Companies)
        {
            if (Company.ProvinceId != ProvinceId || Company.HouseholdIds.IsEmpty()) continue;
            Company.OwnerId == 0 ? ++FriendlyHere : ++EnemyHere;
        }
        RightLine(FString::Printf(TEXT("Province %d"), ProvinceId + 1));
        RightLine(FString::Printf(TEXT("Owner: %s"), *OwnerName(Province->OwnerId)));
        RightLine(FString::Printf(TEXT("Garrison: %d"), EnemyHere));
        RightLine(FString::Printf(TEXT("Player companies here: %d"), FriendlyHere));
    }
    RightLine(TEXT("SELECTED COMPANY"), FLinearColor(1.0f, 0.72f, 0.22f));
    const int32 CompanyId = Controller->GetSelectedCompanyId();
    const FAPArmyState* Selected = Companies.FindByPredicate(
        [CompanyId](const FAPArmyState& Entry) { return Entry.CompanyId == CompanyId; });
    bAttackEnabled = false;
    if (Selected)
    {
        RightLine(FString::Printf(TEXT("Company %d | %s"), Selected->CompanyId, *SoldierTypeName(Selected->SoldierType)));
        RightLine(FString::Printf(TEXT("Strength %d | Province %d"), Selected->HouseholdIds.Num(), Selected->ProvinceId + 1));
        RightLine(Selected->TravelTicksRemaining > 0 ? TEXT("Status: Moving") : TEXT("Status: Ready"));
        bAttackEnabled = Selected->TravelTicksRemaining == 0 && Companies.ContainsByPredicate(
            [Selected](const FAPArmyState& Other)
            {
                return Other.OwnerId != Selected->OwnerId && Other.ProvinceId == Selected->ProvinceId
                    && !Other.HouseholdIds.IsEmpty();
            });
    }
    else RightLine(TEXT("None"));

    constexpr float MusterPanelWidth = 500.0f;
    constexpr float MusterPanelHeight = 260.0f;
    const float MusterX = FMath::Clamp(20.0f, 0.0f, FMath::Max(0.0f, ViewWidth - MusterPanelWidth));
    const float MusterY = FMath::Clamp(ViewHeight - MusterPanelHeight - 20.0f, 0.0f,
        FMath::Max(0.0f, ViewHeight - MusterPanelHeight));
    DrawPanel(MusterX, MusterY, FMath::Min(MusterPanelWidth, ViewWidth),
        FMath::Min(MusterPanelHeight, ViewHeight), FLinearColor(0.01f, 0.015f, 0.02f, 0.98f));
    DrawText(TEXT("RECRUIT / MUSTER"), FLinearColor(1.0f, 0.78f, 0.15f),
        MusterX + 18.0f, MusterY + 14.0f, GEngine->GetMediumFont(), 1.15f, false);
    DrawText(TEXT("Requires 3 available workers | Company limit 4"), FLinearColor::White,
        MusterX + 18.0f, MusterY + 48.0f, GEngine->GetSmallFont(), 1.0f, false);
    const bool bCanMuster = Working >= 3 && PlayerCompanyCount < UAPSimulationSubsystem::MaxCompanies;
    const float ButtonX = MusterX + 18.0f;
    constexpr float ButtonWidth = 464.0f;
    constexpr float ButtonHeight = 50.0f;
    SpearButton = FBox2D(FVector2D(ButtonX, MusterY + 82.0f),
        FVector2D(ButtonX + ButtonWidth, MusterY + 82.0f + ButtonHeight));
    RangedButton = FBox2D(FVector2D(ButtonX, MusterY + 138.0f),
        FVector2D(ButtonX + ButtonWidth, MusterY + 138.0f + ButtonHeight));
    ScoutButton = FBox2D(FVector2D(ButtonX, MusterY + 194.0f),
        FVector2D(ButtonX + ButtonWidth, MusterY + 194.0f + ButtonHeight));
    DrawButton(TEXT("SPEAR - 3 HOUSEHOLDS"), SpearButton, bCanMuster);
    DrawButton(TEXT("RANGED - 3 HOUSEHOLDS"), RangedButton, bCanMuster);
    DrawButton(TEXT("SCOUT - 3 HOUSEHOLDS"), ScoutButton, bCanMuster);

    AttackButton = FBox2D(FVector2D(RightX + 18.0f, 285.0f), FVector2D(RightX + 302.0f, 333.0f));
    DrawButton(TEXT("ATTACK"), AttackButton, bAttackEnabled);

    const float EventX = FMath::Min(540.0f, ViewWidth * 0.38f);
    const float EventWidth = FMath::Max(0.0f, ViewWidth - EventX - 360.0f);
    DrawPanel(EventX, ViewHeight - 95.0f, EventWidth, 70.0f,
        FLinearColor(0.025f, 0.035f, 0.045f, 0.92f));
    DrawText(TEXT("LAST EVENT"), FLinearColor(1.0f, 0.72f, 0.22f), EventX + 16.0f, ViewHeight - 83.0f,
        GEngine->GetSmallFont(), 0.9f, false);
    DrawText(Controller->GetLastEvent(), FLinearColor::White, EventX + 16.0f, ViewHeight - 57.0f,
        GEngine->GetSmallFont(), 1.0f, false);

    if (Controller->IsBattleResultVisible())
    {
        const float BoxX = ViewWidth * 0.5f - 180.0f;
        const float BoxY = ViewHeight * 0.5f - 110.0f;
        DrawPanel(BoxX, BoxY, 360.0f, 220.0f, FLinearColor(0.04f, 0.05f, 0.06f, 0.96f));
        DrawText(TEXT("BATTLE RESULT"), FLinearColor(1.0f, 0.72f, 0.22f), BoxX + 18.0f, BoxY + 16.0f,
            GEngine->GetSmallFont(), 1.2f, false);
        TArray<FString> ResultLines;
        Controller->GetBattleResult().ParseIntoArrayLines(ResultLines);
        float ResultY = BoxY + 55.0f;
        for (const FString& ResultLine : ResultLines)
        {
            DrawText(ResultLine, FLinearColor::White, BoxX + 18.0f, ResultY, GEngine->GetSmallFont(), 1.0f, false);
            ResultY += 27.0f;
        }
    }
}
