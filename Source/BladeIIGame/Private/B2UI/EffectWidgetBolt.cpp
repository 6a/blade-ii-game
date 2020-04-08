#include "B2UI/EffectWidgetBolt.h"

#include "TimerManager.h"

void UEffectWidgetBolt::Play(const FVector2D& InTargetScreenPosition, float StartDelay, float InPostDelay)
{
	Super::Play(InTargetScreenPosition, StartDelay, InPostDelay);

	// Offset the target screen position as I havent been able to figure out how to center the damn pivot reeeeeee
	TargetScreenPosition.X -= (GetDesiredSize().X / 2);

	if (GetWorld()->GetTimerManager().TimerExists(DelayedPlayHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(DelayedPlayHandle);
	}

	if (StartDelay > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(DelayedPlayHandle, this, &UEffectWidgetBolt::RunAnimation, StartDelay, false);
	}
	else
	{
		RunAnimation();
	}
}

void UEffectWidgetBolt::RunAnimation()
{
	FWidgetTransform CurrentTransform(RenderTransform);
	CurrentTransform.Translation = TargetScreenPosition;
	SetRenderTransform(CurrentTransform);

	PlayAnimation(EffectAnimation);
}
