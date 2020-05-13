#include "B2Engine/GameStateMachine/GSM_State_OpponentBlast.h"

#include "B2Utility/Log.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_OpponentBlast::GSM_State_OpponentBlast()
{
	GSM_State::GSM_State();
}

void GSM_State_OpponentBlast::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	ABladeIIGameMode* GI = GameModeInstance;

	// Play blast animation in the center of the screen
	GI->GetUIEffectLayer()->Play(EUIEffect::Blast, nullptr, 0.2f, 0.0f);

	GI->GetDealer()->PreBlastSelect(EPlayer::Player);

	// Play blast sound effect
	GI->GetGameSound()->PlaySFX(ESFX::EffectBlastStart, 0.2f);
	GI->GetGameSound()->PlaySFX(ESFX::EffectBase, 0.2f);

	OpponentMessage(EOpponentMessage::Blast);

	B2Utility::LogInfo("GSM_State_OpponentBlast PreBlastSelect");
}

void GSM_State_OpponentBlast::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

	EUIEffectEvent Event;
	while (GI->GetUIEffectLayer()->EventQueue.Dequeue(Event))
	{
		if (Event == EUIEffectEvent::Finished)
		{
			B2Utility::LogInfo("GSM_State_OpponentBlast BlastSelect");

			GI->GetDealer()->BlastSelect(EPlayer::Player);

		}
	}
}

void GSM_State_OpponentBlast::End()
{
	GSM_State::End();


}
