#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APPrototypeWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class ASHFALLPROVINCES_API UAPPrototypeWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UButton* GetSpearButton() const { return SpearButton; }
    UButton* GetRangedButton() const { return RangedButton; }
    UButton* GetScoutButton() const { return ScoutButton; }
    UButton* GetAttackButton() const { return AttackButton; }
    bool AreCommandCallbacksBound() const;

protected:
    virtual void NativeOnInitialized() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
    UFUNCTION() void HandleSpearClicked();
    UFUNCTION() void HandleRangedClicked();
    UFUNCTION() void HandleScoutClicked();
    UFUNCTION() void HandleAttackClicked();
    void BuildInterface();
    void RefreshFromSimulation();

    UPROPERTY(Transient) TObjectPtr<UButton> SpearButton;
    UPROPERTY(Transient) TObjectPtr<UButton> RangedButton;
    UPROPERTY(Transient) TObjectPtr<UButton> ScoutButton;
    UPROPERTY(Transient) TObjectPtr<UButton> AttackButton;
    UPROPERTY(Transient) TObjectPtr<UTextBlock> ResourcesText;
    UPROPERTY(Transient) TObjectPtr<UTextBlock> PopulationText;
    UPROPERTY(Transient) TObjectPtr<UTextBlock> ArmyText;
    UPROPERTY(Transient) TObjectPtr<UTextBlock> SelectionText;
    UPROPERTY(Transient) TObjectPtr<UTextBlock> LastEventText;
    UPROPERTY(Transient) TObjectPtr<UTextBlock> BattleResultText;
};
