#pragma once

#include "CoreMinimal.h"

#include "GSM_State.h"

class GSM_State_OpponentTurn : public GSM_State
{
public:
	GSM_State_OpponentTurn();

	virtual void Init(class ABladeIIGameMode* GameMode) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void End() override;

	virtual EGameState Type() const override { return EGameState::WaitingForOpponentMove; }

private:
	
	/* Set to true when this state has processed a move - so that we dont process the next move until the next cycle (oponent -> player -> back to opponent) */
	bool bStale;
};

