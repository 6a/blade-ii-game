#include "B2UI/EffectWidgetForce.h"

#include "TimerManager.h"

#include "B2Utility/Log.h"

void UEffectWidgetForce::Play(const FVector2D& InTargetScreenPosition, float StartDelay, float InPostDelay)
{
	Super::Play(InTargetScreenPosition, StartDelay, InPostDelay);
}

void UEffectWidgetForce::RunAnimation()
{
	RunAnimationAtTargetScreenPosition();
}
