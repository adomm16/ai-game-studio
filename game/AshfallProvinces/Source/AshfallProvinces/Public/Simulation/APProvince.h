#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Simulation/APTypes.h"
#include "APProvince.generated.h"

UCLASS(BlueprintType)
class ASHFALLPROVINCES_API UAPProvince : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame) FAPProvinceState State;
};
