#include "B2Engine/GameStateMachine/GSM_State_PlayerBlast.h"

#include "B2Misc/Utility.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_PlayerBlast::GSM_State_PlayerBlast()
{
	GSM_State::GSM_State();
}

void GSM_State_PlayerBlast::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	ABladeIIGameMode* GI = GameModeInstance;

	// Play blast animation in the center of the screen
	GI->GetEffectLayer()->Play(EUIEffect::Blast, nullptr, 0.0f, 0.5f);
}

void GSM_State_PlayerBlast::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

	if (GI->GetGameState()->bPendingEffectRequiresAction)
	{
		// Perform the rest of the blast sequence


		GI->GetGameState()->bPendingEffectRequiresAction = false;
	}
}

void GSM_State_PlayerBlast::End()
{
	GSM_State::End();


}
