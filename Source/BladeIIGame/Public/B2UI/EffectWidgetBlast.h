#pragma once

#include "CoreMinimal.h"

#include "B2UI/EffectWidget.h"

#include "EffectWidgetBlast.generated.h"

UCLASS()
class BLADEIIGAME_API UEffectWidgetBlast : public UEffectWidget
{
	GENERATED_BODY()

public:

	/**
	 * Play the effect at the specified world position
	 * @param InTargetScreenPosition - Target screen position at which to play the animation
	 * @param StartDelay - How long to wait before playing the effect
	 * @param InPostDelay - How long to wait after the effect has finished, before triggering the effect finished callback
	 */
	virtual void Play(const FVector2D& InTargetScreenPosition, float StartDelay, float InPostDelay);

private:

	/* Run the animation for this widget */
	virtual void RunAnimation() override;
};
