#include "UI/APPrototypeWidget.h"

#include "APPlayerController.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Simulation/APSimulationSubsystem.h"
#include "Styling/CoreStyle.h"

namespace
{
FSlateFontInfo PrototypeFont(int32 Size, bool bBold = false)
{
    return FCoreStyle::GetDefaultFontStyle(bBold ? TEXT("Bold") : TEXT("Regular"), Size);
}

UTextBlock* AddText(UWidgetTree* Tree, UVerticalBox* Box, const FString& Text, int32 Size,
    const FLinearColor& Color = FLinearColor::White, bool bBold = false)
{
    UTextBlock* Label = Tree->ConstructWidget<UTextBlock>();
    Label->SetText(FText::FromString(Text));
    Label->SetFont(PrototypeFont(Size, bBold));
    Label->SetColorAndOpacity(FSlateColor(Color));
    Label->SetAutoWrapText(true);
    UVerticalBoxSlot* Slot = Box->AddChildToVerticalBox(Label);
    Slot->SetPadding(FMargin(0.0f, 3.0f));
    return Label;
}

UButton* AddCommandButton(UWidgetTree* Tree, UVerticalBox* Box, const FString& Text)
{
    UButton* Button = Tree->ConstructWidget<UButton>();
    FButtonStyle Style = Button->GetStyle();
    Style.Normal.TintColor = FSlateColor(FLinearColor(.07f,.22f,.29f,.96f));
    Style.Hovered.TintColor = FSlateColor(FLinearColor(.12f,.42f,.55f,1.0f));
    Style.Pressed.TintColor = FSlateColor(FLinearColor(.68f,.42f,.12f,1.0f));
    Style.Disabled.TintColor = FSlateColor(FLinearColor(.08f,.08f,.08f,.62f));
    Button->SetStyle(Style);
    UTextBlock* Label = Tree->ConstructWidget<UTextBlock>();
    Label->SetText(FText::FromString(Text));
    Label->SetFont(PrototypeFont(18, true));
    Label->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    Label->SetJustification(ETextJustify::Center);
    Button->SetContent(Label);
    UVerticalBoxSlot* Slot = Box->AddChildToVerticalBox(Button);
    Slot->SetPadding(FMargin(0.0f, 5.0f));
    Slot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
    return Button;
}

UBorder* AddPanel(UWidgetTree* Tree, UCanvasPanel* Root, const FAnchors& Anchors,
    const FVector2D& Position, const FVector2D& Size, const FVector2D& Alignment = FVector2D::ZeroVector)
{
    UBorder* Border = Tree->ConstructWidget<UBorder>();
    Border->SetBrushColor(FLinearColor(0.012f, 0.018f, 0.023f, 0.89f));
    Border->SetPadding(FMargin(14.0f));
    UCanvasPanelSlot* Slot = Root->AddChildToCanvas(Border);
    Slot->SetAnchors(Anchors);
    Slot->SetPosition(Position);
    Slot->SetSize(Size);
    Slot->SetAlignment(Alignment);
    return Border;
}

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

void UAPPrototypeWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    BuildInterface();
    SpearButton->OnClicked.AddDynamic(this, &UAPPrototypeWidget::HandleSpearClicked);
    RangedButton->OnClicked.AddDynamic(this, &UAPPrototypeWidget::HandleRangedClicked);
    ScoutButton->OnClicked.AddDynamic(this, &UAPPrototypeWidget::HandleScoutClicked);
    AttackButton->OnClicked.AddDynamic(this, &UAPPrototypeWidget::HandleAttackClicked);
    SetVisibility(ESlateVisibility::Visible);
}

void UAPPrototypeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    RefreshFromSimulation();
}

bool UAPPrototypeWidget::AreCommandCallbacksBound() const
{
    return SpearButton && RangedButton && ScoutButton && AttackButton
        && SpearButton->OnClicked.IsBound() && RangedButton->OnClicked.IsBound()
        && ScoutButton->OnClicked.IsBound() && AttackButton->OnClicked.IsBound();
}

