#include "B2Engine/GamePhaseStateMachine/GPSM_Phase.h"

#include "B2GameMode/BladeIIGameGameMode.h"

GPSM_Phase::GPSM_Phase()
{

}

void GPSM_Phase::Init(ABladeIIGameGameMode* GameMode)
{
	GameModeInstance = GameMode;
}

void GPSM_Phase::Tick(float DeltaSeconds)
{

}

void GPSM_Phase::End()
{

}

EGameState GPSM_Phase::Type() const
{
	return EGameState::WaitingForInitialDeal;
}

void GPSM_Phase::SetCurrentCardToOriginalTransform()
{
	ACard* CurrentCard = GameModeInstance->GetArena()->PlayerHand->GetCardByIndex(GameModeInstance->GetGameState()->CursorSlotIndex);

	if (CurrentCard)
	{
		FB2Transform NewTransform = GameModeInstance->GetArena()->PlayerHand->GetTransformForIndex(GameModeInstance->GetGameState()->CursorSlotIndex);
		CurrentCard->SetActorLocationAndRotation(NewTransform.Position, NewTransform.Rotation);
	}
}

void GPSM_Phase::SetCurrentCardToSelectedTransform()
{
	ACard* CurrentCard = GameModeInstance->GetArena()->PlayerHand->GetCardByIndex(GameModeInstance->GetGameState()->CursorSlotIndex);

	if (CurrentCard)
	{
		FB2Transform NewTransform = GameModeInstance->GetArena()->PlayerHand->GetTransformForIndex(GameModeInstance->GetGameState()->CursorSlotIndex);
		NewTransform.Position += GameModeInstance->GetCursor()->OFFSET_WHEN_SELECTED;

		CurrentCard->SetActorLocationAndRotation(NewTransform.Position, NewTransform.Rotation);
	}
}

void GPSM_Phase::SetCurrentCardToPreEffectTransform()
{
}