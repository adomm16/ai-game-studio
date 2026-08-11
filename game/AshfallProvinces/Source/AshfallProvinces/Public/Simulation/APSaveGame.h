#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Simulation/APTypes.h"
#include "APSaveGame.generated.h"

UCLASS()
class ASHFALLPROVINCES_API UAPSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(SaveGame) int64 SimulationTick = 0;
    UPROPERTY(SaveGame) uint32 RandomSeed = 1;
    UPROPERTY(SaveGame) FAPResources SettlementResources;
    UPROPERTY(SaveGame) int32 SettlementBuildingSlots = 8;
    UPROPERTY(SaveGame) TArray<FAPHouseholdState> Households;
    UPROPERTY(SaveGame) TArray<FAPProvinceState> Provinces;
    UPROPERTY(SaveGame) TArray<FAPArmyState> Companies;
};
