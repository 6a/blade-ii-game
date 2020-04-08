#include "B2UI/EffectWidget.h"

#include "B2Misc/Utility.h"

void UEffectWidget::Play(const FVector2D& InTargetScreenPosition, float StartDelay, float InPostDelay)
{
	TargetScreenPosition = InTargetScreenPosition;
	PostDelay = InPostDelay;
}

void UEffectWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (OnEffectFinished.IsBound()) OnEffectFinished.Broadcast();

	B2Utility::LogInfo("EffectAnimation playback event handled");
}
