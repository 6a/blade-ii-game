#include "B2Engine/GamePhaseStateMachine/GPSM_Phase_PlayerBolt.h"

#include "B2Misc/Utility.h"

#include "B2GameMode/BladeIIGameGameMode.h"

GPSM_Phase_PlayerBolt::GPSM_Phase_PlayerBolt()
{
	GPSM_Phase::GPSM_Phase();
}

void GPSM_Phase_PlayerBolt::Init(ABladeIIGameGameMode* GameMode)
{
	GPSM_Phase::Init(GameMode);

	// Play bolt animation at opponents last card position
}

void GPSM_Phase_PlayerBolt::Tick(float DeltaSeconds)
{
	GPSM_Phase::Tick(DeltaSeconds);
}

void GPSM_Phase_PlayerBolt::End()
{
	GPSM_Phase::End();


}
