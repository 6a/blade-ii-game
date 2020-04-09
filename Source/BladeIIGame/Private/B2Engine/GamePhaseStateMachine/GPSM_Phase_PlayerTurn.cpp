#include "B2Engine/GamePhaseStateMachine/GPSM_Phase_PlayerTurn.h"

#include "B2Misc/Utility.h"

#include "B2GameMode/BladeIIGameGameMode.h"

const FVector ARC_ON_MOVE = FVector(0, 0, 4);

GPSM_Phase_PlayerTurn::GPSM_Phase_PlayerTurn()
{
	GPSM_Phase::GPSM_Phase();
}

void GPSM_Phase_PlayerTurn::Init(ABladeIIGameGameMode* GameMode)
{
	GPSM_Phase::Init(GameMode);

	ABladeIIGameGameMode* GI = GameModeInstance;

	ACardSelector* Cursor = GI->GetCursor();

	FB2Transform NewSelectorTransform = GI->GetArena()->PlayerHand->GetTransformForIndex(0);
	NewSelectorTransform.Position += Cursor->OFFSET_WHEN_SELECTED;

	Cursor->SetActorLocationAndRotation(NewSelectorTransform.Position, NewSelectorTransform.Rotation);
	Cursor->ToggleActorVisibility(true);

	SetCurrentCardToSelectedTransform();

	GI->GetGameState()->bAcceptPlayerInput = true;
	GI->GetGameState()->CursorPosition = ECardSlot::PlayerHand;
	GI->GetGameState()->CursorSlotIndex = 0;

	uint32 CurrentPlayerScore = GI->GetGameState()->PlayerScore;
	uint32 CurrentOpponentScore = GI->GetGameState()->OpponentScore;
	GI->GetArena()->ScoreDisplay->Update(CurrentPlayerScore, CurrentOpponentScore);
}

void GPSM_Phase_PlayerTurn::Tick(float DeltaSeconds)
{
	GPSM_Phase::Tick(DeltaSeconds);

	ABladeIIGameGameMode* GI = GameModeInstance;

	if (GI->GetGameState()->bAcceptPlayerInput)
	{
		EInput Button;
		while (GI->GetLocalPlayerInput()->ButtonInputQueue.Dequeue(Button))
		{
			ACardSelector* Cursor = GI->GetCursor();

			if (GI->GetGameState()->CursorPosition == ECardSlot::PlayerHand)
			{
				switch (Button)
				{
				case EInput::NavigateLeft:
				{
					uint32 NewCursorIndex = GI->GetGameState()->CursorSlotIndex > 0 ? GI->GetGameState()->CursorSlotIndex - 1 : GI->GetArena()->PlayerHand->Count() - 1;

					// Return the currently selected card to its original transform
					SetCurrentCardToOriginalTransform();

					GI->GetGameState()->CursorSlotIndex = NewCursorIndex;

					FB2Transform TargetTransform = GI->GetArena()->PlayerHand->GetTransformForIndex(GI->GetGameState()->CursorSlotIndex);
					TargetTransform.Position += Cursor->OFFSET_WHEN_SELECTED;
					Cursor->SetActorLocationAndRotation(TargetTransform.Position, TargetTransform.Rotation);

					// Raise the newly selected card by the offset
					SetCurrentCardToSelectedTransform();

					break;
				}
				case EInput::NavigateRight:
				{
					uint32 NewCursorIndex = GI->GetGameState()->CursorSlotIndex < GI->GetArena()->PlayerHand->Count() - 1 ? GI->GetGameState()->CursorSlotIndex + 1 : 0;

					// Return the currently selected card to its original transform
					SetCurrentCardToOriginalTransform();

					GI->GetGameState()->CursorSlotIndex = NewCursorIndex;

					FB2Transform TargetTransform = GI->GetArena()->PlayerHand->GetTransformForIndex(GI->GetGameState()->CursorSlotIndex);
					TargetTransform.Position += Cursor->OFFSET_WHEN_SELECTED;
					Cursor->SetActorLocationAndRotation(TargetTransform.Position, TargetTransform.Rotation);

					// Raise the newly selected card by the offset
					SetCurrentCardToSelectedTransform();

					break;
				}
				case EInput::Menu:
					// Handle menu open / close etc
					break;
				case EInput::Select:
					GI->GetCursor()->ToggleActorVisibility(false);

					ACard* SelectedCard = GI->GetArena()->PlayerHand->GetCardByIndex(GI->GetGameState()->CursorSlotIndex);

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

					}
					else
					{
						// From player hand to player field
						UCardSlot* CurrentSlot = GI->GetArena()->PlayerHand;
						UCardSlot* TargetSlot = GI->GetArena()->PlayerField;

						GI->GetDealer()->Move(CurrentSlot, GI->GetGameState()->CursorSlotIndex, TargetSlot, ARC_ON_MOVE);

					}

					if (bUsedBlastEffect)
					{
						// handle blast effects differently as they have an additional stage (card selection)
					}
					else
					{
						// Inform the opponent server that a new move was made

						// Update the engine state if required (such as ending the game)

						// Send the opponent server any instructions (such as game over)
					}

					GI->GetGameState()->bAcceptPlayerInput = false;
					break;
				}
			}
		}
	}
}

void GPSM_Phase_PlayerTurn::End()
{
	GPSM_Phase::End();


}
