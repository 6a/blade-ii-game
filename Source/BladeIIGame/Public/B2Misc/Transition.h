#pragma once

#include "CoreMinimal.h"

class B2Transition
{
public:
	FVector CurrentPosition;
	FRotator CurrentRotation;

	/* Default constructor to avoid having to store a pointer when declared */
	B2Transition();

	/**
	 * Start a transition.
	 * @param StartPosition - Starting position for this transition
	 * @param EndPosition - Ending position for this transition
	 * @param StartRotation - Starting rotation for this transition
	 * @param EndRotation - Ending rotation for this transition
	 * @param Duration - How long this transition should take
	 * @param ArcRatio - How high the transition should arc (vertically) as a ratio of the total distance travelled
	 * @param Delay - How long to wait before transitioning
	 */
	B2Transition(FVector StartPosition, FVector EndPosition, FRotator StartRotation, FRotator EndRotation, float Duration, float ArcRatio = 0.5f, float Delay = 0.f);

	/**
	 * Ticks this transition by 1 frame.
	 * @param DeltaTime - The current DeltaTime for this frame
	 */
	void Tick(float DeltaTime);

	/* Returns true when this transition has completed */
	bool Done() const;

private:
	FVector StartPosition;
	FVector EndPosition;

	FRotator StartRotation;
	FRotator EndRotation;

	float MinVerticalOffset;
	float MaxVerticalOffset;

	float Duration;
	float CurrentAlpha;
	float RemainingDelay;
};

