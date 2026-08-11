#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Simulation/APTypes.h"
#include "APArmyCompany.generated.h"

UCLASS(BlueprintType)
class ASHFALLPROVINCES_API UAPArmyCompany : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame) FAPArmyState State;
};
