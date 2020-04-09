#pragma once

#include "CoreMinimal.h"

#include "GSM_State.h"

class GSM_State_PlayerBlast : public GSM_State
{
public:
	GSM_State_PlayerBlast();

	virtual void Init(class ABladeIIGameMode* GameMode) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void End() override;

	virtual EGameState Type() const override { return EGameState::PlayerBolt; }
};

