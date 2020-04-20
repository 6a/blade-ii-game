#pragma once

#include "CoreMinimal.h"

#include "B2Engine/ServerUpdate.h"

#include "GSM_State.h"

class GSM_State_OpponentTurn : public GSM_State
{
public:
	GSM_State_OpponentTurn();
	virtual ~GSM_State_OpponentTurn() {}

	virtual void Init(class ABladeIIGameMode* GameMode) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void End() override;

	virtual EGameState Type() const override { return EGameState::OpponentTurn; }

private:

	/* Minimum and maximum articifial delays */
	const float AI_DELAY_MIN = 0.4f;
	const float AI_DELAY_MAX = 1.f;
	
	/* Set to true when this state has received a move update - so that we dont consume the next move */
	bool bMovedReceived;

	/* Set to true when this state has handled the move update - so that we dont handle it again */
	bool bMoveHandled;

	/* The move update that has been received, and will be processed once the execution time has been reached */
	FB2ServerUpdate CachedMove;

	/* The time at which the stored move will be executed */
	float MoveExecutionTime;
};

