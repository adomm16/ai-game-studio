#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Simulation/APTypes.h"
#include "APPlayerController.generated.h"

UCLASS()
class ASHFALLPROVINCES_API AAPPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AAPPlayerController();

    UFUNCTION(BlueprintPure) int32 GetSelectedCompanyId() const { return SelectedCompanyId; }
    UFUNCTION(BlueprintPure) int32 GetSelectedProvinceId() const { return SelectedProvinceId; }
    UFUNCTION(BlueprintPure) int32 GetTargetProvinceId() const { return TargetProvinceId; }
    UFUNCTION(BlueprintPure) FString GetLastEvent() const { return LastEvent; }
    UFUNCTION(BlueprintPure) FString GetBattleResult() const { return BattleResult; }
    UFUNCTION(BlueprintPure) bool IsBattleResultVisible() const;

    void RequestMuster(EAPSoldierType SoldierType);
    void RequestAttack();

protected:
    virtual void SetupInputComponent() override;
    virtual void PlayerTick(float DeltaTime) override;

private:
    void MusterSpear();
    void MusterRanged();
    void MusterScout();
    void MusterCompany(EAPSoldierType SoldierType);
    void SelectCompany1();
    void SelectCompany2();
    void SelectCompany3();
    void SelectCompany4();
    void SelectCompanySlot(int32 SlotIndex);
    void CycleTargetProvince();
    void MoveSelectedCompany();
    void ResolveSelectedBattle();
    void SaveGame();
    void LoadGame();
    void HandleLeftMouseButton();
    void HandleRightMouseButton();
    bool HandleWorldClick(bool bMoveOrder);
    void SelectProvince(int32 ProvinceId);
    void SelectCompany(int32 CompanyId);
    void TryAutoCapture();

    int32 SelectedCompanyId = INDEX_NONE;
    int32 SelectedProvinceId = 0;
    int32 TargetProvinceId = 1;
    FString LastEvent = TEXT("Ready - use the RECRUIT / MUSTER panel");
    FString BattleResult;
    float BattleResultExpiresAt = -1.0f;
};
