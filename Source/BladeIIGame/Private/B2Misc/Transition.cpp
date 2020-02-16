#include "B2Misc/Transition.h"

#include "Math/UnrealMathUtility.h"
#include "..\..\Public\B2Misc\Transition.h"

const float LERP_MIN = 0.f;
const float LERP_MAX = 1.f;
const float LERP_MAX_HALF = LERP_MAX / 2;
const float EASE_EXPONENT = 2.f;

B2Transition::B2Transition()
{
	CurrentAlpha = 1.f;
}

B2Transition::B2Transition(FVector StartPosition, FVector EndPosition, FRotator StartRotation, FRotator EndRotation, float Duration, float ArcRatio, float Delay)
	: StartPosition(StartPosition), EndPosition(EndPosition), StartRotation(StartRotation), EndRotation(EndRotation), Duration(Duration), RemainingDelay(Delay)
{
	/* Set initial values */
	CurrentPosition = StartPosition;
	CurrentRotation = StartRotation;
	MinVerticalOffset = 0;
	CurrentAlpha = 0;

	/* Calculate maximum vertical offset as a ratio of the total distance that will be covered */
	float TotalDistance = FVector::Distance(StartPosition, EndPosition);
	MaxVerticalOffset = TotalDistance * ArcRatio;
}

void B2Transition::Tick(float DeltaTime)
{
	/* Calculate the step for this frame and increment the current alpha */
	float Step = LERP_MAX / (Duration / DeltaTime);

	if (RemainingDelay > 0)
	{
		RemainingDelay = FMath::Max(RemainingDelay - Step, 0.f);
		return;
	}

	CurrentAlpha = FMath::Clamp(CurrentAlpha + Step, LERP_MIN, LERP_MAX);

	/* Calculate the vertical offset */
	float VerticalAlpha = 2 * (CurrentAlpha > LERP_MAX_HALF ? (LERP_MAX - CurrentAlpha) : CurrentAlpha);
	float VerticalOffset = FMath::InterpEaseOut(MinVerticalOffset, MaxVerticalOffset, VerticalAlpha, EASE_EXPONENT);

	/* Calculate and update the current position including the current vertical offset */
	CurrentPosition = FMath::InterpEaseInOut(StartPosition, EndPosition, CurrentAlpha, EASE_EXPONENT);
	CurrentPosition.Z += VerticalOffset;

	/* Calculate and update the current rotation */
	CurrentRotation = FMath::InterpEaseOut(StartRotation, EndRotation, CurrentAlpha, EASE_EXPONENT);
}

bool B2Transition::Done() const
{
	return CurrentAlpha == 1.f;
}
