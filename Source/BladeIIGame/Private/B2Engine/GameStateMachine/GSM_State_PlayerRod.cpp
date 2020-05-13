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
	GI->GetUIEffectLayer()->Play(Effect, &TargetWorldPosition, 0.25f, 0.0f);
	
	// Flip the card back to upright 
	GI->GetDealer()->FlipFieldCard(TargetCard, true, 0.25f);
	GI->UpdateCardState();

	// Play base sound effect
	GI->GetGameSound()->PlaySFX(ESFX::EffectRod, 0.2f);

	GI->GetCamera()->Shake(0.2f);
}

void GSM_State_PlayerRod::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

	EUIEffectEvent Event;
	while (GI->GetUIEffectLayer()->EventQueue.Dequeue(Event))
	{
		if (Event == EUIEffectEvent::Ready)
		{
			// Remove the rod card from the players hand
			ACard* SelectedCard = RemoveCurrentPlayerCard();
			SelectedCard->SetActorHiddenInGame(true);
			SelectedCard->SetActorLocation(GI->GetArena()->PlayerDiscard->GetNextTransform().Position);

			// Update card slots 
			GI->GetArena()->PlayerDiscard->Add(SelectedCard);

			// Update the card positions in the hand as we have just removed one
			GI->GetDealer()->UpdateHandPositions(EPlayer::Player);
		}
		else if (Event == EUIEffectEvent::Finished)
		{
			// Signal to the game mode that the turn has finished
			GI->EndState();
		}
	}
}

void GSM_State_PlayerRod::End()
{
	GSM_State::End();


}