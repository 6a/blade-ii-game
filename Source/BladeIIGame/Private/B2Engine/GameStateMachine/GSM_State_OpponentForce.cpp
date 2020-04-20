#include "B2Engine/GameStateMachine/GSM_State_OpponentForce.h"

#include "TimerManager.h"

#include "B2Utility/Log.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_OpponentForce::GSM_State_OpponentForce()
{
	GSM_State::GSM_State();
}

void GSM_State_OpponentForce::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	ABladeIIGameMode* GI = GameModeInstance;

	// Play Force animation at players next free slot
	EUIEffect Effect = EUIEffect::Force;
	FVector TargetWorldPosition = GI->GetArena()->OpponentField->GetNextTransform().Position;
	GI->GetUIEffectLayer()->Play(Effect, &TargetWorldPosition, 1.0f, 0.4f);
	
	ACard* CurrentForceCard = GI->GetArena()->OpponentHand->GetFirstOfType(ECard::Force);
	GI->GetDealer()->ForceOut(CurrentForceCard);

	CurrentForceCard->FadeOut(0.4f);
}

void GSM_State_OpponentForce::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

	EUIEffectEvent Event;
	while (GI->GetUIEffectLayer()->EventQueue.Dequeue(Event))
	{
		if (Event == EUIEffectEvent::Ready)
		{
			// Move the played force card down to the field
			ACard* UsedForceCard = GI->GetArena()->OpponentHand->RemoveFirstOfType(ECard::Force);

			// Update the card positions in the hand as we have just removed one
			GI->GetDealer()->UpdateHandPositions(EPlayer::Opponent);

			GI->GetDealer()->ForceIn(UsedForceCard);
			UsedForceCard->FadeIn(0.5f);

			GI->GetArena()->OpponentField->Add(UsedForceCard);

			GI->UpdateCardState();
			
		}
		else if (Event == EUIEffectEvent::Finished)
		{
			// Signal to the game mode that the turn has finished
			GI->EndState();
		}
	}
}

void GSM_State_OpponentForce::End()
{
	GSM_State::End();


}