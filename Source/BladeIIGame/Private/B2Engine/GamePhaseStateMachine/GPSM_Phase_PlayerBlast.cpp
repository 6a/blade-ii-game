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

	ABladeIIGameGameMode* GI = GameModeInstance;

	// Play blast animation in the center of the screen
	GI->GetEffectLayer()->Play(EEffect::Blast, nullptr, 0.0f, 0.5f);
}

void GPSM_Phase_PlayerBlast::Tick(float DeltaSeconds)
{
	GPSM_Phase::Tick(DeltaSeconds);

	ABladeIIGameGameMode* GI = GameModeInstance;

	if (GI->GetGameState()->bPendingEffectRequiresAction)
	{
		// Perform the rest of the blast sequence


		GI->GetGameState()->bPendingEffectRequiresAction = false;
	}
}

void GPSM_Phase_PlayerBlast::End()
{
	GPSM_Phase::End();


}
