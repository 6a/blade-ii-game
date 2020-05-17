#include "B2Engine/GameStateMachine/GSM_State_Tutorial.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_Tutorial::GSM_State_Tutorial()
{
	GSM_State::GSM_State();
}

void GSM_State_Tutorial::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	ABladeIIGameMode* GI = GameModeInstance;

	ACardSelector* Cursor = GI->GetCursor();

	Cursor->ToggleActorVisibility(true);
	Cursor->SetActorLocationAndRotation(GI->GetArena()->PlayerDeck->GetTransformForIndex(GI->GetArena()->PlayerDeck->Num() - 1).Position, FRotator::ZeroRotator);

	GI->GetGameState()->bAcceptPlayerInput = true;
	GI->GetGameState()->CursorPosition = ECardSlot::PlayerDeck;
	GI->GetGameState()->CursorSlotIndex = 0;

	Phase = 0;
}

void GSM_State_Tutorial::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

	FB2ServerUpdate MoveUpdate;
	if (GI->GetOpponent()->MoveUpdateQueue.Dequeue(MoveUpdate))
	{
		GI->GetGameState()->bAcceptPlayerInput = true;


	}


	if (GI->GetGameState()->bAcceptPlayerInput)
	{
		EInput Button;
		if (GI->GetLocalPlayerInput()->ButtonInputQueue.Dequeue(Button))
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

				// Update the phase and the tutorial overlay at the same time.
				GI->GetUITutorialLayer()->SetPhase(++Phase);

				// If we are in the gameplay phase, end the tutorial state and transition into a normal game.
				if (Phase > LastPhase)
				{
					GI->EndTutorial();

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

				break;
			}
		}
	}
}

void GSM_State_Tutorial::End()
{
	GSM_State::End();

}

void GSM_State_Tutorial::UpdateCursorPosition(uint32 NewCursorIndex, bool bIsBlastSelecting)
{
	GSM_State::UpdateCursorPosition(NewCursorIndex);

	ABladeIIGameMode* GI = GameModeInstance;

	// Force card edge case
	ACard* CurrentCard = GetCurrentCard();
	if (CurrentCard->Type == ECard::Force && GI->GetGameState()->CursorPosition == ECardSlot::PlayerHand)
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
