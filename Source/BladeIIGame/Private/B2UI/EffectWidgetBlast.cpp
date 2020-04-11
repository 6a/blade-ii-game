#include "B2UI/EffectWidgetBlast.h"

#include "TimerManager.h"

void UEffectWidgetBlast::Play(const FVector2D& InTargetScreenPosition, float StartDelay, float InPostDelay)
{
	RunAnimationCallback.BindUFunction(this, FName("RunAnimation"));

	Super::Play(InTargetScreenPosition, StartDelay, InPostDelay);
}

void UEffectWidgetBlast::RunAnimation()
{
	PlayAnimation(EffectAnimation);
}
