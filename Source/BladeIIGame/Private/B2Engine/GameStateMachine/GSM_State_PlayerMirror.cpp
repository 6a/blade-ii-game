#include "B2Engine/GameStateMachine/GSM_State_PlayerMirror.h"

#include "TimerManager.h"

#include "B2Utility/Log.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_PlayerMirror::GSM_State_PlayerMirror()
{
	GSM_State::GSM_State();
}

void GSM_State_PlayerMirror::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	ABladeIIGameMode* GI = GameModeInstance;

	ACard* TargetCard = GI->GetArena()->PlayerField->GetLast();

	// Play bolt animation at players last field card
	EUIEffect Effect = EUIEffect::Mirror;
	GI->GetEffectLayer()->Play(Effect, nullptr, 0.25f, 0.0f);
}

void GSM_State_PlayerMirror::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

	EUIEffectEvent Event;
	while (GI->GetEffectLayer()->EventQueue.Dequeue(Event))
	{
		if (Event == EUIEffectEvent::Ready)
		{
			GI->GetDealer()->Mirror();
			GI->UpdateCardState();
		}
		else if (Event == EUIEffectEvent::Finished)
		{
			// Signal to the game mode that the turn has finished
			GI->FinishTurn();
		}
	}
}

void GSM_State_PlayerMirror::End()
{
	GSM_State::End();


}