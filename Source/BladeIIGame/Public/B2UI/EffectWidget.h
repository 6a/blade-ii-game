#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "EffectWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEffectFinishedDelegate);

UCLASS()
class BLADEIIGAME_API UEffectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/* Event for when this effect finished playing */
	FEffectFinishedDelegate OnEffectFinished;

	/**
	 * Play the effect.
	 * @param StartDelay - How long to wait before playing the effect
	 * @param InPostDelay - How long to wait after the effect has finished, before triggering the effect finished callback
	 */
	virtual void Play(float StartDelay, float InPostDelay);

	/**
	 * Play the effect at the specified world position
	 * @param TargetWorldPosition - The world position at which to play the effect (will be translated into screen space internally)
	 * @param StartDelay - How long to wait before playing the effect
	 * @param InPostDelay - How long to wait after the effect has finished, before triggering the effect finished callback
	 */
	virtual void Play(const FVector& TargetWorldPosition, float StartDelay, float InPostDelay);

private:
	/* The screen position to center the widget to during play */
	FVector2D TargetScreenPosition;

	/* The time (in seconds) to delay the effect finished callback */
	float PostDelay;

	/* Callback for when an animation finishes */
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

	/**
	 * Convert a world-space position to a screen (viewport) space position
	 * @param WorldPosition - The World position to convert
	 * @return The screen-space position
	 */
	FVector2D WorldToScreenPosition(FVector WorldPosition) const;

	/**
	 * Returns the center position of the screen (viewport)
	 * @return The center position
	 */
	FVector2D GetScreenCenter() const;
};
