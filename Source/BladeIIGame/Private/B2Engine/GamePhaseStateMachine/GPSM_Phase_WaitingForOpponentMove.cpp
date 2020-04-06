#include "B2Engine/GamePhaseStateMachine/GPSM_Phase_WaitingForOpponentMove.h"

#include "B2Misc/Utility.h"

#include "../BladeIIGameGameMode.h"

GPSM_Phase_WaitingForOpponentMove::GPSM_Phase_WaitingForOpponentMove()
{
	GPSM_Phase::GPSM_Phase();
}

void GPSM_Phase_WaitingForOpponentMove::Init(ABladeIIGameGameMode* GameMode)
{
	GPSM_Phase::Init(GameMode);
}

void GPSM_Phase_WaitingForOpponentMove::Tick(float DeltaSeconds)
{
	GPSM_Phase::Tick(DeltaSeconds);
}

void GPSM_Phase_WaitingForOpponentMove::End()
{
	GPSM_Phase::End();


}
