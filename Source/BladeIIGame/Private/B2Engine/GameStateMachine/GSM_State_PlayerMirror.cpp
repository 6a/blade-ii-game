#include "B2Engine/GameStateMachine/GSM_State_PlayerMirror.h"

#include "TimerManager.h"

#include "B2Utility/Log.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_PlayerMirror::GSM_State_PlayerMirror()
{
	GSM_State::GSM_State();
}

void GSM_State_PlayerMirror::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	ABladeIIGameMode* GI = GameModeInstance;

	ACard* TargetCard = GI->GetArena()->PlayerField->GetLast();

	// Play Mirror animation at the center of the screen
	EUIEffect Effect = EUIEffect::Mirror;
	GI->GetUIEffectLayer()->Play(Effect, nullptr, 0.25f, 0.0f);

	// Play base sound effect
	GI->GetGameSound()->PlaySFX(ESFX::EffectBase, 0.25f);

	GI->GetCamera()->Shake(0.25f);
}

void GSM_State_PlayerMirror::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

	EUIEffectEvent Event;
	while (GI->GetUIEffectLayer()->EventQueue.Dequeue(Event))
	{
		if (Event == EUIEffectEvent::Ready)
		{
			GI->GetDealer()->Mirror();
			GI->UpdateCardState();
		}
		else if (Event == EUIEffectEvent::Finished)
		{
			// Remove the mirror card from the players hand
			ACard* SelectedCard = RemoveCurrentPlayerCard();
			SelectedCard->SetActorHiddenInGame(true);
			SelectedCard->SetActorLocation(GI->GetArena()->PlayerDiscard->GetNextTransform().Position);

			// Update card slots 
			GI->GetArena()->PlayerDiscard->Add(SelectedCard);

			// Update the card positions in the hand as we have just removed one
			GI->GetDealer()->UpdateHandPositions(EPlayer::Player);

			// Signal to the game mode that the turn has finished
			GI->EndState();
		}
	}
}

void GSM_State_PlayerMirror::End()
{
	GSM_State::End();


}