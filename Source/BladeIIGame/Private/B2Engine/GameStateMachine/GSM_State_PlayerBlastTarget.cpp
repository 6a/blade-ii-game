#include "B2Engine/GameStateMachine/GSM_State_PlayerBlastTarget.h"

#include "TimerManager.h"

#include "B2Utility/Log.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_PlayerBlastTarget::GSM_State_PlayerBlastTarget()
{
	GSM_State::GSM_State();
}

void GSM_State_PlayerBlastTarget::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	ABladeIIGameMode* GI = GameModeInstance;

	GI->GetGameState()->bAcceptPlayerInput = true;
	GI->GetGameState()->CursorPosition = ECardSlot::OpponentHand;
	GI->GetGameState()->CursorSlotIndex = 0;

	ACardSelector* Cursor = GI->GetCursor();

	SetCurrentCardToSelectedTransform();

	UpdateCursorPosition(0, true);
	Cursor->ToggleActorVisibility(true);
}

void GSM_State_PlayerBlastTarget::Tick(float DeltaSeconds)
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
				uint32 NewCursorIndex = GI->GetGameState()->CursorSlotIndex < GI->GetArena()->OpponentHand->Num() - 1 ? GI->GetGameState()->CursorSlotIndex + 1 : 0;
				UpdateCursorPosition(NewCursorIndex, true);

				break;
			}
			case EInput::NavigateRight:
			{
				uint32 NewCursorIndex = GI->GetGameState()->CursorSlotIndex > 0 ? GI->GetGameState()->CursorSlotIndex - 1 : GI->GetArena()->OpponentHand->Num() - 1;
				UpdateCursorPosition(NewCursorIndex, true);

				break;
			}
			case EInput::Menu:
				// Handle menu open / close etc
				break;
			case EInput::Select:
				GI->GetCursor()->ToggleActorVisibility(false);

				GI->GetGameState()->bAcceptPlayerInput = false;
				break;
			}
		}
	}

}

void GSM_State_PlayerBlastTarget::End()
{
	GSM_State::End();


}