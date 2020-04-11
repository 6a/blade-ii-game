#pragma once

#include "CoreMinimal.h"

#include "../../B2Enum/GameStateEnum.h"
#include "../../B2Game/Card.h"

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

	/* Returns the card currently under the cursor to its original transform */
	void SetCurrentCardToOriginalTransform();

	/* Returns the card currently under the cursor to its selected transform */
	void SetCurrentCardToSelectedTransform();

	/* Returns the card currently under the cursor to its pre effect transform */
	void SetCurrentCardToPreEffectTransform();

};

