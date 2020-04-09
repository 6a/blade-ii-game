#pragma once

#include "CoreMinimal.h"
#include "GPSM_Phase.h"

class B2GPSM
{
public:
	B2GPSM(class ABladeIIGameGameMode* GameMode);

	/**
	 * Ends the current state, and changes to the type of the template argument
	 * @param GPSM_StateType - The state to switch to
	 */
	template<class GPSM_StateType>
	void ChangeState()
	{
		if (CurrentState) CurrentState->End();

		CurrentState = new GPSM_StateType();
		CurrentState->Init(GameModeInstance);
	}

	/**
	 * Returns true if the specified state is the same as the current state
	 * @param State - The state to compare
	 */
    bool IsCurrentState(EGameState State)
	{
		return State == CurrentState->Type();
	}

	/* Progress the current state of the state machine */
	void Tick(float DeltaSeconds);

private:
	UPROPERTY()
	class ABladeIIGameGameMode* GameModeInstance;

	GPSM_Phase* CurrentState;
};
