#include "Greybox/APGreyboxWorldBuilder.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyLightComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/SkyLight.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/TextRenderActor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "APPlayerController.h"
#include "Simulation/APSimulationSubsystem.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
constexpr float ProvinceRadiusScale = 5.2f;

FString SoldierTypeName(EAPSoldierType Type)
{
    switch (Type)
    {
    case EAPSoldierType::Ranged: return TEXT("RANGED");
    case EAPSoldierType::Scout: return TEXT("SCOUT");
    default: return TEXT("SPEAR");
    }
}
}

AAPGreyboxWorldBuilder::AAPGreyboxWorldBuilder()
{
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeAsset(TEXT("/Engine/BasicShapes/Cone.Cone"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
    CylinderMesh = CylinderAsset.Object;
    CubeMesh = CubeAsset.Object;
    ConeMesh = ConeAsset.Object;
    SphereMesh = SphereAsset.Object;
    BaseMaterial = MaterialAsset.Object;
}

void AAPGreyboxWorldBuilder::BeginPlay()
{
    Super::BeginPlay();
    SpawnStaticWorld();
    UpdateCompanyVisuals(0.0f);
}

void AAPGreyboxWorldBuilder::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    UpdateCompanyVisuals(DeltaSeconds);
    CompanyRefreshAccumulator += DeltaSeconds;
    if (CompanyRefreshAccumulator >= 0.5f)
    {
        CompanyRefreshAccumulator = 0.0f;
        UpdateProvinceVisuals();
    }
}

FVector AAPGreyboxWorldBuilder::ProvinceLocation(int32 ProvinceId)
{
    static const FVector Locations[] = {
        FVector(-1600.0f, -850.0f, 30.0f),
        FVector(0.0f, -950.0f, 30.0f),
        FVector(1600.0f, -700.0f, 30.0f),
        FVector(-1450.0f, 850.0f, 30.0f),
        FVector(100.0f, 850.0f, 30.0f),
        FVector(1650.0f, 900.0f, 30.0f)
    };
    return Locations[FMath::Clamp(ProvinceId, 0, 5)];
}

AActor* AAPGreyboxWorldBuilder::SpawnShape(UStaticMesh* Mesh, const FVector& Location,
    const FVector& Scale, const FLinearColor& Color, const FString& Name, float Yaw)
{
    if (!Mesh)
    {
        return nullptr;
    }

    AStaticMeshActor* Actor = GetWorld()->SpawnActor<AStaticMeshActor>(Location, FRotator(0.0f, Yaw, 0.0f));
    Actor->SetActorLabel(Name);
    UStaticMeshComponent* Component = Actor->GetStaticMeshComponent();
    Component->SetMobility(EComponentMobility::Movable);
    Actor->SetActorScale3D(Scale);
    Component->SetStaticMesh(Mesh);
    Component->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Component->SetCollisionResponseToAllChannels(ECR_Ignore);
    Component->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    if (BaseMaterial)
    {
        UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(BaseMaterial, Actor);
        Material->SetVectorParameterValue(TEXT("Color"), Color);
        Component->SetMaterial(0, Material);
    }
    return Actor;
}

AActor* AAPGreyboxWorldBuilder::SpawnLabel(const FString& Text, const FVector& Location, const FColor& Color)
{
    ATextRenderActor* Label = GetWorld()->SpawnActor<ATextRenderActor>(Location, FRotator(0.0f, 0.0f, 0.0f));
    UTextRenderComponent* TextComponent = Label->GetTextRender();
    TextComponent->SetText(FText::FromString(Text));
    TextComponent->SetTextRenderColor(Color);
    TextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
    TextComponent->SetWorldSize(80.0f);
    return Label;
}

void AAPGreyboxWorldBuilder::SpawnStaticWorld()
{
    const UAPSimulationSubsystem* Simulation = GetWorld()->GetSubsystem<UAPSimulationSubsystem>();
    if (!Simulation)
    {
        return;
    }

    SpawnTerrain();
    SpawnAtmosphere();
    const TArray<FAPProvinceState> Provinces = Simulation->GetProvinces();
    for (const FAPProvinceState& Province : Provinces)
    {
        SpawnProvince(Province);
        for (int32 AdjacentId : Province.AdjacentProvinceIds)
        {
            if (AdjacentId > Province.ProvinceId)
            {
                SpawnRoute(Province.ProvinceId, AdjacentId);
            }
        }
    }
    SpawnSettlement();
    SpawnAIAndNeutralLandmarks();
}

void AAPGreyboxWorldBuilder::SpawnTerrain()
{
    // Broad overlapping low-poly shelves hide the editor checkerboard and create a warm ashland basin.
    SpawnShape(CubeMesh, FVector(0, 0, 12), FVector(48, 35, 0.22f),
        FLinearColor(0.12f, 0.105f, 0.08f), TEXT("AshfallTerrainBase"));
    static const FVector TileLocations[] = {
        FVector(-2200,-1200,25), FVector(-650,-1300,20), FVector(1050,-1150,28), FVector(2250,-700,38),
        FVector(-2100,750,35), FVector(-500,700,24), FVector(1050,850,32), FVector(2350,1100,44),
        FVector(-600,1900,48), FVector(1050,1950,55)
    };
    static const FLinearColor TileColors[] = {
        FLinearColor(.21f,.23f,.14f), FLinearColor(.28f,.23f,.14f), FLinearColor(.19f,.22f,.13f),
        FLinearColor(.27f,.17f,.12f), FLinearColor(.18f,.20f,.12f), FLinearColor(.25f,.22f,.15f),
        FLinearColor(.16f,.19f,.12f), FLinearColor(.30f,.19f,.13f), FLinearColor(.20f,.18f,.13f),
        FLinearColor(.24f,.20f,.14f)
    };
    for (int32 Index = 0; Index < UE_ARRAY_COUNT(TileLocations); ++Index)
    {
        SpawnShape(SphereMesh, TileLocations[Index], FVector(14.0f, 11.0f, 0.30f + (Index % 3) * .08f),
            TileColors[Index], FString::Printf(TEXT("TerrainRegion_%02d"), Index), Index * 17.0f);
    }
    // Deterministic rock/brush silhouettes give scale without an actor-heavy foliage system.
    for (int32 Index = 0; Index < 24; ++Index)
    {
        const float X = -2850.0f + (Index % 8) * 820.0f;
        const float Y = -2050.0f + (Index / 8) * 1900.0f + (Index % 2) * 240.0f;
        SpawnShape(ConeMesh, FVector(X, Y, 45.0f), FVector(.32f,.32f,.75f + (Index % 3) * .2f),
            FLinearColor(.12f,.15f,.09f), TEXT("AshPine"), Index * 29.0f);
    }
}

void AAPGreyboxWorldBuilder::SpawnAtmosphere()
{
    ADirectionalLight* Sun = GetWorld()->SpawnActor<ADirectionalLight>(FVector::ZeroVector, FRotator(-48,-32,0));
    Sun->GetLightComponent()->SetIntensity(5.2f);
    Sun->GetLightComponent()->SetLightColor(FLinearColor(1.0f,.72f,.48f));
    ASkyLight* Sky = GetWorld()->SpawnActor<ASkyLight>();
    Sky->GetLightComponent()->SetIntensity(0.8f);
    AExponentialHeightFog* Fog = GetWorld()->SpawnActor<AExponentialHeightFog>();
    Fog->GetComponent()->SetFogDensity(0.00065f);
    Fog->GetComponent()->SetFogInscatteringColor(FLinearColor(.42f,.31f,.22f));
}

void AAPGreyboxWorldBuilder::SpawnProvince(const FAPProvinceState& Province)
{
    const FLinearColor Color = Province.OwnerId == 0
        ? FLinearColor(0.10f, 0.34f, 0.48f)
        : (Province.OwnerId == 1 ? FLinearColor(0.48f, 0.13f, 0.09f) : FLinearColor(0.34f, 0.29f, 0.20f));
    const FString OwnerLabel = Province.OwnerId == 0 ? TEXT("PLAYER") : (Province.OwnerId == 1 ? TEXT("AI") : TEXT("NEUTRAL"));
    const FVector Location = ProvinceLocation(Province.ProvinceId);
    AActor* ProvinceShape = SpawnShape(CylinderMesh, Location + FVector(0,0,30),
        FVector(ProvinceRadiusScale, ProvinceRadiusScale * .82f, 0.10f), Color,
        FString::Printf(TEXT("Province_%d"), Province.ProvinceId));
    if (ProvinceShape) ProvinceShape->Tags.Add(FName(*FString::Printf(TEXT("ProvinceId:%d"), Province.ProvinceId)));
    ProvinceShapes.Add(Province.ProvinceId, ProvinceShape);
    ProvinceLabels.Add(Province.ProvinceId, SpawnLabel(
        FString::Printf(TEXT("PROVINCE %d - %s"), Province.ProvinceId + 1, *OwnerLabel),
        Location + FVector(0.0f, 0.0f, 175.0f), Province.OwnerId == 1 ? FColor(255,110,90) : FColor::White));
    // Ownership banner: restrained color accent rather than flooding the terrain.
    SpawnShape(CubeMesh, Location + FVector(-260,0,120), FVector(.08f,.08f,1.5f),
        FLinearColor(.10f,.08f,.05f), TEXT("BannerPole"));
    SpawnShape(CubeMesh, Location + FVector(-225,0,210), FVector(.55f,.06f,.38f), Color, TEXT("OwnershipBanner"));
}

void AAPGreyboxWorldBuilder::SpawnRoute(int32 FromProvinceId, int32 ToProvinceId)
{
    const FVector Start = ProvinceLocation(FromProvinceId);
    const FVector End = ProvinceLocation(ToProvinceId);
    const FVector Delta = End - Start;
    const FVector Midpoint = (Start + End) * 0.5f + FVector(0.0f, 0.0f, 58.0f);
    const float LengthScale = Delta.Size2D() / 100.0f;
    const float Yaw = FMath::RadiansToDegrees(FMath::Atan2(Delta.Y, Delta.X));
    SpawnShape(CubeMesh, Midpoint, FVector(LengthScale, 0.34f, 0.045f),
        FLinearColor(0.30f, 0.20f, 0.11f), TEXT("ProvinceRoute"), Yaw);
    SpawnShape(CubeMesh, Midpoint + FVector(0,0,7), FVector(LengthScale, 0.20f, 0.028f),
        FLinearColor(0.46f, 0.34f, 0.20f), TEXT("ProvinceRoadSurface"), Yaw);
    RouteVisualCount += 2;
}

void AAPGreyboxWorldBuilder::SpawnSettlement()
{
    const FVector TownCenter = ProvinceLocation(0) + FVector(0.0f, 0.0f, 75.0f);
    SpawnShape(CylinderMesh, TownCenter, FVector(2.6f, 2.2f, 0.10f),
        FLinearColor(0.24f, 0.19f, 0.12f), TEXT("PlayerTownSquare"));
    SpawnBuilding(TownCenter + FVector(0,0,55), 45.0f, TEXT("PlayerKeep"),
        FLinearColor(.38f,.34f,.27f), 1.55f);
    SpawnLabel(TEXT("ASHFALL HOLD"), TownCenter + FVector(0.0f, 0.0f, 310.0f), FColor(255,210,100));
    bHasPlayerSettlementVisual = true;

    for (int32 BuildingIndex = 0; BuildingIndex < 8; ++BuildingIndex)
    {
        const float Angle = 2.0f * PI * static_cast<float>(BuildingIndex) / 8.0f;
        const FVector Offset(FMath::Cos(Angle) * 345.0f, FMath::Sin(Angle) * 290.0f, 45.0f);
        SpawnBuilding(TownCenter + Offset, FMath::RadiansToDegrees(Angle) + 90.0f,
            FString::Printf(TEXT("BuildingSlot_%d"), BuildingIndex + 1),
            BuildingIndex % 2 ? FLinearColor(.42f,.29f,.17f) : FLinearColor(.34f,.31f,.23f),
            .72f + (BuildingIndex % 3) * .10f);
        ++PlayerBuildingVisualCount;
    }
    // Four short lanes organize the settlement around a central square.
    for (int32 Road = 0; Road < 4; ++Road)
    {
        SpawnShape(CubeMesh, TownCenter + FVector((Road < 2 ? 1 : 0) * (Road % 2 ? 180 : -180),
            (Road >= 2 ? 1 : 0) * (Road % 2 ? 160 : -160), 15),
            Road < 2 ? FVector(1.7f,.16f,.025f) : FVector(.16f,1.5f,.025f),
            FLinearColor(.39f,.28f,.16f), TEXT("TownLane"));
    }
}

void AAPGreyboxWorldBuilder::SpawnBuilding(const FVector& Location, float Yaw, const FString& Name,
    const FLinearColor& WallColor, float Scale)
{
    SpawnShape(CubeMesh, Location, FVector(.62f,.52f,.62f) * Scale, WallColor, Name + TEXT("_Walls"), Yaw);
    SpawnShape(ConeMesh, Location + FVector(0,0,86.0f * Scale), FVector(.88f,.78f,.42f) * Scale,
        FLinearColor(.22f,.09f,.055f), Name + TEXT("_Roof"), Yaw + 45.0f);
    SpawnShape(CubeMesh, Location + FVector(42.0f * Scale,0,-12.0f * Scale), FVector(.05f,.18f,.28f) * Scale,
        FLinearColor(.12f,.07f,.035f), Name + TEXT("_Door"), Yaw);
}

void AAPGreyboxWorldBuilder::SpawnAIAndNeutralLandmarks()
{
    const FVector AI = ProvinceLocation(5) + FVector(0,0,95);
    SpawnShape(CubeMesh, AI, FVector(1.25f,1.05f,.72f), FLinearColor(.31f,.27f,.23f), TEXT("AIOutpostKeep"));
    for (int32 Corner = 0; Corner < 4; ++Corner)
    {
        SpawnShape(CylinderMesh, AI + FVector(Corner < 2 ? -135 : 135, Corner % 2 ? -115 : 115, 35),
            FVector(.35f,.35f,1.15f), FLinearColor(.38f,.30f,.24f), TEXT("AIOutpostTower"));
    }
    SpawnLabel(TEXT("CINDER WATCH\nAI OUTPOST"), AI + FVector(0,0,220), FColor(255,90,70));
    bHasAIOutpostVisual = true;
    for (int32 ProvinceId = 1; ProvinceId < 5; ++ProvinceId)
    {
        const FVector P = ProvinceLocation(ProvinceId) + FVector(120,-90,95);
        if (ProvinceId % 2 == 0)
        {
            SpawnShape(CylinderMesh, P, FVector(.42f,.42f,.9f), FLinearColor(.30f,.27f,.21f), TEXT("NeutralWatchtower"));
            SpawnShape(ConeMesh, P + FVector(0,0,100), FVector(.58f,.58f,.35f), FLinearColor(.19f,.12f,.07f), TEXT("NeutralRoof"));
        }
        else
        {
            SpawnBuilding(P, ProvinceId * 31.0f, TEXT("NeutralHamlet"), FLinearColor(.37f,.30f,.21f), .55f);
        }
    }
}

void AAPGreyboxWorldBuilder::UpdateProvinceVisuals()
{
    const UAPSimulationSubsystem* Simulation = GetWorld()->GetSubsystem<UAPSimulationSubsystem>();
    if (!Simulation)
    {
        return;
    }
    for (const FAPProvinceState& Province : Simulation->GetProvinces())
    {
        const FLinearColor Color = Province.OwnerId == 0
            ? FLinearColor(0.08f, 0.42f, 0.65f)
            : (Province.OwnerId == 1 ? FLinearColor(0.65f, 0.12f, 0.08f) : FLinearColor(0.30f, 0.28f, 0.24f));
        if (AStaticMeshActor* Shape = Cast<AStaticMeshActor>(ProvinceShapes.FindRef(Province.ProvinceId)))
        {
            if (UMaterialInstanceDynamic* Material = Cast<UMaterialInstanceDynamic>(
                Shape->GetStaticMeshComponent()->GetMaterial(0)))
            {
                Material->SetVectorParameterValue(TEXT("Color"), Color);
            }
            const AAPPlayerController* Controller = Cast<AAPPlayerController>(GetWorld()->GetFirstPlayerController());
            Shape->SetActorScale3D(Controller && Controller->GetSelectedProvinceId() == Province.ProvinceId
                ? FVector(ProvinceRadiusScale * 1.10f, ProvinceRadiusScale * .90f, 0.16f)
                : FVector(ProvinceRadiusScale, ProvinceRadiusScale * .82f, 0.10f));
        }
        if (ATextRenderActor* Label = Cast<ATextRenderActor>(ProvinceLabels.FindRef(Province.ProvinceId)))
        {
            const FString OwnerLabel = Province.OwnerId == 0 ? TEXT("PLAYER")
                : (Province.OwnerId == 1 ? TEXT("AI") : TEXT("NEUTRAL"));
            Label->GetTextRender()->SetText(FText::FromString(FString::Printf(
                TEXT("PROVINCE %d - %s"), Province.ProvinceId + 1, *OwnerLabel)));
            Label->GetTextRender()->SetTextRenderColor(
                Province.OwnerId == 0 ? FColor::Cyan : (Province.OwnerId == 1 ? FColor::Red : FColor::White));
        }
    }
}

void AAPGreyboxWorldBuilder::SpawnUnitFormation(int32 CompanyId, EAPSoldierType Type,
    const FVector& Location, const FLinearColor& Color)
{
    TArray<TObjectPtr<AActor>>& Parts = CompanyFormationParts.FindOrAdd(CompanyId);
    const int32 FigureCount = Type == EAPSoldierType::Scout ? 3 : 5;
    for (int32 Index = 0; Index < FigureCount; ++Index)
    {
        const FVector Local((Index % 3 - 1) * 70.0f, (Index / 3) * 80.0f - 35.0f, 25.0f);
        AActor* Soldier = SpawnShape(CylinderMesh, Location + Local, FVector(.16f,.16f,.48f), Color,
            FString::Printf(TEXT("Company_%d_Soldier"), CompanyId));
        if (Soldier) Soldier->Tags.Add(FName(*FString::Printf(TEXT("CompanyId:%d"), CompanyId)));
        Parts.Add(Soldier);
        if (Type == EAPSoldierType::Spear)
        {
            Parts.Add(SpawnShape(CubeMesh, Location + Local + FVector(22,0,45), FVector(.025f,.025f,.78f),
                FLinearColor(.40f,.29f,.16f), TEXT("SpearPole"), -8.0f));
        }
        else if (Type == EAPSoldierType::Ranged)
        {
            Parts.Add(SpawnShape(ConeMesh, Location + Local + FVector(18,0,35), FVector(.11f,.05f,.38f),
                FLinearColor(.55f,.34f,.14f), TEXT("BowMarker"), 90.0f));
        }
    }
    AActor* Ring = SpawnShape(CylinderMesh, Location + FVector(0,0,-2), FVector(1.2f,1.2f,.025f),
        FLinearColor(.12f,.72f,1.0f), TEXT("CompanySelectionRing"));
    Ring->SetActorHiddenInGame(true);
    CompanySelectionRings.Add(CompanyId, Ring);
}

bool AAPGreyboxWorldBuilder::IsCompanyVisualSelected(int32 CompanyId) const
{
    const AActor* Ring = CompanySelectionRings.FindRef(CompanyId);
    return Ring && !Ring->IsHidden();
}

FVector AAPGreyboxWorldBuilder::GetCompanyVisualLocation(int32 CompanyId) const
{
    const AActor* Shape = CompanyShapes.FindRef(CompanyId);
    return Shape ? Shape->GetActorLocation() : FVector::ZeroVector;
}

void AAPGreyboxWorldBuilder::UpdateCompanyVisuals(float DeltaSeconds)
{
    const UAPSimulationSubsystem* Simulation = GetWorld()->GetSubsystem<UAPSimulationSubsystem>();
    if (!Simulation)
    {
        return;
    }

    const AAPPlayerController* Controller = Cast<AAPPlayerController>(GetWorld()->GetFirstPlayerController());
    const int32 SelectedCompanyId = Controller ? Controller->GetSelectedCompanyId() : INDEX_NONE;
    TSet<int32> ActiveCompanyIds;
    for (const FAPArmyState& Company : Simulation->GetCompanies())
    {
        if (Company.HouseholdIds.IsEmpty())
        {
            continue;
        }
        ActiveCompanyIds.Add(Company.CompanyId);
        const FVector Offset(Company.OwnerId == 0 ? -180.0f : 180.0f, 120.0f, 150.0f);
        FVector DesiredLocation = ProvinceLocation(Company.ProvinceId) + Offset;
        if (Company.DestinationProvinceId != INDEX_NONE && Company.TravelTicksRemaining > 0)
        {
            const float TotalTicks = Company.SoldierType == EAPSoldierType::Scout ? 1.0f : 2.0f;
            const float Progress = FMath::Clamp(1.0f - Company.TravelTicksRemaining / TotalTicks, 0.0f, 1.0f);
            DesiredLocation = FMath::Lerp(ProvinceLocation(Company.ProvinceId),
                ProvinceLocation(Company.DestinationProvinceId), Progress) + Offset;
        }
        const FLinearColor Color = Company.OwnerId == 0
            ? FLinearColor(0.15f, 0.65f, 1.0f) : FLinearColor(0.95f, 0.15f, 0.08f);
        AActor* Shape = CompanyShapes.FindRef(Company.CompanyId);
        if (!IsValid(Shape))
        {
            Shape = SpawnShape(CylinderMesh, DesiredLocation, FVector(.82f,.82f,.06f),
                FLinearColor(Color.R * .4f, Color.G * .4f, Color.B * .4f),
                FString::Printf(TEXT("Company_%d"), Company.CompanyId));
            if (Shape) Shape->Tags.Add(FName(*FString::Printf(TEXT("CompanyId:%d"), Company.CompanyId)));
            CompanyShapes.Add(Company.CompanyId, Shape);
            SpawnUnitFormation(Company.CompanyId, Company.SoldierType, DesiredLocation, Color);
        }
        AActor* Label = CompanyLabels.FindRef(Company.CompanyId);
        if (!IsValid(Label))
        {
            Label = SpawnLabel(TEXT("COMPANY"), DesiredLocation + FVector(0.0f, 0.0f, 190.0f),
                Company.OwnerId == 0 ? FColor::Cyan : FColor::Red);
            CompanyLabels.Add(Company.CompanyId, Label);
        }

        const FVector VisualLocation = DeltaSeconds > 0.0f
            ? FMath::VInterpConstantTo(Shape->GetActorLocation(), DesiredLocation, DeltaSeconds, 650.0f)
            : DesiredLocation;
        Shape->SetActorLocation(VisualLocation);
        Shape->SetActorScale3D(Company.CompanyId == SelectedCompanyId
            ? FVector(1.05f, 1.05f, .07f) : FVector(.82f,.82f,.06f));
        Label->SetActorLocation(VisualLocation + FVector(0.0f, 0.0f, 190.0f));
        if (AActor* Ring = CompanySelectionRings.FindRef(Company.CompanyId))
        {
            Ring->SetActorLocation(VisualLocation + FVector(0,0,-2));
            Ring->SetActorHiddenInGame(Company.CompanyId != SelectedCompanyId);
        }
        if (TArray<TObjectPtr<AActor>>* Parts = CompanyFormationParts.Find(Company.CompanyId))
        {
            for (int32 PartIndex = 0; PartIndex < Parts->Num(); ++PartIndex)
            {
                AActor* Part = (*Parts)[PartIndex];
                if (!Part) continue;
                const int32 UnitIndex = Company.SoldierType == EAPSoldierType::Scout ? PartIndex : PartIndex / 2;
                const FVector Local((UnitIndex % 3 - 1) * 70.0f, (UnitIndex / 3) * 80.0f - 35.0f,
                    PartIndex % 2 == 1 && Company.SoldierType != EAPSoldierType::Scout ? 70.0f : 25.0f);
                Part->SetActorLocation(VisualLocation + Local);
            }
        }
        if (Company.DestinationProvinceId != INDEX_NONE && Company.TravelTicksRemaining > 0 && Company.OwnerId == 0)
        {
            AActor* Marker = DestinationMarkers.FindRef(Company.CompanyId);
            if (!Marker)
            {
                Marker = SpawnShape(CylinderMesh, ProvinceLocation(Company.DestinationProvinceId) + FVector(0,0,72),
                    FVector(.75f,.75f,.035f), FLinearColor(1.0f,.65f,.12f), TEXT("DestinationMarker"));
                DestinationMarkers.Add(Company.CompanyId, Marker);
            }
            Marker->SetActorHiddenInGame(false);
            Marker->SetActorLocation(ProvinceLocation(Company.DestinationProvinceId) + FVector(0,0,72));
        }
        else if (AActor* Marker = DestinationMarkers.FindRef(Company.CompanyId)) Marker->SetActorHiddenInGame(true);
        if (ATextRenderActor* TextLabel = Cast<ATextRenderActor>(Label))
        {
            TextLabel->GetTextRender()->SetText(FText::FromString(FString::Printf(TEXT("%s%s %s x%d"),
                Company.CompanyId == SelectedCompanyId ? TEXT("> ") : TEXT(""),
                Company.OwnerId == 0 ? TEXT("PLAYER") : TEXT("AI"),
                *SoldierTypeName(Company.SoldierType), Company.HouseholdIds.Num())));
        }
    }

    TArray<int32> KnownCompanyIds;
    CompanyShapes.GetKeys(KnownCompanyIds);
    for (int32 CompanyId : KnownCompanyIds)
    {
        if (!ActiveCompanyIds.Contains(CompanyId))
        {
            if (AActor* Shape = CompanyShapes.FindRef(CompanyId)) { Shape->Destroy(); }
            if (AActor* Label = CompanyLabels.FindRef(CompanyId)) { Label->Destroy(); }
            if (AActor* Ring = CompanySelectionRings.FindRef(CompanyId)) { Ring->Destroy(); }
            if (AActor* Marker = DestinationMarkers.FindRef(CompanyId)) { Marker->Destroy(); }
            if (TArray<TObjectPtr<AActor>>* Parts = CompanyFormationParts.Find(CompanyId))
                for (AActor* Part : *Parts) if (Part) Part->Destroy();
            CompanyShapes.Remove(CompanyId);
            CompanyLabels.Remove(CompanyId);
            CompanySelectionRings.Remove(CompanyId);
            DestinationMarkers.Remove(CompanyId);
            CompanyFormationParts.Remove(CompanyId);
        }
    }
}
