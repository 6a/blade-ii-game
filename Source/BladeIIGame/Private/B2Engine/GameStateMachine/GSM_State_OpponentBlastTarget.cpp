#include "B2Engine/GameStateMachine/GSM_State_OpponentBlastTarget.h"

#include "TimerManager.h"

#include "B2Utility/Log.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_OpponentBlastTarget::GSM_State_OpponentBlastTarget()
{
	GSM_State::GSM_State();
}

void GSM_State_OpponentBlastTarget::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	ABladeIIGameMode* GI = GameModeInstance;

	GI->GetGameState()->CursorPosition = ECardSlot::PlayerHand;
	GI->GetGameState()->CursorSlotIndex = 0;

	ACardSelector* Cursor = GI->GetCursor();

	SetCurrentCardToSelectedTransform();
	UpdateCursorPosition(0, true);
	Cursor->ToggleActorVisibility(true);

	TargetCard = GI->GetArena()->PlayerHand->GetRandomOfType(GI->GetGameState()->MostRecentBlastedCard);
	NextActionTime = GI->GetWorld()->GetTimeSeconds() + WAIT_TIME_BETWEEN_ACTIONS;
	bTargetCardSelected = false;

	GI->GetUIStatusIndicatorLayer()->SetState(UStatusIndicator::State::OpponentTurn, false);
}

void GSM_State_OpponentBlastTarget::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

	float Time = GI->GetWorld()->GetTimeSeconds();

	if (!bTargetCardSelected && Time >= NextActionTime)
	{
		// If the match returns zero then the cursor is on the right card
		// Otherwise, we move to the right by one
		if (GetCurrentCard()->GetID().Compare(TargetCard->GetID()) == 0)
		{
			GI->GetCursor()->ToggleActorVisibility(false);

			GI->GetDealer()->OpponentEffectCard(TargetCard);

			bTargetCardSelected = true;

			// Set indicator to waiting state
			GI->GetUIStatusIndicatorLayer()->SetState(UStatusIndicator::State::Waiting);
		}
		else
		{
			// We shouldnt have to wrap, but just incase something crazy happens... We will.
			uint32 NewCursorIndex = GI->GetGameState()->CursorSlotIndex < GI->GetArena()->PlayerHand->Num() - 1 ? GI->GetGameState()->CursorSlotIndex + 1 : 0;
			UpdateCursorPosition(NewCursorIndex, true);

			NextActionTime = GI->GetWorld()->GetTimeSeconds() + WAIT_TIME_BETWEEN_ACTIONS;
		}
	}
	else if (GI->GetGameState()->bBlastAnimationPending)
	{
		GI->GetGameState()->bBlastAnimationPending = false;

		FVector TargetLocation = TargetCard->GetActorLocation();
		GI->GetUIEffectLayer()->Play(EUIEffect::BlastTarget, &TargetLocation, 1, 0);

		// Play blast sound effect
		GI->GetGameSound()->PlaySFX(ESFX::EffectBlastSelect, 1.05f);
	}
	else
	{
		EUIEffectEvent Event;
		while (GI->GetUIEffectLayer()->EventQueue.Dequeue(Event))
		{
			if (Event == EUIEffectEvent::Ready)
			{
				// Remove the target card from its slot
				GI->GetArena()->PlayerHand->RemoveByID(TargetCard->GetID());

				// Get a reference to the target card (should be the one that is currently selected
				TargetCard->SetActorHiddenInGame(true);

				// Update card slots 
				GI->GetArena()->PlayerDiscard->Add(TargetCard);
			}
			else if (Event == EUIEffectEvent::Finished)
			{
				// Remove the blast card (and reset the 
				ACard* BlastCard = GI->GetArena()->OpponentHand->RemoveByID(GI->GetGameState()->MostRecentBlastCardID);
				BlastCard->SetActorHiddenInGame(true);

				GI->GetArena()->PrintOpponentCards();
				
				GI->GetArena()->OpponentDiscard->Add(BlastCard);

				GI->GetDealer()->BlastCleanup(EPlayer::Player);

				// Update the card positions in the hand as we have just removed one
				GI->GetDealer()->UpdateHandPositions(EPlayer::Opponent);
			}
		}
	}
}

void GSM_State_OpponentBlastTarget::End()
{
	GSM_State::End();


}