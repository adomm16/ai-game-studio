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
    UFUNCTION(BlueprintPure) int32 GetTargetProvinceId() const { return TargetProvinceId; }
    UFUNCTION(BlueprintPure) FString GetLastEvent() const { return LastEvent; }

protected:
    virtual void SetupInputComponent() override;

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

    int32 SelectedCompanyId = INDEX_NONE;
    int32 TargetProvinceId = 1;
    FString LastEvent = TEXT("Ready - muster a company with M, R or C");
};
