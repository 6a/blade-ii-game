#include "B2Engine/GamePhaseStateMachine/GPSM_Phase_PlayerTurn.h"

#include "B2Misc/Utility.h"

#include "B2GameMode/BladeIIGameGameMode.h"

GPSM_Phase_PlayerTurn::GPSM_Phase_PlayerTurn()
{
	GPSM_Phase::GPSM_Phase();
}

void GPSM_Phase_PlayerTurn::Init(ABladeIIGameGameMode* GameMode)
{
	GPSM_Phase::Init(GameMode);

	ACardSelector* Cursor = GameModeInstance->GetCursor();

	FB2Transform NewSelectorTransform = GameModeInstance->GetArena()->PlayerHand->GetTransformForIndex(0);
	NewSelectorTransform.Position += Cursor->OFFSET_WHEN_SELECTED;

	Cursor->SetActorLocationAndRotation(NewSelectorTransform.Position, NewSelectorTransform.Rotation);
	Cursor->ToggleActorVisibility(true);

	SetCurrentCardToSelectedTransform();

	GameModeInstance->GetGameState()->bAcceptPlayerInput = true;
	GameModeInstance->GetGameState()->CursorPosition = ECardSlot::PlayerHand;
	GameModeInstance->GetGameState()->CursorSlotIndex = 0;
}

void GPSM_Phase_PlayerTurn::Tick(float DeltaSeconds)
{
	GPSM_Phase::Tick(DeltaSeconds);

	if (GameModeInstance->GetGameState()->bAcceptPlayerInput)
	{
		EButton Button;
		while (GameModeInstance->GetLocalPlayerInput()->ButtonInputQueue.Dequeue(Button))
		{
			ACardSelector* Cursor = GameModeInstance->GetCursor();

			if (GameModeInstance->GetGameState()->CursorPosition == ECardSlot::PlayerHand)
			{
				switch (Button)
				{
				case EButton::NavigateLeft:
					if (GameModeInstance->GetGameState()->CursorSlotIndex > 0)
					{
						// Return the currently selected card to its original transform
						SetCurrentCardToOriginalTransform();

						GameModeInstance->GetGameState()->CursorSlotIndex--;

						FB2Transform TargetTransform = GameModeInstance->GetArena()->PlayerHand->GetTransformForIndex(GameModeInstance->GetGameState()->CursorSlotIndex);
						TargetTransform.Position += Cursor->OFFSET_WHEN_SELECTED;
						Cursor->SetActorLocationAndRotation(TargetTransform.Position , TargetTransform.Rotation);

						// Raise the newly selected card by the offset
						SetCurrentCardToSelectedTransform();
					}
					break;
				case EButton::NavigateRight:
					if (GameModeInstance->GetGameState()->CursorSlotIndex < GameModeInstance->GetArena()->PlayerHand->Count() - 1)
					{
						// Return the currently selected card to its original transform
						SetCurrentCardToOriginalTransform();

						GameModeInstance->GetGameState()->CursorSlotIndex++;

						FB2Transform TargetTransform = GameModeInstance->GetArena()->PlayerHand->GetTransformForIndex(GameModeInstance->GetGameState()->CursorSlotIndex);
						TargetTransform.Position += Cursor->OFFSET_WHEN_SELECTED;
						Cursor->SetActorLocationAndRotation(TargetTransform.Position, TargetTransform.Rotation);

						// Raise the newly selected card by the offset
						SetCurrentCardToSelectedTransform();
					}
					break;
				case EButton::Menu:
					// Handle menu open / close etc
					break;
				case EButton::Select:
					GameModeInstance->GetCursor()->ToggleActorVisibility(false);

					ACard* SelectedCard = GameModeInstance->GetArena()->PlayerHand->GetCardByIndex(GameModeInstance->GetGameState()->CursorSlotIndex);

					// Depending on the type of card and/or the board state, we either place the card on the field, or execute a special card
					// Here we also check for effects that occurred, so we can use them to branch later
					bool bUsedRodEffect = (SelectedCard->Type == ECard::ElliotsOrbalStaff && !GameModeInstance->GetArena()->PlayerField->GetLast()->IsActive());
					bool bUsedBoltEffect = (SelectedCard->Type == ECard::Bolt);
					bool bUsedMirrorEffect = (SelectedCard->Type == ECard::Mirror);
					bool bUsedBlastEffect = (SelectedCard->Type == ECard::Blast);
					bool bUsedForceEffect = (SelectedCard->Type == ECard::Force);

					if (bUsedRodEffect || bUsedBoltEffect || bUsedMirrorEffect || bUsedBlastEffect)
					{

					}
					else
					{

					}

					//// From player hand to player field
					//UCardSlot* CurrentSlot = GameModeInstance->GetCardSlot(ECardSlot::PlayerDeck);
					//UCardSlot* TargetSlot = GameModeInstance->GetArena()->PlayerField;

					//GameModeInstance->GetDealer()->MoveFromDeck(CurrentSlot, GameModeInstance->GetArena()->PlayerDeck->Count() - 1, TargetSlot, false);
					//GameModeInstance->GetGameState()->Cards.PlayerField.Push(GameModeInstance->GetGameState()->Cards.PlayerDeck.Pop());

					//// From opponent deck to opponent field
					//CurrentSlot = GameModeInstance->GetCardSlot(ECardSlot::OpponentDeck);
					//TargetSlot = GameModeInstance->GetArena()->OpponentField;

					//GameModeInstance->GetDealer()->MoveFromDeck(CurrentSlot, GameModeInstance->GetArena()->OpponentDeck->Count() - 1, TargetSlot);
					//GameModeInstance->GetGameState()->Cards.OpponentField.Push(GameModeInstance->GetGameState()->Cards.OpponentDeck.Pop());

					//GameModeInstance->GetGameState()->bAcceptPlayerInput = false;
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

void GPSM_Phase_PlayerTurn::SetCurrentCardToOriginalTransform()
{
	ACard* CurrentCard = GameModeInstance->GetArena()->PlayerHand->GetCardByIndex(GameModeInstance->GetGameState()->CursorSlotIndex);

	if (CurrentCard)
	{
		FB2Transform NewTransform = GameModeInstance->GetArena()->PlayerHand->GetTransformForIndex(GameModeInstance->GetGameState()->CursorSlotIndex);
		CurrentCard->SetActorLocationAndRotation(NewTransform.Position, NewTransform.Rotation);
	}
}

void GPSM_Phase_PlayerTurn::SetCurrentCardToSelectedTransform()
{
	ACard* CurrentCard = GameModeInstance->GetArena()->PlayerHand->GetCardByIndex(GameModeInstance->GetGameState()->CursorSlotIndex);

	if (CurrentCard)
	{
		FB2Transform NewTransform = GameModeInstance->GetArena()->PlayerHand->GetTransformForIndex(GameModeInstance->GetGameState()->CursorSlotIndex);
		NewTransform.Position += GameModeInstance->GetCursor()->OFFSET_WHEN_SELECTED;

		CurrentCard->SetActorLocationAndRotation(NewTransform.Position, NewTransform.Rotation);
	}
}

void GPSM_Phase_PlayerTurn::SetCurrentCardToPreEffectTransform()
{
}
