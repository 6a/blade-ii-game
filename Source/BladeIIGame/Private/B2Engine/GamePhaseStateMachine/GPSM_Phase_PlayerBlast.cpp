#include "B2Engine/GamePhaseStateMachine/GPSM_Phase_PlayerBlast.h"

#include "B2Misc/Utility.h"

#include "B2GameMode/BladeIIGameGameMode.h"

GPSM_Phase_PlayerBlast::GPSM_Phase_PlayerBlast()
{
	GPSM_Phase::GPSM_Phase();
}

void GPSM_Phase_PlayerBlast::Init(ABladeIIGameGameMode* GameMode)
{
	GPSM_Phase::Init(GameMode);

	// Play blast animation in the center of the screen
	GameModeInstance->GetEffectLayer()->Play(EEffect::Blast, nullptr, 0.5f, 0.5f);
}

void GPSM_Phase_PlayerBlast::Tick(float DeltaSeconds)
{
	GPSM_Phase::Tick(DeltaSeconds);
}

void GPSM_Phase_PlayerBlast::End()
{
	GPSM_Phase::End();


}
