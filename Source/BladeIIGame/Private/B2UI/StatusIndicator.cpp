#include "B2UI/StatusIndicator.h"

#include "B2Utility/Log.h"

void UStatusIndicator::SetState(State NewState)
{
	if (NewState != CurrentState)
	{
		CurrentState = NewState;
		TurnTimeRemaining = TIMER_MAX;

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
		}

		if (AnimationToPlay)
		{
			StopAllAnimations();
			PlayAnimation(AnimationToPlay);
		}
	}
}

void UStatusIndicator::NativeOnInitialized()
{
	CurrentState = State::Waiting;
	WaitingAnimationTimer = 0;
	WaitingAnimationPhase = 0;
	TurnTimeRemaining = TIMER_MAX;
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
	else
	{
		TimerTextString = FString::FromInt(FMath::FloorToInt(TurnTimeRemaining));
		TurnTimeRemaining = FMath::Clamp(TurnTimeRemaining - InDeltaTime, 0.f, TIMER_MAX);

		if (TurnTimeRemaining < WARNING_ANIM_START_TIME)
		{
			if (WarningAnim && !IsAnimationPlaying(WarningAnim))
			{
				PlayAnimation(WarningAnim, 0, 0);
			}
		}
	}

	if (TimerText)
	{
		// No need to check if its worth updating, as this is performed within the slate code later
		TimerText->SetText(FText::FromString(TimerTextString));
	}
}
