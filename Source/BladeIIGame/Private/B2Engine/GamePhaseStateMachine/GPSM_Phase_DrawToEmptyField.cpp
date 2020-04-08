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

	FVector NewSelectorPosition = GI->GetArena()->PlayerDeck->GetTransformForIndex(GI->GetArena()->PlayerDeck->Count() - 1).Position;

	GI->GetCursor()->SetActorLocationAndRotation(NewSelectorPosition, FRotator::ZeroRotator);
	GI->GetCursor()->ToggleActorVisibility(true);

	GI->GetGameState()->bAcceptPlayerInput = true;
	GI->GetGameState()->CursorPosition = ECardSlot::PlayerDeck;
	GI->GetGameState()->PlayerScore = 0;
	GI->GetGameState()->OpponentScore = 0;

	GI->GetArena()->ScoreDisplay->Update(0, 0);
}

void GPSM_Phase_DrawToEmptyField::Tick(float DeltaSeconds)
{
	GPSM_Phase::Tick(DeltaSeconds);

	ABladeIIGameGameMode* GI = GameModeInstance;

	if (GI->GetGameState()->bAcceptPlayerInput)
	{
		EButton Button;
		while (GI->GetLocalPlayerInput()->ButtonInputQueue.Dequeue(Button))
		{
			if (GI->GetGameState()->CursorPosition == ECardSlot::PlayerDeck)
			{
				// Early exit if we try to navigate off from the deck - shouldnt be able to do that
				if ((Button == EButton::NavigateLeft || Button == EButton::NavigateRight))
				{
					continue;
				}

				switch (Button)
				{
				case EButton::Menu:
					// Handle menu open / close etc
					break;
				case EButton::Select:
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
