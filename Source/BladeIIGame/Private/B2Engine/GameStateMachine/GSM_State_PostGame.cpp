#include "B2Engine/GameStateMachine/GSM_State_PostGame.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_PostGame::GSM_State_PostGame()
{
	GSM_State::GSM_State();
}

void GSM_State_PostGame::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	ABladeIIGameMode* GI = GameModeInstance;
}

void GSM_State_PostGame::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;
}

void GSM_State_PostGame::End()
{
	GSM_State::End();
}