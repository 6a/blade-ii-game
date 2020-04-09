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
	if (OnEffectFinished.IsBound()) OnEffectFinished.Broadcast();
}

void UEffectWidget::OnEffectReady()
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
