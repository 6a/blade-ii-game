#pragma once

#include "CoreMinimal.h"

#include "B2Enum/GameStateEnum.h"
#include "B2Enum/ServerUpdateEnum.h"
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
	ACard* RemoveCurrentCard();

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

	EServerUpdate CardToServerMessage(ECard Card) const;

private:
	/* Min, max, and offset values for converting server messages to and from cards */
	const uint32 SERVER_MESSAGE_CARD_MIN = 1;
	const uint32 SERVER_MESSAGE_CARD_MAX = 11;
	const uint32 SERVER_MESSAGE_CARD_OFFSET = 1;
};

