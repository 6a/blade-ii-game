#pragma once

#include "CoreMinimal.h"

#include "GPSM_Phase.h"

class GPSM_Phase_WaitingForInitialDeal : public GPSM_Phase
{
public:
	GPSM_Phase_WaitingForInitialDeal();

	virtual void Init(class ABladeIIGameGameMode* GameMode) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void End() override;

	virtual EGamePhase GetStateType() override { return EGamePhase::WaitingForInitialDeal; }
};

