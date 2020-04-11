#include "B2UI/EffectWidgetForce.h"

#include "TimerManager.h"
#include "Components/WidgetSwitcherSlot.h"

#include "B2Utility/Log.h"

void UEffectWidgetForce::Play(const FVector2D& InTargetScreenPosition, float StartDelay, float InPostDelay)
{
	RunAnimationCallback.BindUFunction(this, FName("RunAnimation"));

	Super::Play(InTargetScreenPosition, StartDelay, InPostDelay);
}

void UEffectWidgetForce::RunAnimation()
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

	B2Utility::LogWarning("FFFF");

	PlayAnimation(EffectAnimation);
}
