#pragma once

#include "CoreMinimal.h"

#include "GPSM_Phase.h"

class GPSM_Phase_DrawToEmptyField : public GPSM_Phase
{
public:
	GPSM_Phase_DrawToEmptyField();

	virtual void Init(class ABladeIIGameGameMode* GameMode) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void End() override;

	virtual EGamePhase GetStateType() override { return EGamePhase::DrawToEmptyField; }
};
