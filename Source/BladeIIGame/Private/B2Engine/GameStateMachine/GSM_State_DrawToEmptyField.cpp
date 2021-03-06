#include "B2Engine/GameStateMachine/GSM_State_DrawToEmptyField.h"

#include "B2Utility/Log.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_DrawToEmptyField::GSM_State_DrawToEmptyField()
{
	GSM_State::GSM_State();
}

void GSM_State_DrawToEmptyField::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	bIsWaitingForOpponentDrawFromHand = false;
	bIsWaitingForNetworkDrawFromDeck = false;

	ABladeIIGameMode* GI = GameModeInstance;

	// If there are cards on the field, clear them
	if (GI->GetArena()->PlayerField->Num() + GI->GetArena()->OpponentField->Num() > 0)
	{
		GI->GetDealer()->ClearField();

		GI->GetUIStatusIndicatorLayer()->SetState(UStatusIndicator::State::Waiting);
	}
	else
	{
		ACardSelector* Cursor = GI->GetCursor();

		ECardSlot NewCursorSlot;
		uint32 NewCursorSlotIndex = 0;

		if (GI->GetArena()->PlayerDeck->Num() > 0)
		{
			NewCursorSlot = ECardSlot::PlayerDeck;
			NewCursorSlotIndex = GI->GetArena()->PlayerDeck->Num() - 1;
		}
		else
		{
			NewCursorSlot = ECardSlot::PlayerHand;
			NewCursorSlotIndex = 0;
		}

		Cursor->ToggleActorVisibility(true);
		GI->GetGameState()->bAcceptPlayerInput = true;
		GI->GetGameState()->CursorPosition = NewCursorSlot;
		GI->GetGameState()->CursorSlotIndex = NewCursorSlotIndex;
		
		if (NewCursorSlot == ECardSlot::PlayerHand)
		{
			UpdateCursorPosition(NewCursorSlotIndex);
		}
		else
		{
			Cursor->SetActorLocationAndRotation(GI->GetArena()->PlayerDeck->GetTransformForIndex(GI->GetArena()->PlayerDeck->Num() - 1).Position, FRotator::ZeroRotator);
		}

		GI->GetUIAvatarLayer()->SetOpponentMessage(EOpponentMessage::Draw, GI->GetOpponentAvatar()->GetCurrentCharacterName());
		GI->GetOpponentAvatar()->AnimateMouth();

		GI->GetUIStatusIndicatorLayer()->SetState(UStatusIndicator::State::DrawACard);

		SetIsTimed();
	}
}

