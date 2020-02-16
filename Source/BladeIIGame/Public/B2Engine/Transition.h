#pragma once

#include "CoreMinimal.h"

class Transition
{
public:
	FVector CurrentPosition;
	FRotator CurrentRotation;

	/**
	 * Start a transition.
	 * @param StartPosition - Starting position for this transition
	 * @param EndPosition - Ending position for this transition
	 * @param StartRotation - Starting rotation for this transition
	 * @param EndRotation - Ending rotation for this transition
	 * @param Duration - How long this transition should take
	 * @param ArcRatio - How high the transition should arc (vertically) as a ratio of the total distance travelled
	 */
	Transition(FVector StartPosition, FVector EndPosition, FRotator StartRotation, FRotator EndRotation, float Duration, float ArcRatio = 0.5f);

	/**
	 * Ticks this transition by 1 frame.
	 * @param DeltaTime - The current DeltaTime for this frame
	 */
	void Tick(float DeltaTime);

	/* Returns true when this transition has completed */
	bool Done() const;

private:
	const float LERP_MIN = 0.f;
	const float LERP_MAX = 1.f;
	const float LERP_MAX_HALF = LERP_MAX / 2;
	const float EASE_EXPONENT = 2.f;

	FVector StartPosition;
	FVector EndPosition;

	FRotator StartRotation;
	FRotator EndRotation;

	float MinVerticalOffset;
	float MaxVerticalOffset;

	float Duration;
	float CurrentAlpha;
};

