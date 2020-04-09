#pragma once

#include "CoreMinimal.h"
#include "GSM_State.h"

class B2GameStateMachine
{
public:
	B2GameStateMachine(class ABladeIIGameMode* GameMode);

	/**
	 * Ends the current state, and changes to the type of the template argument
	 * @param GSM_State - The state to switch to
	 */
	template<class GSM_State>
	void ChangeState()
	{
		if (CurrentState) CurrentState->End();

		CurrentState = new GSM_State();
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
	class ABladeIIGameMode* GameModeInstance;

	GSM_State* CurrentState;
};
