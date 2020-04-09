#include "B2Engine/GamePhaseStateMachine/GPSM_Phase_DrawToEmptyField.h"

#include "B2Misc/Utility.h"

#include "B2GameMode/BladeIIGameGameMode.h"

const FVector ARC_ON_DRAW_FROM_DECK = FVector(0, 0, 12);

GPSM_Phase_DrawToEmptyField::GPSM_Phase_DrawToEmptyField()
{
	GPSM_Phase::GPSM_Phase();
}

void GPSM_Phase_DrawToEmptyField::Init(ABladeIIGameGameMode* GameMode)
{
	GPSM_Phase::Init(GameMode);

	ABladeIIGameGameMode* GI = GameModeInstance;

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

void GPSM_Phase_DrawToEmptyField::Tick(float DeltaSeconds)
{
	GPSM_Phase::Tick(DeltaSeconds);

	ABladeIIGameGameMode* GI = GameModeInstance;

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

void GPSM_Phase_DrawToEmptyField::End()
{
	GPSM_Phase::End();


}
