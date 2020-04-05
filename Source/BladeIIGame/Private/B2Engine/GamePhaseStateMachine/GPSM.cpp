#include "B2Engine/GamePhaseStateMachine/GPSM.h"

#include "../BladeIIGameGameMode.h"

B2GPSM::B2GPSM(ABladeIIGameGameMode* GameMode)
{
	GameModeInstance = GameMode;
	CurrentState = nullptr;
}

void B2GPSM::Tick(float DeltaSeconds)
{
	CurrentState->Tick(DeltaSeconds);
}
