#pragma once

#include "CoreMinimal.h"

#include "GSM_State.h"

class GSM_State_Tutorial : public GSM_State
{
public:
	GSM_State_Tutorial();

	virtual void Init(class ABladeIIGameMode* GameMode) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void End() override;

	virtual EGameState Type() const override { return EGameState::Tutorial; }

private:

	/* The last phase, after which we end the tutorial and should enter into a normal game */
	const uint32 LastPhase = 22;

	/* Current phase for the tutorial */
	uint32 Phase;

	/* Update the hand to display the currently selected card */
	virtual void UpdateCursorPosition(uint32 NewCursorIndex, bool bIsBlastSelecting = false) override;
};

