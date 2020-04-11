#include "B2Engine/GameStateMachine/GSM_State.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State::GSM_State()
{

}

void GSM_State::Init(ABladeIIGameMode* GameMode)
{
	GameModeInstance = GameMode;
}

void GSM_State::Tick(float DeltaSeconds)
{

}

void GSM_State::End()
{

}

EGameState GSM_State::Type() const
{
	return EGameState::WaitingForInitialDeal;
}

void GSM_State::SetCurrentCardToOriginalTransform()
{
	ABladeIIGameMode* GI = GameModeInstance;

	ACard* CurrentCard = GetCurrentCard();

	if (CurrentCard)
	{
		FB2Transform NewTransform = GI->GetArena()->PlayerHand->GetTransformForIndex(GI->GetGameState()->CursorSlotIndex);
		CurrentCard->SetActorLocationAndRotation(NewTransform.Position, NewTransform.Rotation);
	}
}

void GSM_State::SetCurrentCardToSelectedTransform()
{
	ABladeIIGameMode* GI = GameModeInstance;

	ACard* CurrentCard = GetCurrentCard();

	if (CurrentCard)
	{
		FB2Transform NewTransform = GI->GetArena()->PlayerHand->GetTransformForIndex(GI->GetGameState()->CursorSlotIndex);
		NewTransform.Position += GI->GetCursor()->OFFSET_WHEN_SELECTED;

		CurrentCard->SetActorLocationAndRotation(NewTransform.Position, NewTransform.Rotation);
	}
}

void GSM_State::SetCurrentCardToPreEffectTransform()
{
}

ACard* GSM_State::GetCurrentCard()
{
	ABladeIIGameMode* GI = GameModeInstance;

	return GI->GetArena()->PlayerHand->GetCardByIndex(GameModeInstance->GetGameState()->CursorSlotIndex);
}

ACard* GSM_State::RemoveCurrentCard()
{
	ABladeIIGameMode* GI = GameModeInstance;

	return GI->GetArena()->PlayerHand->RemoveByIndex(GameModeInstance->GetGameState()->CursorSlotIndex);
}
