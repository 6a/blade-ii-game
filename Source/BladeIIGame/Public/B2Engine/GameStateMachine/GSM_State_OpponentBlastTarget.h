#pragma once

#include "CoreMinimal.h"

#include "GSM_State.h"

class GSM_State_OpponentBlastTarget : public GSM_State
{
public:
	GSM_State_OpponentBlastTarget();

	virtual void Init(class ABladeIIGameMode* GameMode) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void End() override;

	virtual EGameState Type() const override { return EGameState::OpponentBlastTarget; }

private:
	/* The time to wait between actions */
	const float WAIT_TIME_BETWEEN_ACTIONS = 0.4f;

	/* The card that the opponent selected */
	ACard* TargetCard;

	/* The time at which the next action (movement or selection) can occurr */
	float NextActionTime;

	/* Whether or not the target card has been selected (so we can early exit until the state ends) */
	bool bTargetCardSelected;
};

