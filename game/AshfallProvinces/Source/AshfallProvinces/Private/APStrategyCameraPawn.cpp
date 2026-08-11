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
    SpringArm->TargetArmLength = 4800.0f;
    SpringArm->SetRelativeRotation(FRotator(-58.0f, -35.0f, 0.0f));
    SpringArm->bDoCollisionTest = false;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

void AAPStrategyCameraPawn::BeginPlay()
{
    Super::BeginPlay();
    SetActorLocation(FVector::ZeroVector);
}

void AAPStrategyCameraPawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    const FVector Movement(PendingMove.Y, PendingMove.X, 0.0f);
    const FVector DesiredLocation = GetActorLocation() + Movement * 1500.0f * DeltaSeconds;
    SetActorLocation(FVector(
        FMath::Clamp(DesiredLocation.X, -3200.0f, 3200.0f),
        FMath::Clamp(DesiredLocation.Y, -2600.0f, 2600.0f),
        0.0f));
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
    SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength - Value * 350.0f, 1600.0f, 6200.0f);
}
