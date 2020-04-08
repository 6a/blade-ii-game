#include "B2Engine/GamePhaseStateMachine/GPSM_Phase_PlayerBolt.h"

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
	EEffect Effect = EEffect::Bolt;
	FVector TargetWorldPosition = GI->GetArena()->OpponentField->GetLast()->GetActorLocation();

	GI->GetEffectLayer()->Play(Effect, &TargetWorldPosition, 0.5f, 0.5f);
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
		TargetCard->AddActorLocalRotation(FRotator(0, 180, 0));

		// Remove the bolt card from the players hand
		SelectedCard->SetActorHiddenInGame(true);

		// Update score
		GI->GetGameState()->OpponentScore = GI->AggregateScore(GI->GetArena()->OpponentField);
		GI->GetArena()->ScoreDisplay->Update(GI->GetGameState()->PlayerScore, GI->GetGameState()->OpponentScore);

		// Update card slots 
		GI->GetArena()->PlayerHand->RemoveByID(SelectedCard->GetID());

		// Signal to the game mode that the turn has finished
		GI->FinishTurn();

		GI->GetGameState()->bPendingEffectRequiresAction = false;
	}
}

void GPSM_Phase_PlayerBolt::End()
{
	GPSM_Phase::End();


}
