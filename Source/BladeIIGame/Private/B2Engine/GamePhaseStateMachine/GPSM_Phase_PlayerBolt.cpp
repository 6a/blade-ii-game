#include "B2Engine/GamePhaseStateMachine/GPSM_Phase_PlayerBolt.h"

#include "TimerManager.h"

#include "B2Misc/Utility.h"

#include "B2GameMode/BladeIIGameGameMode.h"

GPSM_Phase_PlayerBolt::GPSM_Phase_PlayerBolt()
{
	GPSM_Phase::GPSM_Phase();
}

void GPSM_Phase_PlayerBolt::Init(ABladeIIGameGameMode* GameMode)
{
	GPSM_Phase::Init(GameMode);

	ABladeIIGameGameMode* GI = GameModeInstance;

	// Play bolt animation at opponents last card position
	EUIEffect Effect = EUIEffect::Bolt;
	FVector TargetWorldPosition = GI->GetArena()->OpponentField->GetLast()->GetActorLocation();
	GI->GetEffectLayer()->Play(Effect, &TargetWorldPosition, 0.f, 0.0f);

	bIsPendingFinishCall = false;
}

void GPSM_Phase_PlayerBolt::Tick(float DeltaSeconds)
{
	GPSM_Phase::Tick(DeltaSeconds);

	ABladeIIGameGameMode* GI = GameModeInstance;

	if (GI->GetGameState()->bPendingEffectRequiresAction)
	{
		// Get a reference to the cards we will be working on
		ACard* SelectedCard = GameModeInstance->GetArena()->PlayerHand->GetCardByIndex(GameModeInstance->GetGameState()->CursorSlotIndex);
		ACard* TargetCard = GameModeInstance->GetArena()->OpponentField->GetLast();

		// Set target card state to inactive
		TargetCard->SetActive(false);

		// Flip the target card
		TargetCard->AddActorWorldRotation(FRotator(180, 0, 0), false, nullptr, ETeleportType::TeleportPhysics);

		// Update score
		GI->GetGameState()->OpponentScore = GI->AggregateScore(GI->GetArena()->OpponentField);
		GI->GetArena()->ScoreDisplay->Update(GI->GetGameState()->PlayerScore, GI->GetGameState()->OpponentScore);

		TimeToCallFinishFunction = GI->GetWorld()->GetTimeSeconds() + 0.5f;
		bIsPendingFinishCall = true;
		
		// Update card slots 
		GI->GetArena()->OpponentField->RemoveByID(TargetCard->GetID());
		GI->GetArena()->OpponentDiscard->Add(TargetCard);

		GI->GetGameState()->bPendingEffectRequiresAction = false;
	}
	else if (bIsPendingFinishCall)
	{
		if (GI->GetWorld()->GetTimeSeconds() > TimeToCallFinishFunction)
		{
			// Remove the bolt card from the players hand
			ACard* SelectedCard = GameModeInstance->GetArena()->PlayerHand->GetCardByIndex(GameModeInstance->GetGameState()->CursorSlotIndex);
			SelectedCard->SetActorHiddenInGame(true);

			// Update card slots 
			GI->GetArena()->PlayerHand->RemoveByID(SelectedCard->GetID());
			GI->GetArena()->PlayerDiscard->Add(SelectedCard);

			// Signal to the game mode that the turn has finished
			GI->FinishTurn();

			bIsPendingFinishCall = false;
		}
	}
}

void GPSM_Phase_PlayerBolt::End()
{
	GPSM_Phase::End();


}