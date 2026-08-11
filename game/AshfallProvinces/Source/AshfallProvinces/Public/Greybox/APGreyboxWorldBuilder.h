#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Simulation/APTypes.h"
#include "APGreyboxWorldBuilder.generated.h"

class UMaterialInterface;
class UStaticMesh;

UCLASS()
class ASHFALLPROVINCES_API AAPGreyboxWorldBuilder : public AActor
{
    GENERATED_BODY()

public:
    AAPGreyboxWorldBuilder();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

private:
    static FVector ProvinceLocation(int32 ProvinceId);
    AActor* SpawnShape(UStaticMesh* Mesh, const FVector& Location, const FVector& Scale,
        const FLinearColor& Color, const FString& Name, float Yaw = 0.0f);
    AActor* SpawnLabel(const FString& Text, const FVector& Location, const FColor& Color);
    void SpawnStaticWorld();
    void SpawnProvince(const FAPProvinceState& Province);
    void SpawnRoute(int32 FromProvinceId, int32 ToProvinceId);
    void SpawnSettlement();
    void UpdateCompanyVisuals(float DeltaSeconds);
    void UpdateProvinceVisuals();

    UPROPERTY() TObjectPtr<UStaticMesh> CylinderMesh;
    UPROPERTY() TObjectPtr<UStaticMesh> CubeMesh;
    UPROPERTY() TObjectPtr<UStaticMesh> ConeMesh;
    UPROPERTY() TObjectPtr<UStaticMesh> SphereMesh;
    UPROPERTY() TObjectPtr<UMaterialInterface> BaseMaterial;
    UPROPERTY() TMap<int32, TObjectPtr<AActor>> CompanyShapes;
    UPROPERTY() TMap<int32, TObjectPtr<AActor>> CompanyLabels;
    UPROPERTY() TMap<int32, TObjectPtr<AActor>> ProvinceShapes;
    UPROPERTY() TMap<int32, TObjectPtr<AActor>> ProvinceLabels;

    float CompanyRefreshAccumulator = 0.0f;
};
