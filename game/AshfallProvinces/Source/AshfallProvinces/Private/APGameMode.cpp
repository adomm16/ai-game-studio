#include "APGameMode.h"
#include "APPlayerController.h"
#include "APStrategyCameraPawn.h"

AAPGameMode::AAPGameMode()
{
    PlayerControllerClass = AAPPlayerController::StaticClass();
    DefaultPawnClass = AAPStrategyCameraPawn::StaticClass();
}
