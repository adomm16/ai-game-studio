#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "APStrategyHUD.generated.h"

UCLASS()
class ASHFALLPROVINCES_API AAPStrategyHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;
    virtual void DrawHUD() override;
    bool HandlePointerInput(const FVector2D& ScreenPosition, bool bRightClick);

private:
    void DrawPanel(float X, float Y, float Width, float Height, const FLinearColor& Color) const;
    void DrawButton(const FString& Label, const FBox2D& Bounds, bool bEnabled) const;

    FBox2D SpearButton;
    FBox2D RangedButton;
    FBox2D ScoutButton;
    FBox2D AttackButton;
    bool bAttackEnabled = false;
};
