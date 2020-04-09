#include "B2Engine/GameStateMachine/GSM_State_DrawToEmptyField.h"

#include "B2Misc/Utility.h"

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
	if (GI->GetArena()->PlayerField->Count() + GI->GetArena()->OpponentField->Count() > 0)
	{
		GI->GetDealer()->ClearField();
	}
	else
	{
		FVector NewSelectorPosition = GI->GetArena()->PlayerDeck->GetTransformForIndex(GI->GetArena()->PlayerDeck->Count() - 1).Position;

		GI->GetCursor()->SetActorLocationAndRotation(NewSelectorPosition, FRotator::ZeroRotator);
		GI->GetCursor()->ToggleActorVisibility(true);
		GI->GetGameState()->bAcceptPlayerInput = true;
		GI->GetGameState()->CursorPosition = ECardSlot::PlayerDeck;
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
			if (GI->GetGameState()->CursorPosition == ECardSlot::PlayerDeck)
			{
				// Early exit if we try to navigate off from the deck - shouldnt be able to do that
				if ((Button == EInput::NavigateLeft || Button == EInput::NavigateRight))
				{
					continue;
				}

				switch (Button)
				{
				case EInput::Menu:
					// Handle menu open / close etc
					break;
				case EInput::Select:
					GI->GetCursor()->ToggleActorVisibility(false);

					// From player deck to player field
					UCardSlot* CurrentSlot = GI->GetCardSlot(ECardSlot::PlayerDeck);
					UCardSlot* TargetSlot = GI->GetArena()->PlayerField;

					GI->GetDealer()->Move(CurrentSlot, GI->GetArena()->PlayerDeck->Count() - 1, TargetSlot, ARC_ON_DRAW_FROM_DECK, false);

					// From opponent deck to opponent field
					CurrentSlot = GI->GetCardSlot(ECardSlot::OpponentDeck);
					TargetSlot = GI->GetArena()->OpponentField;

					GI->GetDealer()->Move(CurrentSlot, GI->GetArena()->OpponentDeck->Count() - 1, TargetSlot, ARC_ON_DRAW_FROM_DECK);

					GI->GetGameState()->bAcceptPlayerInput = false;
					break;
				}
			}
		}
	}
}

void GSM_State_DrawToEmptyField::End()
{
	GSM_State::End();


}
