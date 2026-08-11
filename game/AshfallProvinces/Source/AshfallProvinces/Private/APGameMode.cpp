#include "APGameMode.h"
#include "APPlayerController.h"
#include "APStrategyCameraPawn.h"
#include "Greybox/APGreyboxWorldBuilder.h"
#include "UI/APStrategyHUD.h"

AAPGameMode::AAPGameMode()
{
    PlayerControllerClass = AAPPlayerController::StaticClass();
    DefaultPawnClass = AAPStrategyCameraPawn::StaticClass();
    HUDClass = AAPStrategyHUD::StaticClass();
}

void AAPGameMode::BeginPlay()
{
    Super::BeginPlay();
    GetWorld()->SpawnActor<AAPGreyboxWorldBuilder>();
}
