#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Simulation/APTypes.h"
#include "APSettlement.generated.h"

UCLASS(BlueprintType)
class ASHFALLPROVINCES_API UAPSettlement : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame) int32 SettlementId = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame) int32 BuildingSlots = 8;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame) FAPResources Resources;
};
