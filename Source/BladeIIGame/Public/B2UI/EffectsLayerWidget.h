#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"

#include "EffectWidget.h"
#include "B2Misc/Enum.h"

#include "EffectsLayerWidget.generated.h"

UCLASS()
class BLADEIIGAME_API UEffectsLayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/* Event for when this effect finished playing */
	FEffectFinishedDelegate OnEffectFinished;

	void Initialise();

	/**
	 * Play the effect.
	 * @param Effect - The effect to play
	 * @param TargetWorldPosition - The World position at which to play the effect (will be translated into screen space internally). Set to null to default to center
	 * @param StartDelay - How long to wait before playing the effect
	 * @param PostDelay - How long to wait after the effect has finished, before triggering the effect finished callback
	 */
	void Play(EEffect Effect, const FVector* TargetWorldPosition = nullptr, float StartDelay = 0.f, float PostDelay = 0.f);

private:
	/* Widget (effect) switcher  */
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* EffectSwitcher;

	/* Effect widgets */
	UPROPERTY(meta = (BindWidget))
	UEffectWidget* Blast;

	/* Effect widgets */
	UPROPERTY(meta = (BindWidget))
	UEffectWidget* Bolt;

	/* Handle any effect finish events for the attached effect widgets */
	UFUNCTION()
	void HandleEffectFinishedEvent();

	/**
	 * Returns the center position of the screen (viewport)
	 * @return The center position
	 */
	FVector2D GetCenterOffset() const;

	/**
	 * Convert a world-space position to a screen (viewport) space position
	 * @param WorldPosition - The World position to convert
	 * @return The screen-space position
	 */
	FVector2D WorldToScreenOffset(FVector WorldPosition) const;
};
