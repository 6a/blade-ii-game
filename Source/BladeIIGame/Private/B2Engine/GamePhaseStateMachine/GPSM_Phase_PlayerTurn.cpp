#include "B2Engine/GamePhaseStateMachine/GPSM_Phase_PlayerTurn.h"

#include "B2Misc/Utility.h"

#include "../BladeIIGameGameMode.h"

GPSM_Phase_PlayerTurn::GPSM_Phase_PlayerTurn()
{
	GPSM_Phase::GPSM_Phase();
}

void GPSM_Phase_PlayerTurn::Init(ABladeIIGameGameMode* GameMode)
{
	GPSM_Phase::Init(GameMode);
}

void GPSM_Phase_PlayerTurn::Tick(float DeltaSeconds)
{
	GPSM_Phase::Tick(DeltaSeconds);
}

void GPSM_Phase_PlayerTurn::End()
{
	GPSM_Phase::End();


}
