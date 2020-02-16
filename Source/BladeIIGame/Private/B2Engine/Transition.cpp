#include "B2Engine/Transition.h"

#include "Math/UnrealMathUtility.h"

Transition::Transition(FVector StartPosition, FVector EndPosition, FRotator StartRotation, FRotator EndRotation, float Duration, float ArcRatio)
	: StartPosition(StartPosition), EndPosition(EndPosition), StartRotation(StartRotation), EndRotation(EndRotation), Duration(Duration)
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

void Transition::Tick(float DeltaTime)
{
	/* Calculate the step for this frame and increment the current alpha */
	float Step = LERP_MAX / (Duration / DeltaTime);
	CurrentAlpha = FMath::Clamp(CurrentAlpha + Step, LERP_MIN, LERP_MAX);

	/* Calculate the vertical offset */
	float VerticalAlpha = 2 * (CurrentAlpha > LERP_MAX_HALF ? (LERP_MAX - CurrentAlpha) : CurrentAlpha);
	float VerticalOffset = FMath::InterpEaseInOut(MinVerticalOffset, MaxVerticalOffset, VerticalAlpha, EASE_EXPONENT);

	/* Calculate and update the current position including the current vertical offset */
	CurrentPosition = FMath::InterpEaseOut(StartPosition, EndPosition, CurrentAlpha, EASE_EXPONENT);
	CurrentPosition.Z += VerticalOffset;

	/* Calculate and update the current rotatio */
	CurrentRotation = FMath::InterpEaseOut(StartRotation, EndRotation, CurrentAlpha, EASE_EXPONENT);
}

bool Transition::Done() const
{
	return CurrentAlpha == 1.f;
}
