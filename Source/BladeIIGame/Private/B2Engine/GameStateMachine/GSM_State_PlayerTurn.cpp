#include "B2Engine/GameStateMachine/GSM_State_PlayerTurn.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_PlayerTurn::GSM_State_PlayerTurn()
{
	GSM_State::GSM_State();
}

void GSM_State_PlayerTurn::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	ABladeIIGameMode* GI = GameModeInstance;

	ACardSelector* Cursor = GI->GetCursor();

	GI->GetGameState()->bAcceptPlayerInput = true;
	GI->GetGameState()->CursorPosition = ECardSlot::PlayerHand;
	GI->GetGameState()->CursorSlotIndex = 0;

	FB2Transform NewSelectorTransform = GI->GetArena()->PlayerHand->GetTransformForIndex(0);
	NewSelectorTransform.Position += Cursor->OFFSET_WHEN_SELECTED;

	Cursor->SetActorLocationAndRotation(NewSelectorTransform.Position, NewSelectorTransform.Rotation);
	Cursor->ToggleActorVisibility(true);

	SetCurrentCardToSelectedTransform();

	ACard* CurrentCard = GI->GetArena()->PlayerHand->GetCardByIndex(0);
	if (CurrentCard)
	{
		if (CurrentCard->Type == ECard::Force)
		{
			GI->GetArena()->ScoreDisplay->Highlight(EPlayer::Player);
		}
	}

	uint32 CurrentPlayerScore = GI->GetGameState()->PlayerScore;
	uint32 CurrentOpponentScore = GI->GetGameState()->OpponentScore;
	GI->GetArena()->ScoreDisplay->Update(CurrentPlayerScore, CurrentOpponentScore);

	if (GI->GetSettings()->IsVersusAI())
	{
		TurnEndTime = GI->GetWorld()->GetTimeSeconds() + MAX_TURN_TIME_AI_GAME;
		bTimedOut = false;
	}
}

void GSM_State_PlayerTurn::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

	// Early exit if we are out of time
	if (GI->GetSettings()->IsVersusAI() && !bTimedOut && GI->GetWorld()->GetTimeSeconds() > TurnEndTime)
	{
		bTimedOut = true;
		GI->EndGame(EPlayer::Opponent, EWinCondition::TimeOut);
		return;
	}

	if (GI->GetGameState()->bAcceptPlayerInput)
	{
		EInput Button;
		while (GI->GetLocalPlayerInput()->ButtonInputQueue.Dequeue(Button))
		{
			ACardSelector* Cursor = GI->GetCursor();

			switch (Button)
			{
			case EInput::NavigateLeft:
			{
				uint32 NewCursorIndex = GI->GetGameState()->CursorSlotIndex > 0 ? GI->GetGameState()->CursorSlotIndex - 1 : GI->GetArena()->PlayerHand->Num() - 1;
				UpdateCursorPosition(NewCursorIndex);

				break;
			}
			case EInput::NavigateRight:
			{
				uint32 NewCursorIndex = GI->GetGameState()->CursorSlotIndex < GI->GetArena()->PlayerHand->Num() - 1 ? GI->GetGameState()->CursorSlotIndex + 1 : 0;
				UpdateCursorPosition(NewCursorIndex);

				break;
			}
			case EInput::Select:
				GI->GetCursor()->ToggleActorVisibility(false);

				ACard* Card = GetCurrentCard();

				// Depending on the type of card and/or the board state, we either place the card on the field, or execute a special card
				// Here we also check for effects that occurred, so we can use them to branch later
				bool bUsedRodEffect = (Card->Type == ECard::ElliotsOrbalStaff && GI->GetArena()->PlayerField->Num() > 0 && !GI->GetArena()->PlayerField->GetLast()->IsActive());
				bool bUsedBoltEffect = (Card->Type == ECard::Bolt && GI->GetArena()->OpponentField->Num() > 0 && GI->GetArena()->OpponentField->GetLast()->IsActive());
				bool bUsedMirrorEffect = (Card->Type == ECard::Mirror && GI->GetArena()->PlayerField->Num() > 0 && GI->GetArena()->OpponentField->Num() > 0);
				bool bUsedBlastEffect = (Card->Type == ECard::Blast && GI->GetArena()->OpponentHand->Num() > 0);
				bool bUsedForceEffect = (Card->Type == ECard::Force);
				bool bUsedNormalCard = !bUsedRodEffect && !bUsedBoltEffect && !bUsedMirrorEffect && !bUsedBlastEffect && !bUsedForceEffect;

				// If the selected card was a normal card or a force card, and the players lastest field card is flipped, remove it
				ACard* LatestFieldCard = GI->GetArena()->PlayerField->GetLast();
				if (LatestFieldCard && !LatestFieldCard->IsActive() && (bUsedForceEffect || bUsedNormalCard))
				{
					GI->GetDealer()->ClearSingleFromField(LatestFieldCard);
				}

				if (!bUsedNormalCard)
				{
					GI->GetDealer()->PlayerEffectCard(Card);
					GI->GetArena()->ScoreDisplay->Highlight(EPlayer::Undecided);
				}
				else
				{
					// From player hand to player field
					UCardSlot* CurrentSlot = GI->GetArena()->PlayerHand;
					UCardSlot* TargetSlot = GI->GetArena()->PlayerField;

					GI->GetDealer()->Move(CurrentSlot, GI->GetGameState()->CursorSlotIndex, TargetSlot, ARC_ON_MOVE);

					// Update the card positions in the hand as we have just removed one
					GI->GetDealer()->UpdateHandPositions(EPlayer::Player);
				}

				// If this was NOT a blast card, update the server - blasts are handled differently,
				// updated during the blast target state
				if (!bUsedBlastEffect)
				{
					// Update the server
					GI->GetOpponent()->SendUpdate(CardToServerUpdate(Card->Type));
				}

				// Play card selection sound effect
				GI->GetGameSound()->PlaySFX(ESFX::CursorSelect);

				GI->GetGameState()->bAcceptPlayerInput = false;

				GI->GetUIStatusIndicatorLayer()->SetState(UStatusIndicator::State::Waiting);
			}
		}
	}
}

void GSM_State_PlayerTurn::End()
{
	GSM_State::End();

}

void GSM_State_PlayerTurn::UpdateCursorPosition(uint32 NewCursorIndex, bool bIsBlastSelecting)
{
	GSM_State::UpdateCursorPosition(NewCursorIndex);

	ABladeIIGameMode* GI = GameModeInstance;

	// Force card edge case
	ACard* CurrentCard = GetCurrentCard();
	if (CurrentCard->Type == ECard::Force && GI->GetGameState()->Turn == EPlayer::Player)
	{
		// Set the force indicator
		GI->GetArena()->ScoreDisplay->Highlight(EPlayer::Player);
	}
	else
	{
		// Set the force indicator
		GI->GetArena()->ScoreDisplay->Highlight(EPlayer::Undecided);
	}
}
