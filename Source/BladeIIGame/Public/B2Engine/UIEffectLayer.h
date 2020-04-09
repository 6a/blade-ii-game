#pragma once

#include "CoreMinimal.h"

#include "B2UI/EffectsLayerWidget.h"

#include "UIEffectLayer.generated.h"

UCLASS()
class UB2UIEffectLayer : public UObject
{
	GENERATED_BODY()

public:
	/* Event for when this effect finished playing */
	FEffectFinishedDelegate OnEffectFinished;

	UB2UIEffectLayer();

	/* Initialise this instance and attach the effects layer UI to the HUD */
	void Initialise();

	/**
	 * Tick this UI effect layer instance.
	 * @param DeltaSeconds - DeltaSeconds for the previous frame.
	 */
	virtual void Tick(float DeltaSeconds);

	/**
	 * Play the effect.
	 * @param Effect - The effect to play
	 * @param TargetWorldPosition - The World position at which to play the effect (will be translated into screen space internally). Set to null to default to center
	 * @param StartDelay - How long to wait before playing the effect
	 * @param PostDelay - How long to wait after the effect has finished, before triggering the effect finished callback
	 */
	void Play(EUIEffect Effect, const FVector* TargetWorldPosition = nullptr, float StartDelay = 0.f, float PostDelay = 0.f);

private:
	TSubclassOf<UEffectsLayerWidget> EffectsLayerWidgetClass;

	UPROPERTY()
	UEffectsLayerWidget* EffectsLayerWidget;

	UFUNCTION()
	void HandleEffectFinishedEvent();
};

