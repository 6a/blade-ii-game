#include "B2Engine/GameStateMachine/GSM_State_PlayerBlast.h"

#include "B2Utility/Log.h"

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
	GI->GetUIEffectLayer()->Play(EUIEffect::Blast, nullptr, 0.2f, 0.0f);

	GI->GetDealer()->PreBlastSelect(EPlayer::Opponent);

	// Play blast sound effect
	GI->GetGameSound()->PlaySFX(ESFX::EffectBlastStart, 0.2f);
	GI->GetGameSound()->PlaySFX(ESFX::EffectBase, 0.2f);
}

void GSM_State_PlayerBlast::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

	EUIEffectEvent Event;
	while (GI->GetUIEffectLayer()->EventQueue.Dequeue(Event))
	{
		if (Event == EUIEffectEvent::Finished)
		{
			GI->GetDealer()->BlastSelect(EPlayer::Opponent);
		}
	}
}

void GSM_State_PlayerBlast::End()
{
	GSM_State::End();


}
