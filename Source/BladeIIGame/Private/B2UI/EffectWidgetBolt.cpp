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
