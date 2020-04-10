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
	GI->GetEffectLayer()->Play(EUIEffect::Blast, nullptr, 0.4f, 0.5f);
}

void GSM_State_PlayerBlast::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

	EUIEffectEvent Event;
	while (GI->GetEffectLayer()->EventQueue.Dequeue(Event))
	{
		if (Event == EUIEffectEvent::Finished)
		{
			// Signal to the game mode that the turn has finished
			GI->FinishTurn();
		}
	}
}

void GSM_State_PlayerBlast::End()
{
	GSM_State::End();


}
