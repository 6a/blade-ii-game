#pragma once

#include "CoreMinimal.h"

#include "GSM_State.h"

class GSM_State_PlayerTurn : public GSM_State
{
public:
	GSM_State_PlayerTurn();

	virtual void Init(class ABladeIIGameMode* GameMode) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void End() override;

	virtual EGameState Type() const override { return EGameState::PlayerTurn; }

private:

	/* The time at which this turn will end, and the player will be forced to forfeit (used for AI games only) */
	float TurnEndTime;

	/* Whether or not the player has run out of time for their turn (used for AI games only) */
	bool bTimedOut;

	/* Update the hand to display the currently selected card */
	virtual void UpdateCursorPosition(uint32 NewCursorIndex, bool bIsBlastSelecting = false) override;
};

