#pragma once

#include "CoreMinimal.h"

#include "../../B2Misc/Enum/GameStateEnum.h"

class GPSM_Phase
{
public:
	GPSM_Phase();

	virtual void Init(class ABladeIIGameGameMode* GameMode);

	virtual void Tick(float DeltaSeconds);

	virtual void End();

	virtual EGameState Type() const;

protected:
	UPROPERTY()
	class ABladeIIGameGameMode* GameModeInstance;

	/* Returns the card currently under the cursor to its original transform */
	void SetCurrentCardToOriginalTransform();

	/* Returns the card currently under the cursor to its selected transform */
	void SetCurrentCardToSelectedTransform();

	/* Returns the card currently under the cursor to its pre effect transform */
	void SetCurrentCardToPreEffectTransform();
};

