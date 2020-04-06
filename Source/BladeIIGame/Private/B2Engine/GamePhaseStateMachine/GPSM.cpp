#include "B2Engine/GamePhaseStateMachine/GPSM.h"

#include "B2GameMode/BladeIIGameGameMode.h"

B2GPSM::B2GPSM(ABladeIIGameGameMode* GameMode)
{
	GameModeInstance = GameMode;
	CurrentState = nullptr;
}

void B2GPSM::Tick(float DeltaSeconds)
{
	CurrentState->Tick(DeltaSeconds);
}
