#include "B2Engine/Transition.h"

#include "Math/UnrealMathUtility.h"

#include "B2Game/GameSound.h"
#include "B2Utility/Log.h"

const float LERP_MIN = 0.f;
const float LERP_MAX = 1.f;
const float LERP_MAX_HALF = LERP_MAX / 2;
const float EASE_EXPONENT = 2.f;
const FVector ARC_OFFSET_MIN = FVector::ZeroVector;

// Setting static members for B2Transition
TMap<B2WaitGroup, size_t> B2Transition::WaitGroups;
B2WaitGroup B2Transition::CurrentWaitGroup = 0;
B2WaitGroup B2Transition::NextWaitGroup = 0;

B2Transition::B2Transition()
{
	CurrentAlpha = 1.f;
}

B2Transition::B2Transition(B2WaitGroup WaitGroup, B2TPosition PositionData, B2TRotation RotationData, float Duration, float Delay)
	: WaitGroup(WaitGroup), Rotation(RotationData), Translation(PositionData), Duration(Duration), RemainingDelay(Delay)
{
	CurrentAlpha = 0;

	bTransitionFinished = false;
	bHasPlayedSound = false;
	bStarted = false;

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

	CurrentPosition = PositionData.StartPosition;
	CurrentRotation = RotationData.StartRotation;
	
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
	}

	if (!bStarted && Translation.StartPosition.Y < 0)
	{
		bStarted = true;
		B2Utility::LogInfo(FString::Printf(TEXT("Animation [%s] in WG [%d] started"), *OwnerID, WaitGroup));
	}

	/* Calculate the step for this frame and increment the current alpha */
	float Step = LERP_MAX / (Duration / DeltaTime);

	// If there is some time to wait, do so, decrementing the wait time and exiting early
	if (RemainingDelay > 0)
	{
		RemainingDelay = FMath::Max(RemainingDelay - Step, 0.f);
		return;
	}

	if (Duration == 0)
	{
		Step = 1;
	}

	/* Play a card sound, the first time the animation ticks */
	if (!bHasPlayedSound)
	{
		GameSound->PlaySFX(ESFX::CardDeal);
		bHasPlayedSound = true;
	}

	/* Calculate and update the alpha value */
	CurrentAlpha = FMath::Clamp(CurrentAlpha + Step, LERP_MIN, LERP_MAX);

	/* Calculate the arc offset */
	float ArcAlpha = 2 * (CurrentAlpha > LERP_MAX_HALF ? (LERP_MAX - CurrentAlpha) : CurrentAlpha);
	FVector Arc = FMath::InterpEaseOut(ARC_OFFSET_MIN, Translation.ArcOffset, ArcAlpha, EASE_EXPONENT);

	/* Calculate and update the current position including the current arc offset */
	CurrentPosition = EaseVector(Translation.StartPosition, Translation.EndPosition, CurrentAlpha, Translation.Ease);
	CurrentPosition += Arc;

	/* Calculate and update the current rotation */
	CurrentRotation = EaseRotator(Rotation.StartRotation, Rotation.EndRotation, CurrentAlpha, Rotation.Ease);

	// Waitgroup check to see if this waitgroup is finished
	if (WaitGroup >= 0)
	{
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
				}
			}
		}
	}
	else
	{
		if (CurrentAlpha == LERP_MAX && !bTransitionFinished)
		{
			bTransitionFinished = true;
		}
	}
}

bool B2Transition::Done() const
{
	bool bWaitGroupDone = !WaitGroups.Contains(WaitGroup);
	bool Done = bWaitGroupDone && bTransitionFinished;

	return Done;
}

bool B2Transition::TransitionInactive() const
{
	return bTransitionFinished;
}

B2WaitGroup B2Transition::GetNextWaitGroup()
{
	return NextWaitGroup++;
}

B2WaitGroup B2Transition::GetCurrentWaitGroup()
{
	return CurrentWaitGroup;
}

void B2Transition::ResetStatic()
{
	WaitGroups.Empty();
	CurrentWaitGroup = 0;
	NextWaitGroup = 0;
}

void B2Transition::SetGameSoundInstance(AGameSound* GameSoundInstance)
{
	GameSound = GameSoundInstance;
}

FVector B2Transition::EaseVector(FVector Start, FVector Target, float Alpha, EEase Ease)
{
	switch (Ease)
	{
	case EEase::EaseIn:
		return FMath::InterpEaseIn(Start, Target, Alpha, EASE_EXPONENT);
	case EEase::EaseInOut:
		return FMath::InterpEaseInOut(Start, Target, Alpha, EASE_EXPONENT);
	case EEase::EaseOut:
		return FMath::InterpEaseOut(Start, Target, Alpha, EASE_EXPONENT);
	case EEase::Linear:
		return FMath::Lerp(Start, Target, Alpha);
	}

	return FVector::ZeroVector;
}

FRotator B2Transition::EaseRotator(FRotator Start, FRotator Target, float Alpha, EEase Ease)
{
	FQuat StartQuat = Start.Quaternion();
	FQuat TargetQuat = Target.Quaternion();

	return FQuat::Slerp(StartQuat, TargetQuat, Alpha).Rotator();
}



