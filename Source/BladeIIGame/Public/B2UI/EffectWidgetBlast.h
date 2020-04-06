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
	 * Play the effect.
	 * @param StartDelay - How long to wait before playing the effect
	 * @param InPostDelay - How long to wait after the effect has finished, before triggering the effect finished callback
	 */
	virtual void Play(float StartDelay, float InPostDelay);

	/**
	 * Play the effect at the specified world position
	 * @param TargetWorldPosition - The world position at which to play the effect (will be translated into screen space internally)
	 * @param InStartDelay - How long to wait before playing the effect
	 * @param InPostDelay - How long to wait after the effect has finished, before triggering the effect finished callback
	 */
	virtual void Play(const FVector& TargetWorldPosition, float InStartDelay, float InPostDelay);
};