void UAPPrototypeWidget::BuildInterface()
{
    if (!WidgetTree) WidgetTree = NewObject<UWidgetTree>(this, TEXT("PrototypeWidgetTree"));
    UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>();
    WidgetTree->RootWidget = Root;

    UBorder* TitlePanel = AddPanel(WidgetTree, Root, FAnchors(0.5f, 0.0f),
        FVector2D(0.0f, 12.0f), FVector2D(480.0f, 82.0f), FVector2D(0.5f, 0.0f));
    UVerticalBox* Title = WidgetTree->ConstructWidget<UVerticalBox>();
    TitlePanel->SetContent(Title);
    UTextBlock* GameTitle = AddText(WidgetTree, Title, TEXT("ASHFALL PROVINCES"), 28,
        FLinearColor(1.0f, 0.72f, 0.16f), true);
    GameTitle->SetJustification(ETextJustify::Center);
    UTextBlock* VersionTitle = AddText(WidgetTree, Title, TEXT("VISUAL PROTOTYPE v0.2"), 18,
        FLinearColor::White, true);
    VersionTitle->SetJustification(ETextJustify::Center);

    UBorder* OverviewPanel = AddPanel(WidgetTree, Root, FAnchors(0.0f, 0.0f),
        FVector2D(14.0f, 14.0f), FVector2D(300.0f, 275.0f));
    UVerticalBox* Overview = WidgetTree->ConstructWidget<UVerticalBox>();
    OverviewPanel->SetContent(Overview);
    AddText(WidgetTree, Overview, TEXT("REALM OVERVIEW"), 18,
        FLinearColor(1.0f, 0.72f, 0.16f), true);
    ResourcesText = AddText(WidgetTree, Overview, TEXT("RESOURCES"), 15);
    PopulationText = AddText(WidgetTree, Overview, TEXT("POPULATION"), 15);
    ArmyText = AddText(WidgetTree, Overview, TEXT("ARMY"), 15);

    UBorder* MusterPanel = AddPanel(WidgetTree, Root, FAnchors(0.0f, 1.0f),
        FVector2D(14.0f, -245.0f), FVector2D(390.0f, 230.0f));
    UVerticalBox* Muster = WidgetTree->ConstructWidget<UVerticalBox>();
    MusterPanel->SetContent(Muster);
    AddText(WidgetTree, Muster, TEXT("RECRUIT / MUSTER"), 19, FLinearColor(1.0f, 0.72f, 0.16f), true);
    SpearButton = AddCommandButton(WidgetTree, Muster, TEXT("SPEAR - 3 HOUSEHOLDS"));
    RangedButton = AddCommandButton(WidgetTree, Muster, TEXT("RANGED - 3 HOUSEHOLDS"));
    ScoutButton = AddCommandButton(WidgetTree, Muster, TEXT("SCOUT - 3 HOUSEHOLDS"));

    UBorder* SelectionPanel = AddPanel(WidgetTree, Root, FAnchors(1.0f, 0.0f),
        FVector2D(-324.0f, 14.0f), FVector2D(310.0f, 350.0f));
    UVerticalBox* Selection = WidgetTree->ConstructWidget<UVerticalBox>();
    SelectionPanel->SetContent(Selection);
    AddText(WidgetTree, Selection, TEXT("SELECTED PROVINCE / COMPANY"), 19,
        FLinearColor(1.0f, 0.72f, 0.16f), true);
    SelectionText = AddText(WidgetTree, Selection, TEXT("None"), 17);
    AttackButton = AddCommandButton(WidgetTree, Selection, TEXT("ATTACK"));

    UBorder* EventPanel = AddPanel(WidgetTree, Root, FAnchors(0.5f, 1.0f),
        FVector2D(0.0f, -20.0f), FVector2D(650.0f, 80.0f), FVector2D(0.5f, 1.0f));
    UVerticalBox* EventBox = WidgetTree->ConstructWidget<UVerticalBox>();
    EventPanel->SetContent(EventBox);
    AddText(WidgetTree, EventBox, TEXT("LAST EVENT"), 16, FLinearColor(1.0f, 0.72f, 0.16f), true);
    LastEventText = AddText(WidgetTree, EventBox, TEXT("Ready"), 17);

    UBorder* BattlePanel = AddPanel(WidgetTree, Root, FAnchors(0.5f, 0.5f),
        FVector2D::ZeroVector, FVector2D(420.0f, 260.0f), FVector2D(0.5f, 0.5f));
    UVerticalBox* BattleBox = WidgetTree->ConstructWidget<UVerticalBox>();
    BattlePanel->SetContent(BattleBox);
    AddText(WidgetTree, BattleBox, TEXT("BATTLE RESULT"), 22, FLinearColor(1.0f, 0.72f, 0.16f), true);
    BattleResultText = AddText(WidgetTree, BattleBox, TEXT(""), 18);
    BattlePanel->SetVisibility(ESlateVisibility::Collapsed);
    BattleResultText->SetToolTipText(FText::FromString(TEXT("BattleResultPanel")));
}

