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

void GSM_State::SetCurrentCardToOriginalTransform(bool bIsBlastSelecting)
{
	ABladeIIGameMode* GI = GameModeInstance;

	ACard* CurrentCard = GetCurrentCard();

	if (CurrentCard)
	{
		FB2Transform NewTransform;

		if (bIsBlastSelecting)
		{
			FVector YMod;

			if (GI->GetGameState()->CursorPosition == ECardSlot::PlayerHand)
			{
				YMod = FVector(1, 1, 1);
			}
			else
			{
				YMod = FVector(1, -1, 1);
			}

			NewTransform.Position = CurrentCard->GetActorLocation() - GI->GetCursor()->OFFSET_WHEN_SELECTED * YMod;
			NewTransform.Rotation = CurrentCard->GetActorRotation();
		}
		else
		{
			NewTransform = GI->GetCardSlot(GI->GetGameState()->CursorPosition)->GetTransformForIndex(GI->GetGameState()->CursorSlotIndex);
		}

		CurrentCard->SetActorLocationAndRotation(NewTransform.Position, NewTransform.Rotation);
	}
}

void GSM_State::SetCurrentCardToSelectedTransform()
{
	ABladeIIGameMode* GI = GameModeInstance;

	ACard* CurrentCard = GetCurrentCard();

	// Used to flip the Y offset if the cursor is on the opponent hand
	FVector OffsetYMod = FVector(1, GI->GetGameState()->CursorPosition == ECardSlot::OpponentHand ? -1 : 1, 1);

	if (CurrentCard)
	{
		FVector NewPosition = CurrentCard->GetActorLocation() + GI->GetCursor()->OFFSET_WHEN_SELECTED * OffsetYMod;

		CurrentCard->SetActorLocationAndRotation(NewPosition, CurrentCard->GetActorRotation());
	}
}

void GSM_State::SetCurrentCardToPreEffectTransform()
{
}

void GSM_State::UpdateCursorPosition(uint32 NewCursorIndex, bool bIsBlastSelecting)
{
	ABladeIIGameMode* GI = GameModeInstance;

	// Return the currently selected card to its original transform
	SetCurrentCardToOriginalTransform(bIsBlastSelecting);

	GI->GetGameState()->CursorSlotIndex = NewCursorIndex;

	ACard* CurrentCard = GetCurrentCard();

	// Used to flip the Y offset if the cursor is on the opponent hand
	FVector OffsetYMod = FVector(1, GI->GetGameState()->CursorPosition == ECardSlot::OpponentHand ? -1 : 1, 1);

	// Used to flip the rotation when the cursor is using the opponents side as a reference (as their cards are upside down)
	FRotator OffsetRotationMod = GI->GetGameState()->CursorPosition == ECardSlot::OpponentHand ? FRotator(180, 0, 0) : FRotator::ZeroRotator;

	FB2Transform TargetTransform;
	TargetTransform.Position = CurrentCard->GetActorLocation() + ACardSelector::OFFSET_WHEN_SELECTED * OffsetYMod;
	TargetTransform.Rotation = CurrentCard->GetActorRotation() + OffsetRotationMod;
	
	GI->GetCursor()->SetActorLocationAndRotation(TargetTransform.Position, TargetTransform.Rotation);

	// Raise the newly selected card by the offset
	SetCurrentCardToSelectedTransform();

	// Play navigation sound effect
	GI->GetGameSound()->PlaySFX(ESFX::CursorNavigate);
}

EServerUpdate GSM_State::CardToServerUpdate(ECard Card) const
{
	EServerUpdate OutUpdateType = EServerUpdate::None;

	if (static_cast<uint32>(Card) >= SERVER_MESSAGE_CARD_MIN || static_cast<uint32>(Card) <= SERVER_MESSAGE_CARD_MAX)
	{
		OutUpdateType = static_cast<EServerUpdate>(static_cast<uint32>(Card) + SERVER_MESSAGE_CARD_OFFSET);
	}

	return OutUpdateType;
}

ECard GSM_State::ServerUpdateToCard(EServerUpdate Update) const
{
	ECard OutCard = ECard::ElliotsOrbalStaff;

	if (static_cast<uint32>(Update) >= SERVER_MESSAGE_CARD_UPDATE_MIN || static_cast<uint32>(Update) <= SERVER_MESSAGE_CARD_UPDATE_MAX)
	{
		OutCard = static_cast<ECard>(static_cast<uint32>(Update) - SERVER_MESSAGE_CARD_OFFSET);
	}

	return OutCard;
}

uint32 GSM_State::GetBoltedCardRealValue(ECard Card) const
{
	int32 OutValue = 0;

	if (Card > ECard::Force)
	{
		Card = static_cast<ECard>(static_cast<uint32>(Card) - BOLTED_CARD_OFFSET);
	}

	OutValue = ACard::TypeToValue(Card);

	return OutValue;
}

void GSM_State::OpponentMessage(EOpponentMessage MessageType) const
{
	ABladeIIGameMode* GI = GameModeInstance;

	GI->GetUIAvatarLayer()->SetOpponentMessage(MessageType, GI->GetOpponentAvatar()->GetCurrentCharacterName());
	GI->GetOpponentAvatar()->AnimateMouth();
}

ACard* GSM_State::GetCurrentCard()
{
	ABladeIIGameMode* GI = GameModeInstance;

	return GI->GetCardSlot(GI->GetGameState()->CursorPosition)->GetCardByIndex(GameModeInstance->GetGameState()->CursorSlotIndex);
}

ACard* GSM_State::RemoveCurrentPlayerCard()
{
	ABladeIIGameMode* GI = GameModeInstance;

	return GI->GetCardSlot(GI->GetGameState()->CursorPosition)->RemoveByIndex(GameModeInstance->GetGameState()->CursorSlotIndex);
}
