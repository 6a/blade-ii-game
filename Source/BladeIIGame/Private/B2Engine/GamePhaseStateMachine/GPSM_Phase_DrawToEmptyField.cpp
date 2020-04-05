#include "B2Engine/GamePhaseStateMachine/GPSM_Phase_DrawToEmptyField.h"

#include "B2Misc/Utility.h"

#include "../BladeIIGameGameMode.h"

GPSM_Phase_DrawToEmptyField::GPSM_Phase_DrawToEmptyField()
{
	GPSM_Phase::GPSM_Phase();
}

void GPSM_Phase_DrawToEmptyField::Init(ABladeIIGameGameMode* GameMode)
{
	GPSM_Phase::Init(GameMode);
}

void GPSM_Phase_DrawToEmptyField::Tick(float DeltaSeconds)
{
	GPSM_Phase::Tick(DeltaSeconds);

	if (GameModeInstance->GetGameState()->bAcceptPlayerInput)
	{
		if (GameModeInstance->GetCursor()->IsVisible())
		{
			EButton Button;
			while (GameModeInstance->GetLocalPlayerInput()->ButtonInputQueue.Dequeue(Button))
			{
				if (GameModeInstance->GetGameState()->CursorPosition == ECardSlot::PlayerDeck)
				{
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

						GameModeInstance->GetDealer()->MoveFromDeck(CurrentSlot, GameModeInstance->GetArena()->PlayerDeck->Size() - 1, TargetSlot, false);
						GameModeInstance->GetGameState()->Cards.PlayerField.Push(GameModeInstance->GetGameState()->Cards.PlayerDeck.Pop());

						// From opponent deck to opponent field
						CurrentSlot = GameModeInstance->GetCardSlot(ECardSlot::OpponentDeck);
						TargetSlot = GameModeInstance->GetArena()->OpponentField;

						GameModeInstance->GetDealer()->MoveFromDeck(CurrentSlot, GameModeInstance->GetArena()->OpponentDeck->Size() - 1, TargetSlot);
						GameModeInstance->GetGameState()->Cards.OpponentField.Push(GameModeInstance->GetGameState()->Cards.OpponentDeck.Pop());
						
						GameModeInstance->GetGameState()->bAcceptPlayerInput = false;
						break;
					}
				}
			}
		}
	}
}

void GPSM_Phase_DrawToEmptyField::End()
{
	GPSM_Phase::End();


}
