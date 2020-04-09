#pragma once

#include "CoreMinimal.h"

#include "B2Enum/EaseEnum.h"

typedef int64 B2WaitGroup;
const B2WaitGroup B2WaitGroupNone = -1;

struct B2TRotation
{
	FRotator StartRotation;
	FRotator EndRotation;
	EEase Ease;
};

struct B2TPosition
{
	FVector StartPosition;
	FVector EndPosition;
	FVector ArcOffset;
	EEase Ease;
};

class B2Transition
{
public:
	FVector CurrentPosition;
	FRotator CurrentRotation;
	B2WaitGroup WaitGroup;

	/* Default constructor to avoid having to store a pointer when declared */
	B2Transition();

	/**
	 * Start a transition.
	 * @param WaitGroup - The wait group for this transition (it will not start until the wait group is active, and will not finish until the waitgroup has been closed)
	 * @param Position - Translation data for this transition
	 * @param Rotation - Rotation data for this transition
	 * @param Duration - How long this transition should take
	 * @param Delay - How long to wait before transitioning
	 */
	B2Transition(B2WaitGroup WaitGroup, B2TPosition Position, B2TRotation Rotation, float Duration = 0.5f, float Delay = 0.f);

	/**
	 * Ticks this transition by 1 frame.
	 * @param DeltaTime - The current DeltaTime for this frame
	 */
	void Tick(float DeltaTime);

	/* Returns true when this transition has completed */
	bool Done() const;

	/* Returns the next wait group */
	static B2WaitGroup GetNextWaitGroup();

	/* Returns the current wait group */
	static B2WaitGroup GetCurrentWaitGroup();

	/* Reset all the static counters such as current and next wait group  */
	static void ResetStatic();

private:
	B2TRotation Rotation;
	B2TPosition Translation;

	float Duration;
	float CurrentAlpha;
	float RemainingDelay;

	// Waitgroups
	bool bTransitionFinished;

	static TMap<B2WaitGroup, size_t> WaitGroups;
	static B2WaitGroup CurrentWaitGroup;
	static B2WaitGroup NextWaitGroup;

	/**
	 * Eases between two vectors using the specified easing method
	 * @param Start - Starting Vector
	 * @param Target - Target Vector
	 * @param Alpha - Current alpha value
	 * @param Ease - Easing method
	 * @return The eased vector
	 */
	FVector EaseVector(FVector Start, FVector Target, float Alpha, EEase Ease);

	/**
	 * Eases between two rotators using the specified easing method
	 * @param Start - Starting rotator
	 * @param Target - Target rotator
	 * @param Alpha - Current alpha value
	 * @param Ease - Easing method
	 * @return The eased rotator
	 */
	FRotator EaseRotator(FRotator Start, FRotator Target, float Alpha, EEase Ease);
};

