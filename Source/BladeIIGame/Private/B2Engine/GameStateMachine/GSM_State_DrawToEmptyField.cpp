#include "B2Engine/GameStateMachine/GSM_State_DrawToEmptyField.h"

#include "B2Utility/Log.h"

#include "B2GameMode/BladeIIGameMode.h"

const FVector ARC_ON_DRAW_FROM_DECK = FVector(0, 0, 12);

GSM_State_DrawToEmptyField::GSM_State_DrawToEmptyField()
{
	GSM_State::GSM_State();
}

void GSM_State_DrawToEmptyField::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	ABladeIIGameMode* GI = GameModeInstance;

	// If there are cards on the field, clear them
	if (GI->GetArena()->PlayerField->Num() + GI->GetArena()->OpponentField->Num() > 0)
	{
		GI->GetDealer()->ClearField();
	}
	else
	{
		ACardSelector* Cursor = GI->GetCursor();

		ECardSlot NewCursorSlot;
		FVector NewCursorPosition;
		FRotator NewCursorRotation;
		uint32 NewCursorSlotIndex = 0;

		if (GI->GetArena()->PlayerDeck->Num() > 0)
		{
			NewCursorSlot = ECardSlot::PlayerDeck;
			NewCursorPosition = GI->GetArena()->PlayerDeck->GetLast()->GetActorLocation();
			NewCursorRotation = GI->GetArena()->PlayerDeck->GetLast()->GetActorRotation();
			NewCursorSlotIndex = GI->GetArena()->PlayerDeck->Num() - 1;
		}
		else
		{
			NewCursorSlot = ECardSlot::PlayerHand;
			NewCursorPosition = GI->GetArena()->PlayerHand->GetFirst()->GetActorLocation() + Cursor->OFFSET_WHEN_SELECTED;
			NewCursorRotation = GI->GetArena()->PlayerHand->GetFirst()->GetActorRotation();
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
					continue;
				}

				uint32 NewCursorIndex = GI->GetGameState()->CursorSlotIndex > 0 ? GI->GetGameState()->CursorSlotIndex - 1 : GI->GetArena()->PlayerHand->Num() - 1;
				UpdateCursorPosition(NewCursorIndex);

				break;
			}
			case EInput::NavigateRight:
			{
				if (GI->GetGameState()->CursorPosition == ECardSlot::PlayerDeck)
				{
					continue;
				}

				uint32 NewCursorIndex = GI->GetGameState()->CursorSlotIndex < GI->GetArena()->PlayerHand->Num() - 1 ? GI->GetGameState()->CursorSlotIndex + 1 : 0;
				UpdateCursorPosition(NewCursorIndex);

				break;
			}
			case EInput::Menu:
				// Handle menu open / close etc
				break;
			case EInput::Select:
				GI->GetCursor()->ToggleActorVisibility(false);
				GI->GetGameState()->bAcceptPlayerInput = false;

				if (GI->GetGameState()->CursorPosition == ECardSlot::PlayerDeck)
				{
					// From player deck to player field
					UCardSlot* CurrentSlot = GI->GetArena()->PlayerDeck;
					UCardSlot* TargetSlot = GI->GetArena()->PlayerField;

					GI->GetDealer()->Move(CurrentSlot, CurrentSlot->Num() - 1, TargetSlot, ARC_ON_DRAW_FROM_DECK, false);

					// From opponent deck to opponent field
					CurrentSlot = GI->GetArena()->OpponentDeck;
					TargetSlot = GI->GetArena()->OpponentField;

					GI->GetDealer()->Move(CurrentSlot, CurrentSlot->Num() - 1, TargetSlot, ARC_ON_DRAW_FROM_DECK);
				}
				else
				{
					// From player hand to player field
					// Dont perform the move yet as we have to wait for the opponents selection as well

					ACard* SelectedCard = GetCurrentCard();
					GI->GetOpponent()->SendUpdate(static_cast<EServerUpdate>(SelectedCard->Type));

					bIsWaitingForOpponentDrawFromHand = true;
				}

				break;
			}
		}
	} 
	else if (bIsWaitingForOpponentDrawFromHand)
	{
		FB2ServerUpdate MoveUpdate;
		while (GI->GetOpponent()->MoveUpdateQueue.Dequeue(MoveUpdate))
		{
			// From player hand to player field
			UCardSlot* CurrentSlot = GI->GetArena()->PlayerHand;
			UCardSlot* TargetSlot = GI->GetArena()->PlayerField;

			GI->GetDealer()->Move(CurrentSlot, GI->GetGameState()->CursorSlotIndex, TargetSlot, ARC_ON_DRAW_FROM_DECK, false);

			// From opponent hand to opponent field
			CurrentSlot = GI->GetArena()->OpponentDeck;
			TargetSlot = GI->GetArena()->OpponentField;

			int32 SourceSlotIndex = CurrentSlot->GetFirstIndexOfType(static_cast<ECard>(static_cast<uint32>(MoveUpdate.Update) - 1));
			if (SourceSlotIndex != -1)
			{
				GI->GetDealer()->Move(CurrentSlot, SourceSlotIndex, TargetSlot, ARC_ON_DRAW_FROM_DECK);
			}
			else
			{
				B2Utility::LogWarning("Received an invalid card from the opponent when waiting for their draw from their hand");

				// Error recovery? Check the next one? End the game?
			}

			bIsWaitingForOpponentDrawFromHand = false;
		}
	}
}

void GSM_State_DrawToEmptyField::End()
{
	GSM_State::End();


}
