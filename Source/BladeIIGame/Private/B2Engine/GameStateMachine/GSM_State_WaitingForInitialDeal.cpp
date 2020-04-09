#include "B2Engine/GameStateMachine/GSM_State_WaitingForInitialDeal.h"

#include "B2Utility/Log.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_WaitingForInitialDeal::GSM_State_WaitingForInitialDeal()
{
	GSM_State::GSM_State();
}

void GSM_State_WaitingForInitialDeal::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);
}

void GSM_State_WaitingForInitialDeal::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);
}

void GSM_State_WaitingForInitialDeal::End()
{
	GSM_State::End();


}
