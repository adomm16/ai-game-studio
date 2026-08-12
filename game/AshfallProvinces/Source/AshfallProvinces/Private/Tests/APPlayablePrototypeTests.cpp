#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "APGameMode.h"
#include "APPlayerController.h"
#include "APStrategyCameraPawn.h"
#include "Greybox/APGreyboxWorldBuilder.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Simulation/APSimulationSubsystem.h"
#include "UI/APStrategyHUD.h"
#include "UI/APPrototypeWidget.h"
#include "Components/Button.h"
#include "GameFramework/HUD.h"
#include "Misc/Paths.h"
#include "UnrealClient.h"
#include "Tests/AutomationCommon.h"

#if WITH_EDITOR
#include "Editor.h"
#include "Tests/AutomationEditorCommon.h"
#endif

namespace
{
int32 CountWorking(const UAPSimulationSubsystem* Simulation)
{
    int32 Count = 0;
    for (const FAPHouseholdState& Household : Simulation->GetHouseholds())
    {
        Count += Household.bAlive && !Household.bMustered && Household.Job != EAPHouseholdJob::Idle;
    }
    return Count;
}

int32 CountMustered(const UAPSimulationSubsystem* Simulation)
{
    int32 Count = 0;
    for (const FAPHouseholdState& Household : Simulation->GetHouseholds())
    {
        Count += Household.bAlive && Household.bMustered;
    }
    return Count;
}

int32 CountPlayerCompanies(const UAPSimulationSubsystem* Simulation)
{
    int32 Count = 0;
    for (const FAPArmyState& Company : Simulation->GetCompanies())
    {
        Count += Company.OwnerId == 0 && !Company.HouseholdIds.IsEmpty();
    }
    return Count;
}

int32 CountAlive(const UAPSimulationSubsystem* Simulation)
{
    int32 Count = 0;
    for (const FAPHouseholdState& Household : Simulation->GetHouseholds()) Count += Household.bAlive;
    return Count;
}

UAPSimulationSubsystem* MakeSimulation()
{
    UAPSimulationSubsystem* Simulation = NewObject<UAPSimulationSubsystem>();
    Simulation->StartNewGame(42);
    return Simulation;
}

bool MusterFirstCompany(UAPSimulationSubsystem* Simulation)
{
    return Simulation->MusterCompany(EAPSoldierType::Spear, {0, 8, 16});
}

void MoveCompanyTo(UAPSimulationSubsystem* Simulation, int32 Destination)
{
    Simulation->OrderMove(0, Destination);
    Simulation->Tick(UAPSimulationSubsystem::FixedTickSeconds);
    Simulation->Tick(UAPSimulationSubsystem::FixedTickSeconds);
}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAPPrototypeRuntimeClassesTest,
    "Ashfall.PlayablePrototype.RuntimeClasses",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAPPrototypeRuntimeClassesTest::RunTest(const FString& Parameters)
{
    const AAPGameMode* GameMode = GetDefault<AAPGameMode>();
    TestTrue(TEXT("GameMode uses APPlayerController"), GameMode->PlayerControllerClass.Get() == AAPPlayerController::StaticClass());
    TestTrue(TEXT("GameMode uses strategy camera"), GameMode->DefaultPawnClass.Get() == AAPStrategyCameraPawn::StaticClass());
    TestTrue(TEXT("GameMode uses interactive HUD"), GameMode->HUDClass.Get() == AAPStrategyHUD::StaticClass());
    TestNotNull(TEXT("Greybox world builder class is available"), AAPGreyboxWorldBuilder::StaticClass());
    UWorld* StartupWorld = GWorld;
    TestNotNull(TEXT("Startup world is loaded"), StartupWorld);
    if (StartupWorld) TestTrue(TEXT("L_FirstTest is the startup map"), StartupWorld->GetMapName().Contains(TEXT("L_FirstTest")));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAPPrototypeMusterTest,
    "Ashfall.PlayablePrototype.MusterEconomyAndLimit",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAPPrototypeMusterTest::RunTest(const FString& Parameters)
{
    UAPSimulationSubsystem* Simulation = MakeSimulation();
    TestEqual(TEXT("Initial households"), Simulation->GetHouseholds().Num(), 24);
    const int32 WorkingBefore = CountWorking(Simulation);
    const int32 MusteredBefore = CountMustered(Simulation);
    const int32 CompaniesBefore = CountPlayerCompanies(Simulation);
    TestTrue(TEXT("Spear muster succeeds"), MusterFirstCompany(Simulation));
    TestEqual(TEXT("Working decreases by three"), CountWorking(Simulation), WorkingBefore - 3);
    TestEqual(TEXT("Mustered increases by three"), CountMustered(Simulation), MusteredBefore + 3);
    TestEqual(TEXT("Player companies increases by one"), CountPlayerCompanies(Simulation), CompaniesBefore + 1);

    Simulation->MusterCompany(EAPSoldierType::Ranged, {1, 9, 17});
    Simulation->MusterCompany(EAPSoldierType::Scout, {2, 10, 18});
    Simulation->MusterCompany(EAPSoldierType::Spear, {3, 11, 19});
    Simulation->MusterCompany(EAPSoldierType::Ranged, {4, 12, 20});
    TestTrue(TEXT("Company hard limit is never exceeded"),
        CountPlayerCompanies(Simulation) <= UAPSimulationSubsystem::MaxCompanies);
    const TArray<FAPArmyState> Companies = Simulation->GetCompanies();
    TestTrue(TEXT("Ranged company path works"), Companies.ContainsByPredicate(
        [](const FAPArmyState& Company) { return Company.OwnerId == 0 && Company.SoldierType == EAPSoldierType::Ranged; }));
    TestTrue(TEXT("Scout company path works"), Companies.ContainsByPredicate(
        [](const FAPArmyState& Company) { return Company.OwnerId == 0 && Company.SoldierType == EAPSoldierType::Scout; }));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAPPrototypeMovementCaptureTest,
    "Ashfall.PlayablePrototype.MovementAndCapture",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAPPrototypeMovementCaptureTest::RunTest(const FString& Parameters)
{
    UAPSimulationSubsystem* Simulation = MakeSimulation();
    TestTrue(TEXT("Company created"), MusterFirstCompany(Simulation));
    TestFalse(TEXT("Non-adjacent order rejected"), Simulation->OrderMove(0, 2));
    TestTrue(TEXT("Adjacent order accepted"), Simulation->OrderMove(0, 1));
    Simulation->Tick(UAPSimulationSubsystem::FixedTickSeconds);
    Simulation->Tick(UAPSimulationSubsystem::FixedTickSeconds);
    TestTrue(TEXT("Neutral province captured"), Simulation->CaptureProvince(0));
    const TArray<FAPProvinceState> Provinces = Simulation->GetProvinces();
    const FAPProvinceState* Province = Provinces.FindByPredicate(
        [](const FAPProvinceState& Entry) { return Entry.ProvinceId == 1; });
    TestNotNull(TEXT("Province exists"), Province);
    if (Province) TestEqual(TEXT("Province owner becomes Player"), Province->OwnerId, 0);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAPPrototypeBattleCasualtyTest,
    "Ashfall.PlayablePrototype.BattleAndPermanentCasualty",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAPPrototypeBattleCasualtyTest::RunTest(const FString& Parameters)
{
    UAPSimulationSubsystem* A = MakeSimulation();
    UAPSimulationSubsystem* B = MakeSimulation();
    MusterFirstCompany(A);
    MusterFirstCompany(B);
    for (int32 Province = 1; Province < UAPSimulationSubsystem::ProvinceCount; ++Province)
    {
        MoveCompanyTo(A, Province);
        MoveCompanyTo(B, Province);
    }
    TestTrue(TEXT("Battle A resolves"), A->ResolveBattle(0, 100));
    TestTrue(TEXT("Battle B resolves"), B->ResolveBattle(0, 100));
    TestEqual(TEXT("Deterministic battle state"), A->GetCompanies()[0].HouseholdIds.Num(),
        B->GetCompanies()[0].HouseholdIds.Num());
    TestEqual(TEXT("Deterministic defender state"), A->GetCompanies().Last().HouseholdIds.Num(),
        B->GetCompanies().Last().HouseholdIds.Num());

    UAPSimulationSubsystem* CasualtySimulation = MakeSimulation();
    MusterFirstCompany(CasualtySimulation);
    const int32 AliveBefore = CountAlive(CasualtySimulation);
    TestEqual(TEXT("One casualty applied"), CasualtySimulation->ApplyCasualties(0, 1), 1);
    const int32 AliveAfter = CountAlive(CasualtySimulation);
    TestEqual(TEXT("Casualty permanently reduces alive households"), AliveAfter, AliveBefore - 1);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAPPrototypeSaveLoadTest,
    "Ashfall.PlayablePrototype.SaveLoadRoundTrip",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAPPrototypeSaveLoadTest::RunTest(const FString& Parameters)
{
    const FString Slot = TEXT("AshfallPrototypeAutomation");
    UGameplayStatics::DeleteGameInSlot(Slot, 0);
    UAPSimulationSubsystem* Source = MakeSimulation();
    MusterFirstCompany(Source);
    MoveCompanyTo(Source, 1);
    Source->CaptureProvince(0);
    Source->ApplyCasualties(0, 1);
    TestTrue(TEXT("Save succeeds"), Source->SaveSimulation(Slot));

    UAPSimulationSubsystem* Loaded = MakeSimulation();
    TestTrue(TEXT("Load succeeds"), Loaded->LoadSimulation(Slot));
    TestEqual(TEXT("Resources round-trip"), Loaded->GetResources().Food, Source->GetResources().Food);
    TestEqual(TEXT("Households round-trip"), Loaded->GetHouseholds().Num(), Source->GetHouseholds().Num());
    TestEqual(TEXT("Companies round-trip"), Loaded->GetCompanies().Num(), Source->GetCompanies().Num());
    TestEqual(TEXT("Province ownership round-trip"), Loaded->GetProvinces()[1].OwnerId, Source->GetProvinces()[1].OwnerId);
    TestEqual(TEXT("Company province round-trip"), Loaded->GetCompanies()[0].ProvinceId, Source->GetCompanies()[0].ProvinceId);
    TestEqual(TEXT("Simulation tick round-trip"), Loaded->GetSimulationTick(), Source->GetSimulationTick());
    UGameplayStatics::DeleteGameInSlot(Slot, 0);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAPPrototypeProvinceAndLayoutTest,
    "Ashfall.PlayablePrototype.ProvinceCountAndHUDLayout",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAPPrototypeProvinceAndLayoutTest::RunTest(const FString& Parameters)
{
    UAPSimulationSubsystem* Simulation = MakeSimulation();
    TestEqual(TEXT("Province count is six"), Simulation->GetProvinces().Num(), 6);
    const FVector2D Viewports[] = {FVector2D(1280, 720), FVector2D(1920, 1080), FVector2D(2560, 1440)};
    for (const FVector2D& Viewport : Viewports)
    {
        const FAPStrategyHUDLayout Layout = AAPStrategyHUD::CalculateLayout(Viewport.X, Viewport.Y);
        const FBox2D ViewBounds(FVector2D::ZeroVector, Viewport);
        TestTrue(TEXT("Muster panel minimum is visible"), ViewBounds.IsInside(Layout.MusterPanel.Min));
        TestTrue(TEXT("Muster panel maximum is visible"), ViewBounds.IsInside(Layout.MusterPanel.Max));
        TestEqual(TEXT("Muster panel width"), static_cast<double>(Layout.MusterPanel.GetSize().X), 500.0);
        TestTrue(TEXT("Spear hitbox is drawn inside panel"), Layout.MusterPanel.IsInside(Layout.SpearButton.Min)
            && Layout.MusterPanel.IsInside(Layout.SpearButton.Max));
        TestTrue(TEXT("Ranged hitbox is drawn inside panel"), Layout.MusterPanel.IsInside(Layout.RangedButton.Min)
            && Layout.MusterPanel.IsInside(Layout.RangedButton.Max));
        TestTrue(TEXT("Scout hitbox is drawn inside panel"), Layout.MusterPanel.IsInside(Layout.ScoutButton.Min)
            && Layout.MusterPanel.IsInside(Layout.ScoutButton.Max));
    }
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAPPrototypeWidgetRuntimeTest,
    "Ashfall.PlayablePrototype.WidgetRuntime",
    EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FAPPrototypeWidgetRuntimeTest::RunTest(const FString& Parameters)
{
    AAPPlayerController* Controller = GWorld
        ? Cast<AAPPlayerController>(UGameplayStatics::GetPlayerController(GWorld, 0)) : nullptr;
    TestNotNull(TEXT("Runtime APPlayerController exists"), Controller);
    if (!Controller) return false;

    UAPPrototypeWidget* Widget = Controller->GetPrototypeWidget();
    TestNotNull(TEXT("Prototype widget was created at runtime"), Widget);
    if (!Widget) return false;
    TestTrue(TEXT("Widget owning player is correct"), Widget->GetOwningPlayer() == Controller);
    TestTrue(TEXT("Widget is in viewport"), Widget->IsInViewport());
    TestEqual(TEXT("Widget visibility is Visible"), Widget->GetVisibility(), ESlateVisibility::Visible);
    TestNotNull(TEXT("Spear button exists"), Widget->GetSpearButton());
    TestNotNull(TEXT("Ranged button exists"), Widget->GetRangedButton());
    TestNotNull(TEXT("Scout button exists"), Widget->GetScoutButton());
    TestTrue(TEXT("Command callbacks are bound"), Widget->AreCommandCallbacksBound());

    UAPSimulationSubsystem* Simulation = GWorld->GetSubsystem<UAPSimulationSubsystem>();
    const int32 CompaniesBefore = Simulation ? CountPlayerCompanies(Simulation) : INDEX_NONE;
    TestNotNull(TEXT("Authoritative simulation exists"), Simulation);
    if (Simulation && Widget->GetSpearButton())
    {
        Widget->GetSpearButton()->OnClicked.Broadcast();
        TestEqual(TEXT("Spear click reaches controller muster command"),
            CountPlayerCompanies(Simulation), CompaniesBefore + 1);
    }
    return true;
}

#if WITH_EDITOR
DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FAPVerifyPIEPrototypeUICommand, FAutomationTestBase*, Test);

bool FAPVerifyPIEPrototypeUICommand::Update()
{
    UWorld* PIEWorld = nullptr;
    for (const FWorldContext& Context : GEngine->GetWorldContexts())
    {
        if (Context.WorldType == EWorldType::PIE)
        {
            PIEWorld = Context.World();
            break;
        }
    }
    Test->TestNotNull(TEXT("PIE world exists"), PIEWorld);
    if (!PIEWorld) return true;

    AAPPlayerController* Controller = Cast<AAPPlayerController>(UGameplayStatics::GetPlayerController(PIEWorld, 0));
    Test->TestNotNull(TEXT("PIE APPlayerController exists"), Controller);
    if (!Controller) return true;

    UAPPrototypeWidget* Widget = Controller->GetPrototypeWidget();
    Test->TestNotNull(TEXT("PIE prototype widget exists"), Widget);
    if (!Widget) return true;

    Test->TestTrue(TEXT("PIE widget IsInViewport"), Widget->IsInViewport());
    Test->TestEqual(TEXT("PIE widget is Visible"), Widget->GetVisibility(), ESlateVisibility::Visible);
    const FVector2D GeometrySize = Widget->GetCachedGeometry().GetLocalSize();
    Test->TestTrue(TEXT("PIE widget geometry is non-zero"), GeometrySize.X > 0.0f && GeometrySize.Y > 0.0f);
    UE_LOG(LogTemp, Display, TEXT("ASHFALL_PIE_WIDGET_GEOMETRY=%.0fx%.0f"), GeometrySize.X, GeometrySize.Y);
    Test->TestNotNull(TEXT("PIE Spear button exists"), Widget->GetSpearButton());
    Test->TestNotNull(TEXT("PIE Ranged button exists"), Widget->GetRangedButton());
    Test->TestNotNull(TEXT("PIE Scout button exists"), Widget->GetScoutButton());

    AAPStrategyHUD* StrategyHUD = Cast<AAPStrategyHUD>(Controller->GetHUD());
    Test->TestNotNull(TEXT("PIE strategy HUD exists"), StrategyHUD);
    if (StrategyHUD)
    {
        Test->TestFalse(TEXT("Old Canvas debug HUD is disabled in PIE"), StrategyHUD->bShowDebugPrototypeHUD);
    }

    UAPSimulationSubsystem* Simulation = PIEWorld->GetSubsystem<UAPSimulationSubsystem>();
    Test->TestNotNull(TEXT("PIE authoritative simulation exists"), Simulation);
    const int32 CompaniesBefore = Simulation ? CountPlayerCompanies(Simulation) : INDEX_NONE;
    if (Simulation && Widget->GetSpearButton())
    {
        Widget->GetSpearButton()->OnClicked.Broadcast();
        Test->TestEqual(TEXT("PIE Spear routing creates one company"),
            CountPlayerCompanies(Simulation), CompaniesBefore + 1);
    }

    AAPGreyboxWorldBuilder* VisualBuilder = nullptr;
    for (TActorIterator<AAPGreyboxWorldBuilder> It(PIEWorld); It; ++It) { VisualBuilder = *It; break; }
    Test->TestNotNull(TEXT("Visual world builder is active"), VisualBuilder);
    if (VisualBuilder && Simulation)
    {
        VisualBuilder->Tick(0.0f);
        Test->TestEqual(TEXT("Six province visuals exist"), VisualBuilder->GetProvinceVisualCount(), 6);
        Test->TestTrue(TEXT("Route visuals exist"), VisualBuilder->GetRouteVisualCount() >= 10);
        Test->TestTrue(TEXT("Player settlement visual exists"), VisualBuilder->HasPlayerSettlementVisual());
        Test->TestTrue(TEXT("Eight player building visuals exist"), VisualBuilder->GetPlayerBuildingVisualCount() >= 8);
        Test->TestTrue(TEXT("AI outpost visual exists"), VisualBuilder->HasAIOutpostVisual());
        Test->TestTrue(TEXT("Company visual spawns after muster"), VisualBuilder->GetCompanyVisualCount() >= 1);
        Test->TestTrue(TEXT("Mustered company selection ring is active"),
            VisualBuilder->IsCompanyVisualSelected(Controller->GetSelectedCompanyId()));
        const int32 CompanyId = Controller->GetSelectedCompanyId();
        const FVector BeforeMove = VisualBuilder->GetCompanyVisualLocation(CompanyId);
        Test->TestTrue(TEXT("Visual movement order accepted"), Simulation->OrderMove(CompanyId, 1));
        Simulation->Tick(UAPSimulationSubsystem::FixedTickSeconds);
        Simulation->Tick(UAPSimulationSubsystem::FixedTickSeconds);
        VisualBuilder->Tick(1.0f);
        const FVector AfterMove = VisualBuilder->GetCompanyVisualLocation(CompanyId);
        Test->TestTrue(TEXT("Company visual reaches movement destination"),
            FVector::Dist2D(BeforeMove, AfterMove) > 500.0f);
    }

    const FString ScreenshotPath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Screenshots/WindowsEditor/AshfallVisualPrototype_v0_2.png"));
    GEngine->Exec(PIEWorld, TEXT("DisableAllScreenMessages"));
    FScreenshotRequest::RequestScreenshot(ScreenshotPath, true, false);
    UE_LOG(LogTemp, Display, TEXT("ASHFALL_PIE_SCREENSHOT_REQUESTED=%s"), *ScreenshotPath);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAPPrototypePIEWidgetRuntimeTest,
    "Ashfall.PlayablePrototype.PIEWidgetRuntime",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAPPrototypePIEWidgetRuntimeTest::RunTest(const FString& Parameters)
{
    FAutomationEditorCommonUtils::LoadMap(TEXT("/Game/L_FirstTest"));
    ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(false));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FAPVerifyPIEPrototypeUICommand(this));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand());
    return true;
}
#endif

#endif
