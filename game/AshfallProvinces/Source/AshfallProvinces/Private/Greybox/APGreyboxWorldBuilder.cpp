#include "Greybox/APGreyboxWorldBuilder.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/TextRenderActor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "APPlayerController.h"
#include "Simulation/APSimulationSubsystem.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
constexpr float ProvinceRadiusScale = 6.2f;

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
    Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
}

void AAPGreyboxWorldBuilder::SpawnProvince(const FAPProvinceState& Province)
{
    const FLinearColor Color = Province.OwnerId == 0
        ? FLinearColor(0.08f, 0.42f, 0.65f)
        : (Province.OwnerId == 1 ? FLinearColor(0.65f, 0.12f, 0.08f) : FLinearColor(0.30f, 0.28f, 0.24f));
    const FString OwnerLabel = Province.OwnerId == 0 ? TEXT("PLAYER") : (Province.OwnerId == 1 ? TEXT("AI") : TEXT("NEUTRAL"));
    const FVector Location = ProvinceLocation(Province.ProvinceId);
    ProvinceShapes.Add(Province.ProvinceId, SpawnShape(CylinderMesh, Location,
        FVector(ProvinceRadiusScale, ProvinceRadiusScale, 0.18f), Color,
        FString::Printf(TEXT("Province_%d"), Province.ProvinceId)));
    ProvinceLabels.Add(Province.ProvinceId, SpawnLabel(
        FString::Printf(TEXT("PROVINCE %d - %s"), Province.ProvinceId + 1, *OwnerLabel),
        Location + FVector(0.0f, 0.0f, 130.0f), Province.OwnerId == 1 ? FColor::Red : FColor::White));
}

void AAPGreyboxWorldBuilder::SpawnRoute(int32 FromProvinceId, int32 ToProvinceId)
{
    const FVector Start = ProvinceLocation(FromProvinceId);
    const FVector End = ProvinceLocation(ToProvinceId);
    const FVector Delta = End - Start;
    const FVector Midpoint = (Start + End) * 0.5f + FVector(0.0f, 0.0f, 5.0f);
    const float LengthScale = Delta.Size2D() / 100.0f;
    const float Yaw = FMath::RadiansToDegrees(FMath::Atan2(Delta.Y, Delta.X));
    SpawnShape(CubeMesh, Midpoint, FVector(LengthScale, 0.12f, 0.05f),
        FLinearColor(0.55f, 0.42f, 0.20f), TEXT("ProvinceRoute"), Yaw);
}

void AAPGreyboxWorldBuilder::SpawnSettlement()
{
    const FVector TownCenter = ProvinceLocation(0) + FVector(0.0f, 0.0f, 100.0f);
    SpawnShape(CylinderMesh, TownCenter, FVector(1.8f, 1.8f, 0.8f),
        FLinearColor(0.85f, 0.68f, 0.22f), TEXT("PlayerTownCenter"));
    SpawnLabel(TEXT("ASHFALL SETTLEMENT"), TownCenter + FVector(0.0f, 0.0f, 190.0f), FColor::Yellow);

    for (int32 BuildingIndex = 0; BuildingIndex < 8; ++BuildingIndex)
    {
        const float Angle = 2.0f * PI * static_cast<float>(BuildingIndex) / 8.0f;
        const FVector Offset(FMath::Cos(Angle) * 320.0f, FMath::Sin(Angle) * 320.0f, 90.0f);
        SpawnShape(CubeMesh, TownCenter + Offset, FVector(0.7f, 0.7f, 1.2f),
            FLinearColor(0.48f, 0.35f, 0.18f), FString::Printf(TEXT("BuildingSlot_%d"), BuildingIndex + 1));
    }

    for (int32 WorkerMarker = 0; WorkerMarker < 6; ++WorkerMarker)
    {
        const FVector Offset(-260.0f + WorkerMarker * 105.0f, -190.0f, 45.0f);
        SpawnShape(CubeMesh, TownCenter + Offset, FVector(0.22f, 0.22f, 0.45f),
            FLinearColor(0.25f, 0.75f, 0.30f), TEXT("WorkingHouseholdMarker"));
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
            UStaticMesh* Mesh = Company.SoldierType == EAPSoldierType::Spear
                ? ConeMesh.Get() : (Company.SoldierType == EAPSoldierType::Ranged ? CubeMesh.Get() : SphereMesh.Get());
            Shape = SpawnShape(Mesh, DesiredLocation, FVector(0.8f, 0.8f, 1.6f), Color,
                FString::Printf(TEXT("Company_%d"), Company.CompanyId));
            CompanyShapes.Add(Company.CompanyId, Shape);
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
            ? FVector(1.15f, 1.15f, 2.1f) : FVector(0.8f, 0.8f, 1.6f));
        Label->SetActorLocation(VisualLocation + FVector(0.0f, 0.0f, 190.0f));
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
            CompanyShapes.Remove(CompanyId);
            CompanyLabels.Remove(CompanyId);
        }
    }
}
