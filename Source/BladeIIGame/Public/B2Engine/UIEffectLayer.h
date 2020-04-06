#pragma once

#include "CoreMinimal.h"

#include "B2UI/EffectsLayerWidget.h"

#include "UIEffectLayer.generated.h"

// TODO remove or use - leaving here so i can add some delegates later
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveReceivedDelegate, const FB2Move&, Move);

UCLASS()
class UB2UIEffectLayer : public UObject
{
	GENERATED_BODY()

public:
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
};

