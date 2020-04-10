#include "B2UI/EffectWidget.h"

#include "TimerManager.h"

#include "B2Utility/Log.h"

void UEffectWidget::Play(const FVector2D& InTargetScreenPosition, float StartDelay, float InPostDelay)
{
	TargetScreenPosition = InTargetScreenPosition;
	PostDelay = InPostDelay;
}

void UEffectWidget::OnEffectFinishedBroadcast()
{
	if (OnEffectEvent.IsBound()) OnEffectEvent.Broadcast(EUIEffectEvent::Finished);
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
