#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "B2Enum/UIEffectEventEnum.h"

#include "EffectWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEffectEventDelegate, EUIEffectEvent, Event);

UCLASS()
class BLADEIIGAME_API UEffectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/* Event for when this effect is eithe ready or finished */
	FEffectEventDelegate OnEffectEvent;

	/**
	 * Play the effect at the specified world position
	 * @param InTargetScreenPosition - Target screen position at which to play the animation
	 * @param StartDelay - How long to wait before playing the effect
	 * @param InPostDelay - How long to wait after the effect has finished, before triggering the effect finished callback
	 */
	virtual void Play(const FVector2D& InTargetScreenPosition, float StartDelay, float InPostDelay);

protected:
	/* Timer handle for any delayed plays */
	FTimerHandle DelayedPlayHandle;

	/* Timer handle for any delayed animation completion events */
	FTimerHandle DelayedCompletionBroadcastHandle;

	/* The screen position to center the widget to during play */
	FVector2D TargetScreenPosition;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnimOptional))
	class UWidgetAnimation* EffectAnimation;

	/* The width of the widest portion of this widget. Used by some widgets to offset from the center due to shitty pivots */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 Width;

	/* Callback for when an animation is considered to be ready to active the effect */
	UFUNCTION(BlueprintCallable, Category = "Events")
	void EffectReady();

	/* Callback for when an animation is considered to be finished */
	UFUNCTION(BlueprintCallable, Category = "Events")
	void EffectFinished();

private:
	/* The time (in seconds) to delay the effect finished callback */
	float PostDelay;

	/* Helper for delaying completion events */
	UFUNCTION()
	void OnEffectFinishedBroadcast();
};
