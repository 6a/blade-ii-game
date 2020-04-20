#include "B2Engine/GameStateMachine/GSM_State_OpponentBolt.h"

#include "TimerManager.h"

#include "B2Utility/Log.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_OpponentBolt::GSM_State_OpponentBolt()
{
	GSM_State::GSM_State();
}

void GSM_State_OpponentBolt::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	ABladeIIGameMode* GI = GameModeInstance;

	// Play bolt animation at players last card position
	EUIEffect Effect = EUIEffect::Bolt;
	FVector TargetWorldPosition = GI->GetArena()->PlayerField->GetLast()->GetActorLocation();
	GI->GetUIEffectLayer()->Play(Effect, &TargetWorldPosition, 0.25f, 0.4f);
}

void GSM_State_OpponentBolt::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

	EUIEffectEvent Event;
	while (GI->GetUIEffectLayer()->EventQueue.Dequeue(Event))
	{
		if (Event == EUIEffectEvent::Ready)
		{
			// Get a reference to the cards we will be bolting
			ACard* TargetCard = GameModeInstance->GetArena()->PlayerField->GetLast();

			// Set target card state to inactive
			GI->GetDealer()->FlipFieldCard(TargetCard, false , 0.0f);

			// Update score
			GI->GetGameState()->PlayerScore = GI->AggregateScore(GI->GetArena()->PlayerField);
			GI->GetArena()->ScoreDisplay->Update(GI->GetGameState()->PlayerScore, GI->GetGameState()->OpponentScore);
		}
		else if (Event == EUIEffectEvent::Finished)
		{
			// Remove the bolt card from the opponents hand
			ACard* SelectedCard = GI->GetArena()->OpponentHand->RemoveFirstOfType(ECard::Bolt);
			SelectedCard->SetActorHiddenInGame(true);
			SelectedCard->SetActorLocation(GI->GetArena()->OpponentDiscard->GetNextTransform().Position);

			// Update card slots 
			GI->GetArena()->OpponentDiscard->Add(SelectedCard);

			// Update the card positions in the hand as we have just removed one
			GI->GetDealer()->UpdateHandPositions(EPlayer::Opponent);

			// Signal to the game mode that the turn has finished
			GI->EndState();
		}
	}
}

void GSM_State_OpponentBolt::End()
{
	GSM_State::End();


}