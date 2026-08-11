#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Simulation/APTypes.h"
#include "APSimulationSubsystem.generated.h"

UCLASS()
class ASHFALLPROVINCES_API UAPSimulationSubsystem : public UTickableWorldSubsystem
{
    GENERATED_BODY()

public:
    static constexpr int32 InitialHouseholdCount = 24;
    static constexpr int32 ProvinceCount = 6;
    static constexpr int32 MaxCompanies = 4;
    static constexpr float FixedTickSeconds = 1.0f;

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override;

    UFUNCTION(BlueprintCallable) void StartNewGame(int32 Seed = 1);
    UFUNCTION(BlueprintCallable) bool MusterCompany(EAPSoldierType Type, const TArray<int32>& HouseholdIds);
    UFUNCTION(BlueprintCallable) bool OrderMove(int32 CompanyId, int32 DestinationProvinceId);
    UFUNCTION(BlueprintCallable) int32 ApplyCasualties(int32 CompanyId, int32 CasualtyCount);
    UFUNCTION(BlueprintCallable) bool ResolveBattle(int32 AttackerCompanyId, int32 DefenderCompanyId);
    UFUNCTION(BlueprintCallable) bool CaptureProvince(int32 CompanyId);
    UFUNCTION(BlueprintCallable) bool SaveSimulation(const FString& SlotName);
    UFUNCTION(BlueprintCallable) bool LoadSimulation(const FString& SlotName);

    UFUNCTION(BlueprintPure) FAPResources GetResources() const { return SettlementResources; }
    UFUNCTION(BlueprintPure) TArray<FAPHouseholdState> GetHouseholds() const { return Households; }
    UFUNCTION(BlueprintPure) TArray<FAPProvinceState> GetProvinces() const { return Provinces; }
    UFUNCTION(BlueprintPure) TArray<FAPArmyState> GetCompanies() const { return Companies; }
    UFUNCTION(BlueprintPure) int64 GetSimulationTick() const { return SimulationTick; }
    UFUNCTION(BlueprintPure) int32 GetSettlementBuildingSlots() const { return SettlementBuildingSlots; }

private:
    void StepSimulation();
    FAPArmyState* FindCompany(int32 CompanyId);
    const FAPProvinceState* FindProvince(int32 ProvinceId) const;

    UPROPERTY(SaveGame) FAPResources SettlementResources;
    UPROPERTY(SaveGame) int32 SettlementBuildingSlots = 8;
    UPROPERTY(SaveGame) TArray<FAPHouseholdState> Households;
    UPROPERTY(SaveGame) TArray<FAPProvinceState> Provinces;
    UPROPERTY(SaveGame) TArray<FAPArmyState> Companies;
    UPROPERTY(SaveGame) int64 SimulationTick = 0;
    UPROPERTY(SaveGame) uint32 RandomSeed = 1;

    float AccumulatedSeconds = 0.0f;
};
