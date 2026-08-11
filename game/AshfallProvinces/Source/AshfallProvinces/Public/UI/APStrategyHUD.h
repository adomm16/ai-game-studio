#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "APStrategyHUD.generated.h"

UCLASS()
class ASHFALLPROVINCES_API AAPStrategyHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;
};
