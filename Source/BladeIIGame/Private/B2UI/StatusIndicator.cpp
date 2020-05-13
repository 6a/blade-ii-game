#include "B2UI/StatusIndicator.h"

#include "B2Utility/Log.h"

void UStatusIndicator::SetState(State NewState, bool bResetTime)
{
	// Don't check to see if the new state == old state, as we can use this function to reset the timer as well

	CurrentState = NewState;

	UWorld* World = GetWorld();
	if (World)
	{
		CountdownStartTime = World->GetRealTimeSeconds();
	}

	if (bResetTime)
	{
		WaitingAnimationTimer = 0;
		WaitingAnimationPhase = 0;
	}

	UWidgetAnimation* AnimationToPlay = nullptr;

	switch (NewState)
	{
	case UStatusIndicator::State::Waiting:
		AnimationToPlay = SwitchToWaitingAnim;
		break;
	case UStatusIndicator::State::PlayerTurn:
		AnimationToPlay = SwitchToPlayerTurnAnim;
		break;
	case UStatusIndicator::State::OpponentTurn:
		AnimationToPlay = SwitchToOpponentTurnAnim;
		break;
	case UStatusIndicator::State::GameOver:
		AnimationToPlay = SwitchToGameOverAnim;
		break;
	case UStatusIndicator::State::DrawACard:
		AnimationToPlay = SwitchToDrawACardAnim;
		break;
	}

	if (AnimationToPlay)
	{
		StopAllAnimations();
		PlayAnimation(AnimationToPlay);
	}
}

void UStatusIndicator::NativeOnInitialized()
{
	CurrentState = State::Waiting;
	WaitingAnimationTimer = 0;
	WaitingAnimationPhase = 0;
	CountdownStartTime = 0;
}

void UStatusIndicator::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FString TimerTextString;
	if (CurrentState == State::Waiting)
	{
		if (WaitingAnimationTimer >= WAITING_PLACEHOLDER_ANIMATION_RATE)
		{
			WaitingAnimationTimer = 0;
			WaitingAnimationPhase++;
		}
		else
		{
			WaitingAnimationTimer += InDeltaTime;
		}

		uint32 WaitingCharCount = (WaitingAnimationPhase % WAITING_PLACEHOLDER_MAX_CHARS) + 1;
		TimerTextString = FString::ChrN(WaitingCharCount, Waiting_PLACEHOLDER_CHAR);
	}
	else if (CurrentState != State::GameOver)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			float TurnTimeRemaining = FMath::Clamp(TIMER_MAX - (World->GetRealTimeSeconds() - CountdownStartTime), 0.f, TIMER_MAX);
			TimerTextString = FString::FromInt(FMath::FloorToInt(TurnTimeRemaining));

			if (TurnTimeRemaining < WARNING_ANIM_START_TIME)
			{
				if (WarningAnim && !IsAnimationPlaying(WarningAnim))
				{
					PlayAnimation(WarningAnim, 0, 0);
				}
			}
		}
	}
	else
	{
		TimerTextString = TEXT("");
	}

	if (TimerText)
	{
		// No need to check if its worth updating, as this is performed within the slate code later
		TimerText->SetText(FText::FromString(TimerTextString));
	}
}
