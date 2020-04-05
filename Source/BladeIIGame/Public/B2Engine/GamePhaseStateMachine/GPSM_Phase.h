#pragma once

#include "CoreMinimal.h"

#include "../../B2Misc/Enum.h"

class GPSM_Phase
{
public:
	GPSM_Phase();

	virtual void Init(class ABladeIIGameGameMode* GameMode);

	virtual void Tick(float DeltaSeconds);

	virtual void End();

	virtual EGamePhase GetStateType();

protected:
	UPROPERTY()
	class ABladeIIGameGameMode* GameModeInstance;
};

