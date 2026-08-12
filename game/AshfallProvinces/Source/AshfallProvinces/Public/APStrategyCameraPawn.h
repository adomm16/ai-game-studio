#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "APStrategyCameraPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ASHFALLPROVINCES_API AAPStrategyCameraPawn : public APawn
{
    GENERATED_BODY()

public:
    AAPStrategyCameraPawn();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void Zoom(float Value);

    UPROPERTY(VisibleAnywhere) TObjectPtr<USceneComponent> SceneRoot;
    UPROPERTY(VisibleAnywhere) TObjectPtr<USpringArmComponent> SpringArm;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UCameraComponent> Camera;

    FVector2D PendingMove = FVector2D::ZeroVector;
    FVector2D SmoothedMove = FVector2D::ZeroVector;
    float DesiredArmLength = 5200.0f;
};
