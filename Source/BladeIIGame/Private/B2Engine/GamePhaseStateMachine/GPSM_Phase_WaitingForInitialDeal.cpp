#include "B2Engine/GamePhaseStateMachine/GPSM_Phase_WaitingForInitialDeal.h"

#include "B2Misc/Utility.h"

#include "../BladeIIGameGameMode.h"

GPSM_Phase_WaitingForInitialDeal::GPSM_Phase_WaitingForInitialDeal()
{
	GPSM_Phase::GPSM_Phase();
}

void GPSM_Phase_WaitingForInitialDeal::Init(ABladeIIGameGameMode* GameMode)
{
	GPSM_Phase::Init(GameMode);
}

void GPSM_Phase_WaitingForInitialDeal::Tick(float DeltaSeconds)
{
	GPSM_Phase::Tick(DeltaSeconds);
}

void GPSM_Phase_WaitingForInitialDeal::End()
{
	GPSM_Phase::End();


}