void GSM_State_DrawToEmptyField::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

	if (GI->GetGameState()->bAcceptPlayerInput)
	{
		EInput Button;
		while (GI->GetLocalPlayerInput()->ButtonInputQueue.Dequeue(Button))
		{
			switch (Button)
			{
			case EInput::NavigateLeft:
			{
				if (GI->GetGameState()->CursorPosition == ECardSlot::PlayerDeck)
				{
					break;
				}

				uint32 NewCursorIndex = GI->GetGameState()->CursorSlotIndex > 0 ? GI->GetGameState()->CursorSlotIndex - 1 : GI->GetArena()->PlayerHand->Num() - 1;
				UpdateCursorPosition(NewCursorIndex);

				break;
			}
			case EInput::NavigateRight:
			{
				if (GI->GetGameState()->CursorPosition == ECardSlot::PlayerDeck)
				{
					break;
				}

				uint32 NewCursorIndex = GI->GetGameState()->CursorSlotIndex < GI->GetArena()->PlayerHand->Num() - 1 ? GI->GetGameState()->CursorSlotIndex + 1 : 0;
				UpdateCursorPosition(NewCursorIndex);

				break;
			}
			case EInput::Select:
				GI->GetCursor()->ToggleActorVisibility(false);
				GI->GetGameState()->bAcceptPlayerInput = false;

				if (GI->GetGameState()->CursorPosition == ECardSlot::PlayerDeck)
				{
					// If vs network opponent, send update to server
					if (GI->GetSettings()->IsVersusAI())
					{
						// From player deck to player field
						UCardSlot* CurrentSlot = GI->GetArena()->PlayerDeck;
						UCardSlot* TargetSlot = GI->GetArena()->PlayerField;

						GI->GetDealer()->Move(CurrentSlot, CurrentSlot->Num() - 1, TargetSlot, ARC_ON_DRAW_FROM_DECK);

						// From opponent deck to opponent field
						CurrentSlot = GI->GetArena()->OpponentDeck;
						TargetSlot = GI->GetArena()->OpponentField;

						GI->GetDealer()->Move(CurrentSlot, CurrentSlot->Num() - 1, TargetSlot, ARC_ON_DRAW_FROM_DECK, false);

						// Play base sound effect
						GI->GetGameSound()->PlaySFX(ESFX::CursorNavigate);
					}
					else
					{
						ACard* SelectedCard = GI->GetArena()->PlayerDeck->GetLast();
						GI->GetOpponent()->SendUpdate(CardToServerUpdate(SelectedCard->Type));

						bIsWaitingForNetworkDrawFromDeck = true;
					}
				}
				else
				{
					// From player hand to player field
					// Dont perform the move yet as we have to wait for the opponents selection as well

					ACard* SelectedCard = GetCurrentCard();
					GI->GetOpponent()->SendUpdate(CardToServerUpdate(SelectedCard->Type));

					bIsWaitingForOpponentDrawFromHand = true;
				}

				GI->GetUIStatusIndicatorLayer()->SetState(UStatusIndicator::State::OpponentTurn, false);

				break;
			}
		}
	} 
	else if (bIsWaitingForOpponentDrawFromHand)
	{
		FB2ServerUpdate MoveUpdate;
		if (GI->GetOpponent()->MoveUpdateQueue.Dequeue(MoveUpdate))
		{
			// From player hand to player field
			UCardSlot* CurrentSlot = nullptr;
			UCardSlot* TargetSlot = GI->GetArena()->PlayerField;

			// Determine where to take the card from
			if (GI->GetGameState()->CursorPosition == ECardSlot::PlayerDeck)
			{
				CurrentSlot = GI->GetArena()->PlayerDeck;
			}
			else
			{
				CurrentSlot = GI->GetArena()->PlayerHand;
			}

			GI->GetDealer()->Move(CurrentSlot, GI->GetGameState()->CursorSlotIndex, TargetSlot, ARC_ON_DRAW_FROM_DECK, false);

			// Play base sound effect
			GI->GetGameSound()->PlaySFX(ESFX::CursorNavigate);

			// From opponent hand to opponent field
			CurrentSlot = nullptr;
			TargetSlot = GI->GetArena()->OpponentField;

			// Determine where to take the card from
			if (GI->GetArena()->OpponentDeck->Num() > 0)
			{
				CurrentSlot = GI->GetArena()->OpponentDeck;
			}
			else
			{
				CurrentSlot = GI->GetArena()->OpponentHand;
			}

			int32 SourceSlotIndex = CurrentSlot->GetFirstIndexOfType(ServerUpdateToCard(MoveUpdate.Code));
			if (SourceSlotIndex != -1)
			{
				GI->GetDealer()->Move(CurrentSlot, SourceSlotIndex, TargetSlot, ARC_ON_DRAW_FROM_DECK);
			}
			else
			{
				B2Utility::LogWarning("Received an invalid card from the opponent when waiting for their draw from their hand");

				// Error recovery? Check the next one? End the game?
			}

			// Update the card positions in the hand as we have just removed cards from each hand
			GI->GetDealer()->UpdateHandPositions(EPlayer::Player, false);
			GI->GetDealer()->UpdateHandPositions(EPlayer::Opponent);

			bIsWaitingForOpponentDrawFromHand = false;
		}
	}
	else if (bIsWaitingForNetworkDrawFromDeck)
	{
		FB2ServerUpdate MoveUpdate;
		if (GI->GetOpponent()->MoveUpdateQueue.Dequeue(MoveUpdate))
		{
			// From player deck to player field
			UCardSlot* CurrentSlot = GI->GetArena()->PlayerDeck;
			UCardSlot* TargetSlot = GI->GetArena()->PlayerField;

			GI->GetDealer()->Move(CurrentSlot, CurrentSlot->Num() - 1, TargetSlot, ARC_ON_DRAW_FROM_DECK, false);

			// From opponent deck to opponent field
			CurrentSlot = GI->GetArena()->OpponentDeck;
			TargetSlot = GI->GetArena()->OpponentField;

			GI->GetDealer()->Move(CurrentSlot, CurrentSlot->Num() - 1, TargetSlot, ARC_ON_DRAW_FROM_DECK);

			// Play base sound effect
			GI->GetGameSound()->PlaySFX(ESFX::CursorNavigate);

			bIsWaitingForNetworkDrawFromDeck = false;
		}
	}
}

void GSM_State_DrawToEmptyField::End()
{
	GSM_State::End();


}
