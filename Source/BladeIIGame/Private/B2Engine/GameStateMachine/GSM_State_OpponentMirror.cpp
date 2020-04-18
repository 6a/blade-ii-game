#include "B2Engine/GameStateMachine/GSM_State_OpponentMirror.h"

#include "TimerManager.h"

#include "B2Utility/Log.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_OpponentMirror::GSM_State_OpponentMirror()
{
	GSM_State::GSM_State();
}

void GSM_State_OpponentMirror::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	ABladeIIGameMode* GI = GameModeInstance;

	ACard* TargetCard = GI->GetArena()->PlayerField->GetLast();

	// Play Mirror animation at the center of the screen
	EUIEffect Effect = EUIEffect::Mirror;
	GI->GetEffectLayer()->Play(Effect, nullptr, 0.25f, 0.0f);
}

void GSM_State_OpponentMirror::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

	EUIEffectEvent Event;
	while (GI->GetEffectLayer()->EventQueue.Dequeue(Event))
	{
		if (Event == EUIEffectEvent::Ready)
		{
			GI->GetDealer()->Mirror();
			GI->UpdateCardState();
		}
		else if (Event == EUIEffectEvent::Finished)
		{
			// Remove the mirror card from the players hand
			ACard* SelectedCard = GI->GetArena()->OpponentHand->RemoveFirstOfType(ECard::Mirror);
			SelectedCard->SetActorHiddenInGame(true);
			SelectedCard->SetActorLocation(GI->GetArena()->OpponentDiscard->GetNextTransform().Position);

			// Update card slots 
			GI->GetArena()->OpponentDiscard->Add(SelectedCard);

			// Update the card positions in the hand as we have just removed one
			GI->GetDealer()->UpdateHandPositions(EPlayer::Opponent);

			// Signal to the game mode that the turn has finished
			GI->EndState();
		}
	}
}

void GSM_State_OpponentMirror::End()
{
	GSM_State::End();


}