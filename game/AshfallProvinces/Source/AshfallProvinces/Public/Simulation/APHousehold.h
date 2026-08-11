#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Simulation/APTypes.h"
#include "APHousehold.generated.h"

UCLASS(BlueprintType)
class ASHFALLPROVINCES_API UAPHousehold : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame) FAPHouseholdState State;
};
