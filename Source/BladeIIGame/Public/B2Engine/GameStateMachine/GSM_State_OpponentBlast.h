#pragma once

#include "CoreMinimal.h"

#include "GSM_State.h"

class GSM_State_OpponentBlast : public GSM_State
{
public:
	GSM_State_OpponentBlast();

	virtual void Init(class ABladeIIGameMode* GameMode) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void End() override;

	virtual EGameState Type() const override { return EGameState::OpponentBlast; }
};

