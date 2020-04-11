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

	/* Update the hand to display the currently selected card */
	void UpdateSelection(uint32 NewCursorIndex);
};

