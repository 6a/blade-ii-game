#include "B2Engine/GameStateMachine/GSM_State_PlayerForce.h"

#include "TimerManager.h"

#include "B2Utility/Log.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_PlayerForce::GSM_State_PlayerForce()
{
	GSM_State::GSM_State();
}

void GSM_State_PlayerForce::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	ABladeIIGameMode* GI = GameModeInstance;

	// Play Force animation at players next free slot
	EUIEffect Effect = EUIEffect::Force;
	FVector TargetWorldPosition = GI->GetArena()->PlayerField->GetNextTransform().Position;
	GI->GetEffectLayer()->Play(Effect, &TargetWorldPosition, 1.0f, 0.4f);
	
	ACard* CurrentForceCard = GetCurrentCard();
	GI->GetDealer()->ForceOut(CurrentForceCard);
	GI->GetArena()->PlayerField->Add(CurrentForceCard);
}

void GSM_State_PlayerForce::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

	EUIEffectEvent Event;
	while (GI->GetEffectLayer()->EventQueue.Dequeue(Event))
	{
		if (Event == EUIEffectEvent::Ready)
		{
			// Move the played force card down to the field
			ACard* UsedForceCard = RemoveCurrentCard();

			// Update the card positions in the hand as we have just removed one
			GI->GetDealer()->UpdateHandPositions(EPlayer::Player);

			GI->GetDealer()->ForceIn(UsedForceCard);

			GI->UpdateCardState();
			
		}
		else if (Event == EUIEffectEvent::Finished)
		{
			// Signal to the game mode that the turn has finished
			GI->FinishTurn();
		}
	}
}

void GSM_State_PlayerForce::End()
{
	GSM_State::End();


}