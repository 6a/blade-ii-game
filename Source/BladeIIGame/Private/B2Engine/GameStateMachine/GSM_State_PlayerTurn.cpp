#include "B2Engine/GameStateMachine/GSM_State_PlayerTurn.h"

#include "B2GameMode/BladeIIGameMode.h"

const FVector ARC_ON_MOVE = FVector(0, 0, 4);

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
}

void GSM_State_PlayerTurn::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

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
			case EInput::Menu:
				// Handle menu open / close etc
				break;
			case EInput::Select:
				GI->GetCursor()->ToggleActorVisibility(false);

				ACard* SelectedCard = GetCurrentCard();

				// Depending on the type of card and/or the board state, we either place the card on the field, or execute a special card
				// Here we also check for effects that occurred, so we can use them to branch later
				bool bUsedRodEffect = (SelectedCard->Type == ECard::ElliotsOrbalStaff && !GI->GetArena()->PlayerField->GetLast()->IsActive());
				bool bUsedBoltEffect = (SelectedCard->Type == ECard::Bolt);
				bool bUsedMirrorEffect = (SelectedCard->Type == ECard::Mirror);
				bool bUsedBlastEffect = (SelectedCard->Type == ECard::Blast);
				bool bUsedForceEffect = (SelectedCard->Type == ECard::Force);

				if (bUsedRodEffect || bUsedBoltEffect || bUsedMirrorEffect || bUsedBlastEffect || bUsedForceEffect)
				{
					GI->GetDealer()->PlayerEffectCard(SelectedCard);
					GI->GetArena()->ScoreDisplay->Highlight(EPlayer::Undecided);
				}
				else
				{
					// From player hand to player field
					UCardSlot* CurrentSlot = GI->GetArena()->PlayerHand;
					UCardSlot* TargetSlot = GI->GetArena()->PlayerField;

					// If the fields most recent card is face down, remove it (if we didnt use an effect card the card is now void)
					ACard* LatestFieldCard = GI->GetArena()->PlayerField->GetLast();
					if (!LatestFieldCard->IsActive())
					{
						GI->GetDealer()->ClearSingleFromField(LatestFieldCard);
					}

					GI->GetDealer()->Move(CurrentSlot, GI->GetGameState()->CursorSlotIndex, TargetSlot, ARC_ON_MOVE);

					GI->FinishTurn();
				}

				// If this was NOT a blast card, update the server - blasts are handled differently,
				// updated during the blast target state
				if (!bUsedBlastEffect)
				{
					// Update the server
					GI->GetOpponent()->SendUpdate(static_cast<EServerUpdate>(SelectedCard->Type));
				}

				GI->GetGameState()->bAcceptPlayerInput = false;
				break;
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
	if (CurrentCard->Type == ECard::Force)
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
