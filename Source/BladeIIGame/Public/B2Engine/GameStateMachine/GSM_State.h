#pragma once

#include "CoreMinimal.h"

#include "B2Enum/GameStateEnum.h"
#include "B2Enum/ServerUpdateEnum.h"
#include "B2Enum/OpponentMessageEnum.h"
#include "B2Game/Card.h"

class GSM_State
{
public:
	GSM_State();

	virtual void Init(class ABladeIIGameMode* GameMode);

	virtual void Tick(float DeltaSeconds);

	virtual void End();

	virtual EGameState Type() const;

protected:
	/* Const values for moving cards around */
	const FVector ARC_ON_MOVE = FVector(0, 0, 4);
	const FVector ARC_ON_DRAW_FROM_DECK = FVector(0, 0, 12);

	UPROPERTY()
	class ABladeIIGameMode* GameModeInstance;

	/**
	 * Get a pointer to the currently selected card
	 * @return A pointer to the card
	 */
	ACard* GetCurrentCard();

	/**
	 * Remove the current card from its slot
	 * @return A pointer to the card
	 */
	ACard* RemoveCurrentPlayerCard();

	/**
	 * Returns the card currently under the cursor to its original transform
	 * @param bIsBlastSelecting - Whether or not the target cards are raised due to the game being in a blast select state
	 */
	void SetCurrentCardToOriginalTransform(bool bIsBlastSelecting = false);

	/* Sets the card currently under the cursor to its selected transform */
	void SetCurrentCardToSelectedTransform();

	/* Returns the card currently under the cursor to its pre effect transform */
	void SetCurrentCardToPreEffectTransform();

	virtual void UpdateCursorPosition(uint32 NewCursorIndex, bool bIsBlastSelecting = false);

	/* Returns the corresponding server update enum for the specified card */
	EServerUpdate CardToServerUpdate(ECard Card) const;

	/* Returns the corresponding card for the specified server update */
	ECard ServerUpdateToCard(EServerUpdate Update) const;

	/* Returns the value of the specified if it wasnt bolted */
	uint32 GetBoltedCardRealValue(ECard Card) const;

	/* Requests a message callout with a message of the specified type + mouth animation for the opponent avatar */
	void OpponentMessage(EOpponentMessage MessageType) const;

private:
	/* The value to remove from a bolted card (non effect) to determine its non flipped type */
	const uint32 BOLTED_CARD_OFFSET = 11;

	/* Min, max, and offset values for converting server messages to and from cards */
	const uint32 SERVER_MESSAGE_CARD_MIN = 0;
	const uint32 SERVER_MESSAGE_CARD_MAX = 10;
	const uint32 SERVER_MESSAGE_CARD_UPDATE_MIN = 1;
	const uint32 SERVER_MESSAGE_CARD_UPDATE_MAX = 11;
	const uint32 SERVER_MESSAGE_CARD_OFFSET = 1;
};

