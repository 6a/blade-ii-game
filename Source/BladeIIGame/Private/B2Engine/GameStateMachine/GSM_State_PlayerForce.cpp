#include "B2Engine/GameStateMachine/GSM_State_PlayerForce.h"

#include "TimerManager.h"

#include "B2Utility/Log.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_PlayerForce::GSM_State_PlayerForce()
{
	GSM_State::GSM_State();
}

void GSM_State_PlayerForce::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	ABladeIIGameMode* GI = GameModeInstance;

	// Play Force animation at players next free slot
	EUIEffect Effect = EUIEffect::Force;
	FVector TargetWorldPosition = GI->GetArena()->PlayerField->GetNextTransform().Position;
	GI->GetUIEffectLayer()->Play(Effect, &TargetWorldPosition, 1.0f, 0.4f);
	
	ACard* CurrentForceCard = GetCurrentCard();
	GI->GetDealer()->ForceOut(CurrentForceCard);

	// Play base sound effect
	GI->GetGameSound()->PlaySFX(ESFX::EffectBase, 0.1f);

	// Play force sound effect 
	GI->GetGameSound()->PlaySFX(ESFX::EffectForce, 1.1f);

	CurrentForceCard->FadeOut(0.4f);
}

void GSM_State_PlayerForce::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

	EUIEffectEvent Event;
	while (GI->GetUIEffectLayer()->EventQueue.Dequeue(Event))
	{
		if (Event == EUIEffectEvent::Ready)
		{
			// Move the played force card down to the field
			ACard* UsedForceCard = RemoveCurrentPlayerCard();

			// Update the card positions in the hand as we have just removed one
			GI->GetDealer()->UpdateHandPositions(EPlayer::Player);

			GI->GetDealer()->ForceIn(UsedForceCard);
			UsedForceCard->FadeIn(0.5f);

			GI->GetArena()->PlayerField->Add(UsedForceCard);

			GI->UpdateCardState();
			
		}
		else if (Event == EUIEffectEvent::Finished)
		{
			// Signal to the game mode that the turn has finished
			GI->EndState();
		}
	}
}

void GSM_State_PlayerForce::End()
{
	GSM_State::End();


}