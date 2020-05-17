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

	GI->GetGameState()->bAcceptPlayerInput = true;
	GI->GetGameState()->CursorPosition = ECardSlot::PlayerDeck;
	GI->GetGameState()->CursorSlotIndex = 0;
}

void GSM_State_Tutorial::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

	if (GI->GetGameState()->bAcceptPlayerInput)
	{
		FB2ServerUpdate MoveUpdate;
		if (GI->GetOpponent()->MoveUpdateQueue.Dequeue(MoveUpdate))
		{

		}

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
				GI->GetCursor()->ToggleActorVisibility(false);

				ACard* Card = GetCurrentCard();

				// Play card selection sound effect
				GI->GetGameSound()->PlaySFX(ESFX::CursorSelect);

				GI->GetGameState()->bAcceptPlayerInput = false;

				GI->GetUIStatusIndicatorLayer()->SetState(UStatusIndicator::State::Waiting);

				break;
			}
		}
	}
}

void GSM_State_Tutorial::End()
{
	GSM_State::End();

}
