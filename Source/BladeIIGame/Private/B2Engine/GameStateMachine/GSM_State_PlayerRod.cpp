#include "B2Engine/GameStateMachine/GSM_State_PlayerRod.h"

#include "TimerManager.h"

#include "B2Utility/Log.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_PlayerRod::GSM_State_PlayerRod()
{
	GSM_State::GSM_State();
}

void GSM_State_PlayerRod::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	ABladeIIGameMode* GI = GameModeInstance;

	ACard* TargetCard = GI->GetArena()->PlayerField->GetLast();

	// Play bolt animation at players last field card
	EUIEffect Effect = EUIEffect::Rod;
	FVector TargetWorldPosition = TargetCard->GetActorLocation();
	GI->GetEffectLayer()->Play(Effect, &TargetWorldPosition, 0.25f, 0.0f);
	
	// Flip the card back to upright 
	GI->GetDealer()->FlipFieldCard(TargetCard, true, 0.25f);
	GI->UpdateCardState();
}

void GSM_State_PlayerRod::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

	EUIEffectEvent Event;
	while (GI->GetEffectLayer()->EventQueue.Dequeue(Event))
	{
		if (Event == EUIEffectEvent::Ready)
		{
			// Get a reference to the rod that was just used
			ACard* SelectedCard = RemoveCurrentCard();

			// Update card slots 
			GI->GetArena()->PlayerDiscard->Add(SelectedCard);
		}
		else if (Event == EUIEffectEvent::Finished)
		{
			// Signal to the game mode that the turn has finished
			GI->FinishTurn();
		}
	}
}

void GSM_State_PlayerRod::End()
{
	GSM_State::End();


}