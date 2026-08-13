#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Simulation/APTypes.h"
#include "APGreyboxWorldBuilder.generated.h"

class UMaterialInterface;
class UMaterialInstanceDynamic;
class UStaticMesh;
class UHierarchicalInstancedStaticMeshComponent;
class UProceduralMeshComponent;

UCLASS()
class ASHFALLPROVINCES_API AAPGreyboxWorldBuilder : public AActor
{
    GENERATED_BODY()

public:
    AAPGreyboxWorldBuilder();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    int32 GetProvinceVisualCount() const { return ProvinceShapes.Num(); }
    int32 GetRouteVisualCount() const { return RouteVisualCount; }
    int32 GetPlayerBuildingVisualCount() const { return PlayerBuildingVisualCount; }
    bool HasPlayerSettlementVisual() const { return bHasPlayerSettlementVisual; }
    bool HasAIOutpostVisual() const { return bHasAIOutpostVisual; }
    int32 GetNeutralLandmarkCount() const { return NeutralLandmarkCount; }
    int32 GetEnvironmentInstanceCount() const { return EnvironmentInstanceCount; }
    int32 GetTerrainPatchCount() const { return TerrainPatchCount; }
    int32 GetCompanyVisualCount() const { return CompanyShapes.Num(); }
    bool IsCompanyVisualSelected(int32 CompanyId) const;
    FVector GetCompanyVisualLocation(int32 CompanyId) const;

private:
    static FVector ProvinceLocation(int32 ProvinceId);
    AActor* SpawnShape(UStaticMesh* Mesh, const FVector& Location, const FVector& Scale,
        const FLinearColor& Color, const FString& Name, float Yaw = 0.0f);
    AActor* SpawnLabel(const FString& Text, const FVector& Location, const FColor& Color);
    void SpawnStaticWorld();
    void SpawnTerrain();
    void SpawnAtmosphere();
    void SpawnProvince(const FAPProvinceState& Province);
    void SpawnRoute(int32 FromProvinceId, int32 ToProvinceId);
    void SpawnSettlement();
    void SpawnAIAndNeutralLandmarks();
    void SpawnBuilding(const FVector& Location, float Yaw, const FString& Name, const FLinearColor& WallColor,
        float Scale = 1.0f);
    void SpawnUnitFormation(int32 CompanyId, EAPSoldierType Type, const FVector& Location, const FLinearColor& Color);
    UHierarchicalInstancedStaticMeshComponent* CreateInstanceLayer(const FString& Name, UStaticMesh* Mesh,
        const FLinearColor& Color);
    UMaterialInstanceDynamic* GetSharedMaterial(const FLinearColor& Color);
    void LogVisualStats();
    void FaceLabelsToCamera();
    void UpdateCompanyVisuals(float DeltaSeconds);
    void UpdateProvinceVisuals();

    UPROPERTY() TObjectPtr<UStaticMesh> CylinderMesh;
    UPROPERTY() TObjectPtr<UStaticMesh> CubeMesh;
    UPROPERTY() TObjectPtr<UStaticMesh> ConeMesh;
    UPROPERTY() TObjectPtr<UStaticMesh> SphereMesh;
    UPROPERTY() TObjectPtr<UMaterialInterface> BaseMaterial;
    UPROPERTY() TMap<uint32, TObjectPtr<UMaterialInstanceDynamic>> SharedMaterials;
    UPROPERTY() TMap<int32, TObjectPtr<AActor>> CompanyShapes;
    UPROPERTY() TMap<int32, TObjectPtr<AActor>> CompanyLabels;
    UPROPERTY() TMap<int32, TObjectPtr<AActor>> ProvinceShapes;
    UPROPERTY() TMap<int32, TObjectPtr<AActor>> ProvinceLabels;
    UPROPERTY() TArray<TObjectPtr<AActor>> WorldLabels;
    UPROPERTY() TMap<int32, TObjectPtr<AActor>> CompanySelectionRings;
    UPROPERTY() TMap<int32, TObjectPtr<AActor>> DestinationMarkers;
    UPROPERTY() TObjectPtr<UHierarchicalInstancedStaticMeshComponent> TerrainDryInstances;
    UPROPERTY() TObjectPtr<UHierarchicalInstancedStaticMeshComponent> TerrainGreenInstances;
    UPROPERTY() TObjectPtr<UHierarchicalInstancedStaticMeshComponent> TerrainAshInstances;
    UPROPERTY() TObjectPtr<UProceduralMeshComponent> ContinuousTerrain;
    UPROPERTY() TObjectPtr<UHierarchicalInstancedStaticMeshComponent> TreeInstances;
    UPROPERTY() TObjectPtr<UHierarchicalInstancedStaticMeshComponent> RockInstances;
    UPROPERTY() TObjectPtr<UHierarchicalInstancedStaticMeshComponent> RidgeInstances;
    TMap<int32, TArray<TObjectPtr<AActor>>> CompanyFormationParts;

    float CompanyRefreshAccumulator = 0.0f;
    int32 RouteVisualCount = 0;
    int32 PlayerBuildingVisualCount = 0;
    bool bHasPlayerSettlementVisual = false;
    bool bHasAIOutpostVisual = false;
    int32 NeutralLandmarkCount = 0;
    int32 EnvironmentInstanceCount = 0;
    int32 TerrainPatchCount = 0;
    bool bVisualStatsLogged = false;
};
