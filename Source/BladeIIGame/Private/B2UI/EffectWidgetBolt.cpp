#include "B2UI/EffectWidgetBolt.h"

#include "TimerManager.h"

#include "B2Utility/Log.h"

void UEffectWidgetBolt::Play(const FVector2D& InTargetScreenPosition, float StartDelay, float InPostDelay)
{
	Super::Play(InTargetScreenPosition, StartDelay, InPostDelay);

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


#include "Components/WidgetSwitcherSlot.h"

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
