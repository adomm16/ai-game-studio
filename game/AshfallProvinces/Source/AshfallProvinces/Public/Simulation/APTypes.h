#pragma once

#include "CoreMinimal.h"
#include "APTypes.generated.h"

UENUM(BlueprintType)
enum class EAPHouseholdJob : uint8
{
    Idle,
    Food,
    Timber,
    Metal
};

UENUM(BlueprintType)
enum class EAPSoldierType : uint8
{
    Spear,
    Ranged,
    Scout
};

USTRUCT(BlueprintType)
struct ASHFALLPROVINCES_API FAPResources
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame) int32 Food = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame) int32 Timber = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame) int32 Metal = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame) int32 Authority = 0;

    FAPResources& operator+=(const FAPResources& Other)
    {
        Food += Other.Food;
        Timber += Other.Timber;
        Metal += Other.Metal;
        Authority += Other.Authority;
        return *this;
    }
};

USTRUCT(BlueprintType)
struct ASHFALLPROVINCES_API FAPHouseholdState
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame) int32 HouseholdId = INDEX_NONE;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame) EAPHouseholdJob Job = EAPHouseholdJob::Idle;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame) bool bAlive = true;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame) bool bMustered = false;
};

USTRUCT(BlueprintType)
struct ASHFALLPROVINCES_API FAPProvinceState
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame) int32 ProvinceId = INDEX_NONE;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame) int32 OwnerId = INDEX_NONE;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame) TArray<int32> AdjacentProvinceIds;
};

USTRUCT(BlueprintType)
struct ASHFALLPROVINCES_API FAPArmyState
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame) int32 CompanyId = INDEX_NONE;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame) int32 OwnerId = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame) EAPSoldierType SoldierType = EAPSoldierType::Spear;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame) int32 ProvinceId = 0;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame) int32 DestinationProvinceId = INDEX_NONE;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame) int32 TravelTicksRemaining = 0;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame) TArray<int32> HouseholdIds;
};
