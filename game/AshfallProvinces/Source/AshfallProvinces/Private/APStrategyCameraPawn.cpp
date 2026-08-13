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
    SpringArm->TargetArmLength = 5200.0f;
    SpringArm->SetRelativeRotation(FRotator(-55.0f, -32.0f, 0.0f));
    SpringArm->bDoCollisionTest = false;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

void AAPStrategyCameraPawn::BeginPlay()
{
    Super::BeginPlay();
    SetActorLocation(FVector(0.0f, 0.0f, 120.0f));
    DesiredArmLength = SpringArm->TargetArmLength;
}

void AAPStrategyCameraPawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    SmoothedMove = FMath::Vector2DInterpTo(SmoothedMove, PendingMove, DeltaSeconds, 6.0f);
    const FVector Movement(SmoothedMove.Y, SmoothedMove.X, 0.0f);
    const FVector DesiredLocation = GetActorLocation() + Movement * 1450.0f * DeltaSeconds;
    SetActorLocation(FVector(
        FMath::Clamp(DesiredLocation.X, -3200.0f, 3200.0f),
        FMath::Clamp(DesiredLocation.Y, -2600.0f, 2600.0f),
        120.0f));
    SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, DesiredArmLength, DeltaSeconds, 7.5f);
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
    DesiredArmLength = FMath::Clamp(DesiredArmLength - Value * 420.0f, 2100.0f, 6500.0f);
}

void AAPStrategyCameraPawn::FocusSettlementForVisualProof()
{
    SetActorLocation(FVector(-1600.0f, -850.0f, 120.0f));
    DesiredArmLength = 2450.0f;
    SpringArm->TargetArmLength = DesiredArmLength;
}
