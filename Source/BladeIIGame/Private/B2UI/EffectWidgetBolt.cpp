#include "B2UI/EffectWidgetBolt.h"

#include "TimerManager.h"
#include "Components/WidgetSwitcherSlot.h"

#include "B2Utility/Log.h"

void UEffectWidgetBolt::Play(const FVector2D& InTargetScreenPosition, float StartDelay, float InPostDelay)
{
	Super::Play(InTargetScreenPosition, StartDelay, InPostDelay);
}

void UEffectWidgetBolt::RunAnimation()
{
	RunAnimationAtTargetScreenPosition();
}
