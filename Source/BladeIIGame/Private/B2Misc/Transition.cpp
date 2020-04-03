#include "B2Misc/Transition.h"

#include "Math/UnrealMathUtility.h"

#include "B2Misc/Utility.h"

const float LERP_MIN = 0.f;
const float LERP_MAX = 1.f;
const float LERP_MAX_HALF = LERP_MAX / 2;
const float EASE_EXPONENT = 2.f;

// Setting static members for B2Transition
TMap<B2WaitGroup, size_t> B2Transition::WaitGroups;
B2WaitGroup B2Transition::CurrentWaitGroup = 0;
B2WaitGroup B2Transition::NextWaitGroup = 0;

B2Transition::B2Transition()
{
	CurrentAlpha = 1.f;
}

B2Transition::B2Transition(B2WaitGroup WaitGroup, FVector StartPosition, FVector EndPosition, FRotator StartRotation, FRotator EndRotation, FVector ArcOffset, EEase Ease, float Duration, float Delay)
	: WaitGroup(WaitGroup), StartPosition(StartPosition), EndPosition(EndPosition), StartRotation(StartRotation), EndRotation(EndRotation), MaxArcOffset(ArcOffset), Ease(Ease), Duration(Duration), RemainingDelay(Delay)
{
	/* Set initial values */
	CurrentPosition = StartPosition;
	CurrentRotation = StartRotation;

	MinArcOffset = FVector::ZeroVector;
	MaxArcOffset = ArcOffset;

	CurrentAlpha = 0;

	bTransitionFinished = false;

	if (WaitGroup >= 0)
	{
		if (!WaitGroups.Contains(WaitGroup))
		{
			WaitGroups.Add(WaitGroup, 1);
		}
		else
		{
			WaitGroups[WaitGroup]++;
		}
	}
}

void B2Transition::Tick(float DeltaTime)
{
	//Waitgroup check to ensure all transitions in a particular wait group start when the waitgroup is active
	if (WaitGroup >= 0)
	{
		if (WaitGroup < CurrentWaitGroup || WaitGroup > CurrentWaitGroup)
		{
			// Early exit if this transitions waitgroup is not yet in action
			return;
		}

		// If done, early exit and remove this transition from its waitgroup
		if (CurrentAlpha == LERP_MAX)
		{
			if (!bTransitionFinished)
			{
				bTransitionFinished = true;
				WaitGroups[WaitGroup]--;

				if (WaitGroups[WaitGroup] <= 0)
				{
					WaitGroups.Remove(WaitGroup);
					CurrentWaitGroup++;

					B2Utility::LogWarning(FString::Printf(TEXT("Removed group")));
				}
			}

			return;
		}
	}
	else
	{
		if (CurrentAlpha == LERP_MAX && !bTransitionFinished)
		{
			bTransitionFinished = true;
		}
	}

	/* Calculate the step for this frame and increment the current alpha */
	float Step = Duration == 0 ? 1 : LERP_MAX / (Duration / DeltaTime);

	// If there is some time to wait, do so, decrementing the wait time and exiting early
	if (RemainingDelay > 0)
	{
		RemainingDelay = FMath::Max(RemainingDelay - Step, 0.f);
		return;
	}

	/* Store a function pointer for the appropriate interpolation function */
	FVector(*EaseFunctionVector)(const FVector & A, const FVector & B, float Alpha, float Exponent) = nullptr;
	FRotator(*EaseFunctionRotator)(const FRotator & A, const FRotator & B, float Alpha, float Exponent) = nullptr;
	switch (Ease)
	{
	case EEase::EaseIn:
		EaseFunctionVector = FMath::InterpEaseIn;
		EaseFunctionRotator = FMath::InterpEaseIn;
		break;
	case EEase::EaseInOut:
		EaseFunctionVector = FMath::InterpEaseInOut;
		EaseFunctionRotator = FMath::InterpEaseInOut;
		break;
	case EEase::EaseOut:
		EaseFunctionVector = FMath::InterpEaseOut;
		EaseFunctionRotator = FMath::InterpEaseOut;
		break;
	}

	/* Calculate and apdate the alpha value */
	CurrentAlpha = FMath::Clamp(CurrentAlpha + Step, LERP_MIN, LERP_MAX);

	/* Calculate the vertical offset */
	float ArcAlpha = 2 * (CurrentAlpha > LERP_MAX_HALF ? (LERP_MAX - CurrentAlpha) : CurrentAlpha);
	FVector Arc = FMath::InterpEaseOut(MinArcOffset, MaxArcOffset, ArcAlpha, EASE_EXPONENT);

	/* Calculate and update the current position including the current arc offset */
	CurrentPosition = (*EaseFunctionVector)(StartPosition, EndPosition, CurrentAlpha, EASE_EXPONENT);
	CurrentPosition += Arc;

	/* Calculate and update the current rotation */
	CurrentRotation = (*EaseFunctionRotator)(StartRotation, EndRotation, CurrentAlpha, EASE_EXPONENT);
}

bool B2Transition::Done() const
{
	bool bWaitGroupDone = !WaitGroups.Contains(WaitGroup);
	bool Done = bWaitGroupDone && bTransitionFinished;

	return Done;
}

B2WaitGroup B2Transition::GetNextWaitGroup()
{
	B2Utility::LogWarning(FString::Printf(TEXT("Assigned waitgroup: %d"), NextWaitGroup));

	return NextWaitGroup++;
}
