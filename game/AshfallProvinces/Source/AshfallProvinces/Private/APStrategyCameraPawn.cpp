#include "APStrategyCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AAPStrategyCameraPawn::AAPStrategyCameraPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(SceneRoot);
    SpringArm->TargetArmLength = 2200.0f;
    SpringArm->SetRelativeRotation(FRotator(-55.0f, 0.0f, 0.0f));
    SpringArm->bDoCollisionTest = false;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

void AAPStrategyCameraPawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    AddActorWorldOffset(FVector(PendingMove.Y, PendingMove.X, 0.0f) * 1200.0f * DeltaSeconds);
}

void AAPStrategyCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AAPStrategyCameraPawn::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AAPStrategyCameraPawn::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Zoom"), this, &AAPStrategyCameraPawn::Zoom);
}

void AAPStrategyCameraPawn::MoveForward(float Value) { PendingMove.Y = Value; }
void AAPStrategyCameraPawn::MoveRight(float Value) { PendingMove.X = Value; }
void AAPStrategyCameraPawn::Zoom(float Value)
{
    SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength - Value * 150.0f, 700.0f, 3500.0f);
}
