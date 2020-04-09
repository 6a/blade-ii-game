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
	ACard* CurrentCard = GameModeInstance->GetArena()->PlayerHand->GetCardByIndex(GameModeInstance->GetGameState()->CursorSlotIndex);

	if (CurrentCard)
	{
		FB2Transform NewTransform = GameModeInstance->GetArena()->PlayerHand->GetTransformForIndex(GameModeInstance->GetGameState()->CursorSlotIndex);
		CurrentCard->SetActorLocationAndRotation(NewTransform.Position, NewTransform.Rotation);
	}
}

void GSM_State::SetCurrentCardToSelectedTransform()
{
	ACard* CurrentCard = GameModeInstance->GetArena()->PlayerHand->GetCardByIndex(GameModeInstance->GetGameState()->CursorSlotIndex);

	if (CurrentCard)
	{
		FB2Transform NewTransform = GameModeInstance->GetArena()->PlayerHand->GetTransformForIndex(GameModeInstance->GetGameState()->CursorSlotIndex);
		NewTransform.Position += GameModeInstance->GetCursor()->OFFSET_WHEN_SELECTED;

		CurrentCard->SetActorLocationAndRotation(NewTransform.Position, NewTransform.Rotation);
	}
}

void GSM_State::SetCurrentCardToPreEffectTransform()
{
}