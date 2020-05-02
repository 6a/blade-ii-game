#pragma once

#include "CoreMinimal.h"

#include "GSM_State.h"

class GSM_State_DrawToEmptyField : public GSM_State
{
public:
	GSM_State_DrawToEmptyField();

	virtual void Init(class ABladeIIGameMode* GameMode) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void End() override;

	virtual EGameState Type() const override { return EGameState::DrawToEmptyField; }

private:

	/* Whether or not we are waiting for the opponents draw from their hand to be received from the server pending a draw from hand to field state */
	bool bIsWaitingForOpponentDrawFromHand;

	/* Whether or not we are waiting for the opponents draw from their deck to be received from the server (Network games only) */
	bool bIsWaitingForNetworkDrawFromDeck;
};