void UAPPrototypeWidget::RefreshFromSimulation()
{
    AAPPlayerController* Controller = GetOwningPlayer<AAPPlayerController>();
    const UAPSimulationSubsystem* Simulation = GetWorld() ? GetWorld()->GetSubsystem<UAPSimulationSubsystem>() : nullptr;
    if (!Controller || !Simulation) return;

    const FAPResources Resources = Simulation->GetResources();
    const TArray<FAPHouseholdState> Households = Simulation->GetHouseholds();
    const TArray<FAPArmyState> Companies = Simulation->GetCompanies();
    const TArray<FAPProvinceState> Provinces = Simulation->GetProvinces();
    int32 Alive = 0, Working = 0, Mustered = 0, PlayerCompanies = 0, Controlled = 0;
    for (const FAPHouseholdState& Household : Households)
    {
        Alive += Household.bAlive;
        Working += Household.bAlive && !Household.bMustered && Household.Job != EAPHouseholdJob::Idle;
        Mustered += Household.bAlive && Household.bMustered;
    }
    for (const FAPArmyState& Company : Companies)
        PlayerCompanies += Company.OwnerId == 0 && !Company.HouseholdIds.IsEmpty();
    for (const FAPProvinceState& Province : Provinces) Controlled += Province.OwnerId == 0;

    ResourcesText->SetText(FText::FromString(FString::Printf(
        TEXT("RESOURCES\nFood %d    Timber %d\nMetal %d    Authority %d"),
        Resources.Food, Resources.Timber, Resources.Metal, Resources.Authority)));
    PopulationText->SetText(FText::FromString(FString::Printf(
        TEXT("POPULATION\nAlive %d    Working %d\nMustered %d    Dead %d"),
        Alive, Working, Mustered, Households.Num() - Alive)));
    ArmyText->SetText(FText::FromString(FString::Printf(
        TEXT("ARMY\nCompanies %d / %d\nControlled provinces %d / %d"), PlayerCompanies,
        UAPSimulationSubsystem::MaxCompanies, Controlled, UAPSimulationSubsystem::ProvinceCount)));

    FString SelectionInfo;
    const int32 ProvinceId = Controller->GetSelectedProvinceId();
    const FAPProvinceState* Province = Provinces.FindByPredicate(
        [ProvinceId](const FAPProvinceState& Entry) { return Entry.ProvinceId == ProvinceId; });
    int32 Garrison = 0;
    for (const FAPArmyState& Company : Companies)
        Garrison += Company.OwnerId == 1 && Company.ProvinceId == ProvinceId && !Company.HouseholdIds.IsEmpty();
    if (Province)
    {
        SelectionInfo = FString::Printf(TEXT("Province %d\nOwner: %s\nGarrison: %d"),
            ProvinceId + 1, *OwnerName(Province->OwnerId), Garrison);
    }
    const int32 CompanyId = Controller->GetSelectedCompanyId();
    const FAPArmyState* Company = Companies.FindByPredicate(
        [CompanyId](const FAPArmyState& Entry) { return Entry.CompanyId == CompanyId; });
    if (Company)
    {
        SelectionInfo += FString::Printf(TEXT("\n\nCompany %d\nType: %s\nStrength: %d\nProvince: %d\nStatus: %s"),
            Company->CompanyId, *SoldierTypeName(Company->SoldierType), Company->HouseholdIds.Num(),
            Company->ProvinceId + 1, Company->TravelTicksRemaining > 0 ? TEXT("Moving") : TEXT("Idle / Arrived"));
    }
    SelectionText->SetText(FText::FromString(SelectionInfo));

    const bool bCanMuster = Working >= 3 && PlayerCompanies < UAPSimulationSubsystem::MaxCompanies
        && Resources.Authority >= 6 && Resources.Metal >= 3;
    SpearButton->SetIsEnabled(bCanMuster);
    RangedButton->SetIsEnabled(bCanMuster);
    ScoutButton->SetIsEnabled(bCanMuster);
    const bool bCanAttack = Company && Company->TravelTicksRemaining == 0 && Companies.ContainsByPredicate(
        [Company](const FAPArmyState& Other)
        {
            return Other.OwnerId != Company->OwnerId && Other.ProvinceId == Company->ProvinceId
                && !Other.HouseholdIds.IsEmpty();
        });
    AttackButton->SetIsEnabled(bCanAttack);
    LastEventText->SetText(FText::FromString(Controller->GetLastEvent()));

    UWidget* BattlePanel = BattleResultText ? BattleResultText->GetParent()->GetParent() : nullptr;
    if (BattlePanel)
    {
        BattlePanel->SetVisibility(Controller->IsBattleResultVisible()
            ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
        BattleResultText->SetText(FText::FromString(Controller->GetBattleResult()));
    }
}

void UAPPrototypeWidget::HandleSpearClicked()
{
    if (AAPPlayerController* Controller = GetOwningPlayer<AAPPlayerController>()) Controller->RequestMuster(EAPSoldierType::Spear);
}

void UAPPrototypeWidget::HandleRangedClicked()
{
    if (AAPPlayerController* Controller = GetOwningPlayer<AAPPlayerController>()) Controller->RequestMuster(EAPSoldierType::Ranged);
}

void UAPPrototypeWidget::HandleScoutClicked()
{
    if (AAPPlayerController* Controller = GetOwningPlayer<AAPPlayerController>()) Controller->RequestMuster(EAPSoldierType::Scout);
}

void UAPPrototypeWidget::HandleAttackClicked()
{
    if (AAPPlayerController* Controller = GetOwningPlayer<AAPPlayerController>()) Controller->RequestAttack();
}
