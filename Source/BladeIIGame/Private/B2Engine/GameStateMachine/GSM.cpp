#include "B2Engine/GameStateMachine/GSM.h"

#include "B2GameMode/BladeIIGameMode.h"

B2GameStateMachine::B2GameStateMachine(ABladeIIGameMode* GameMode)
{
	GameModeInstance = GameMode;
	CurrentState = nullptr;
}

void B2GameStateMachine::Tick(float DeltaSeconds)
{
	CurrentState->Tick(DeltaSeconds);
}
