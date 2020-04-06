#pragma once

#include "CoreMinimal.h"

#include "GPSM_Phase.h"

class GPSM_Phase_PlayerTurn : public GPSM_Phase
{
public:
	GPSM_Phase_PlayerTurn();

	virtual void Init(class ABladeIIGameGameMode* GameMode) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void End() override;

	virtual EGamePhase GetStateType() override { return EGamePhase::PlayerTurn; }

private:
	/* Returns the card currently under the cursor to its original transform */
	void SetCurrentCardToOriginalTransform();

	/* Returns the card currently under the cursor to its selected transform */
	void SetCurrentCardToSelectedTransform();

	/* Returns the card currently under the cursor to its pre effect transform */
	void SetCurrentCardToPreEffectTransform();
};

