#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"

#include "B2UI/EffectWidget.h"
#include "B2Enum/UIEffectEnum.h"
#include "B2Enum/UIEffectEventEnum.h"

#include "EffectsLayerWidget.generated.h"

UCLASS()
class BLADEIIGAME_API UEffectsLayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/* Event for when this effect finished playing */
	FEffectEventDelegate OnEffectEvent;

	void Initialise();

	/**
	 * Play the effect.
	 * @param Effect - The effect to play
	 * @param TargetWorldPosition - The World position at which to play the effect (will be translated into screen space internally). Set to null to default to center
	 * @param StartDelay - How long to wait before playing the effect
	 * @param PostDelay - How long to wait after the effect has finished, before triggering the effect finished callback
	 */
	void Play(EUIEffect Effect, const FVector* TargetWorldPosition = nullptr, float StartDelay = 0.f, float PostDelay = 0.f);

private:
	/* Widget (effect) switcher  */
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* EffectSwitcher;

	/* Effect widgets */
	UPROPERTY(meta = (BindWidgetOptional))
	UEffectWidget* Rod;

	/* Effect widgets */
	UPROPERTY(meta = (BindWidgetOptional))
	UEffectWidget* Bolt;

	/* Effect widgets */
	UPROPERTY(meta = (BindWidgetOptional))
	UEffectWidget* Mirror;

	/* Effect widgets */
	UPROPERTY(meta = (BindWidgetOptional))
	UEffectWidget* Blast;

	/* Effect widgets */
	UPROPERTY(meta = (BindWidgetOptional))
	UEffectWidget* BlastTarget;

	/* Effect widgets */
	UPROPERTY(meta = (BindWidgetOptional))
	UEffectWidget* Force;

	/* Endgame widgets */
	UPROPERTY(meta = (BindWidgetOptional))
	UEffectWidget* Victory;

	/* Endgame widgets */
	UPROPERTY(meta = (BindWidgetOptional))
	UEffectWidget* Draw;

	/* Endgame widgets */
	UPROPERTY(meta = (BindWidgetOptional))
	UEffectWidget* Defeat;

	/* Handle any effect finish events for the attached effect widgets */
	UFUNCTION()
	void HandleEffectEvent(EUIEffectEvent Event);

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

	/**
	 * Returns the UI scale based on the current screen resolution
	 * @param LocalPlayerController - A pointer to the local player controller
	 * @return The UI scale
	 */
	float GetUIScale(APlayerController* LocalPlayerController) const;
};
