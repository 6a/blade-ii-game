#pragma once

#include "CoreMinimal.h"

#include "../../B2Enum/GameStateEnum.h"

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

	/* Returns the card currently under the cursor to its original transform */
	void SetCurrentCardToOriginalTransform();

	/* Returns the card currently under the cursor to its selected transform */
	void SetCurrentCardToSelectedTransform();

	/* Returns the card currently under the cursor to its pre effect transform */
	void SetCurrentCardToPreEffectTransform();
};

