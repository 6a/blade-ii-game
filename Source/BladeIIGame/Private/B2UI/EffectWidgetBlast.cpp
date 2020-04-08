#include "B2UI/EffectWidgetBlast.h"

#include "TimerManager.h"

void UEffectWidgetBlast::Play(const FVector2D& InTargetScreenPosition, float StartDelay, float InPostDelay)
{
	Super::Play(InTargetScreenPosition, StartDelay, InPostDelay);

	if (GetWorld()->GetTimerManager().TimerExists(DelayedPlayHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(DelayedPlayHandle);
	}

	if (StartDelay > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(DelayedPlayHandle, this, &UEffectWidgetBlast::RunAnimation, StartDelay, false);
	}
	else
	{
		RunAnimation();
	}
}

void UEffectWidgetBlast::RunAnimation()
{
	PlayAnimation(EffectAnimation);
}
