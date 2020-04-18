#include "B2Engine/GameStateMachine/GSM_State_PlayerBolt.h"

#include "TimerManager.h"

#include "B2Utility/Log.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_PlayerBolt::GSM_State_PlayerBolt()
{
	GSM_State::GSM_State();
}

void GSM_State_PlayerBolt::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	ABladeIIGameMode* GI = GameModeInstance;

	// Play bolt animation at opponents last card position
	EUIEffect Effect = EUIEffect::Bolt;
	FVector TargetWorldPosition = GI->GetArena()->OpponentField->GetLast()->GetActorLocation();
	GI->GetEffectLayer()->Play(Effect, &TargetWorldPosition, 0.25f, 0.4f);
}

void GSM_State_PlayerBolt::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

	EUIEffectEvent Event;
	while (GI->GetEffectLayer()->EventQueue.Dequeue(Event))
	{
		if (Event == EUIEffectEvent::Ready)
		{
			// Get a reference to the cards we will be working on
			ACard* SelectedCard = GetCurrentPlayerCard();
			ACard* TargetCard = GameModeInstance->GetArena()->OpponentField->RemoveLast();

			// Set target card state to inactive
			GI->GetDealer()->FlipFieldCard(TargetCard, false , 0.0f);

			// Update score
			GI->GetGameState()->OpponentScore = GI->AggregateScore(GI->GetArena()->OpponentField);
			GI->GetArena()->ScoreDisplay->Update(GI->GetGameState()->PlayerScore, GI->GetGameState()->OpponentScore);

			// Update card slots 
			TargetCard->SetActorLocation(GI->GetArena()->OpponentDiscard->GetNextTransform().Position);
			GI->GetArena()->OpponentDiscard->Add(TargetCard);
		}
		else if (Event == EUIEffectEvent::Finished)
		{
			// Remove the bolt card from the players hand
			ACard* SelectedCard = RemoveCurrentPlayerCard();
			SelectedCard->SetActorHiddenInGame(true);
			SelectedCard->SetActorLocation(GI->GetArena()->PlayerDiscard->GetNextTransform().Position);

			// Update card slots 
			GI->GetArena()->PlayerDiscard->Add(SelectedCard);

			// Update the card positions in the hand as we have just removed one
			GI->GetDealer()->UpdateHandPositions(EPlayer::Player);

			// Signal to the game mode that the turn has finished
			GI->EndState();
		}
	}
}

void GSM_State_PlayerBolt::End()
{
	GSM_State::End();


}