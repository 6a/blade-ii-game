#include "B2Engine/GamePhaseStateMachine/GPSM_Phase_DrawToEmptyField.h"

#include "B2Misc/Utility.h"

#include "B2GameMode/BladeIIGameGameMode.h"

GPSM_Phase_DrawToEmptyField::GPSM_Phase_DrawToEmptyField()
{
	GPSM_Phase::GPSM_Phase();
}

void GPSM_Phase_DrawToEmptyField::Init(ABladeIIGameGameMode* GameMode)
{
	GPSM_Phase::Init(GameMode);

	FVector NewSelectorPosition = GameModeInstance->GetArena()->PlayerDeck->GetTransformForIndex(GameModeInstance->GetArena()->PlayerDeck->Count() - 1).Position;

	GameModeInstance->GetCursor()->SetActorLocationAndRotation(NewSelectorPosition, FRotator::ZeroRotator);
	GameModeInstance->GetCursor()->ToggleActorVisibility(true);

	GameModeInstance->GetGameState()->bAcceptPlayerInput = true;
	GameModeInstance->GetGameState()->CursorPosition = ECardSlot::PlayerDeck;
}

void GPSM_Phase_DrawToEmptyField::Tick(float DeltaSeconds)
{
	GPSM_Phase::Tick(DeltaSeconds);

	if (GameModeInstance->GetGameState()->bAcceptPlayerInput)
	{
		EButton Button;
		while (GameModeInstance->GetLocalPlayerInput()->ButtonInputQueue.Dequeue(Button))
		{
			if (GameModeInstance->GetGameState()->CursorPosition == ECardSlot::PlayerDeck)
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
					GameModeInstance->GetCursor()->ToggleActorVisibility(false);

					// From player deck to player field
					UCardSlot* CurrentSlot = GameModeInstance->GetCardSlot(ECardSlot::PlayerDeck);
					UCardSlot* TargetSlot = GameModeInstance->GetArena()->PlayerField;

					GameModeInstance->GetDealer()->MoveFromDeck(CurrentSlot, GameModeInstance->GetArena()->PlayerDeck->Count() - 1, TargetSlot, false);
					GameModeInstance->GetGameState()->Cards.PlayerField.Push(GameModeInstance->GetGameState()->Cards.PlayerDeck.Pop());

					// From opponent deck to opponent field
					CurrentSlot = GameModeInstance->GetCardSlot(ECardSlot::OpponentDeck);
					TargetSlot = GameModeInstance->GetArena()->OpponentField;

					GameModeInstance->GetDealer()->MoveFromDeck(CurrentSlot, GameModeInstance->GetArena()->OpponentDeck->Count() - 1, TargetSlot);
					GameModeInstance->GetGameState()->Cards.OpponentField.Push(GameModeInstance->GetGameState()->Cards.OpponentDeck.Pop());

					GameModeInstance->GetGameState()->bAcceptPlayerInput = false;
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
