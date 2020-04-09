#include "B2Engine/GameStateMachine/GSM_State_WaitingForOpponentMove.h"

#include "B2Utility/Log.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_WaitingForOpponentMove::GSM_State_WaitingForOpponentMove()
{
	GSM_State::GSM_State();
}

void GSM_State_WaitingForOpponentMove::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);
}

void GSM_State_WaitingForOpponentMove::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);
}

void GSM_State_WaitingForOpponentMove::End()
{
	GSM_State::End();


}