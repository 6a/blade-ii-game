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

private:
	TSubclassOf<UEffectsLayerWidget> EffectsLayerWidgetClass;

	UPROPERTY()
	UEffectsLayerWidget* EffectsLayerWidget;

	UFUNCTION()
	void HandleEffectFinished();
};

