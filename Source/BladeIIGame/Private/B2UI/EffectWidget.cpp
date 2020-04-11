#include "B2UI/EffectWidget.h"

#include "TimerManager.h"
#include "Components/WidgetSwitcherSlot.h"

#include "B2Utility/Log.h"

void UEffectWidget::Play(const FVector2D& InTargetScreenPosition, float StartDelay, float InPostDelay)
{
	TargetScreenPosition = InTargetScreenPosition;
	PostDelay = InPostDelay;

	if (GetWorld()->GetTimerManager().TimerExists(DelayedPlayHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(DelayedPlayHandle);
	}

	if (StartDelay > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(DelayedPlayHandle, this, &UEffectWidget::RunAnimation, StartDelay, false);
	}
	else
	{
		RunAnimation();
	}
}

void UEffectWidget::OnEffectFinishedBroadcast()
{
	if (OnEffectEvent.IsBound()) OnEffectEvent.Broadcast(EUIEffectEvent::Finished);
}

void UEffectWidget::RunAnimation()
{
	PlayAnimation(EffectAnimation);
}

void UEffectWidget::RunAnimationAtTargetScreenPosition()
{
	UWidgetSwitcherSlot* WSSlot = Cast<UWidgetSwitcherSlot>(Slot);

	float NewX = TargetScreenPosition.X / 2;
	float NewY = TargetScreenPosition.Y / 2;

	if (WSSlot)
	{
		FMargin NewMargin
		{
			NewX,		// Left
			NewY,		// Top
			-NewX,		// Right
			-NewY		// Bottom
		};

		WSSlot->SetPadding(NewMargin);
	}

	PlayAnimation(EffectAnimation);
}

void UEffectWidget::EffectReady()
{
	if (OnEffectEvent.IsBound()) OnEffectEvent.Broadcast(EUIEffectEvent::Ready);
}

void UEffectWidget::EffectFinished()
{
	if (GetWorld()->GetTimerManager().TimerExists(DelayedPlayHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(DelayedPlayHandle);
	}

	if (PostDelay > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(DelayedPlayHandle, this, &UEffectWidget::OnEffectFinishedBroadcast, PostDelay, false);
	}
	else
	{
		OnEffectFinishedBroadcast();
	}
}
