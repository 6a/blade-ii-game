#include "B2Engine/UIEffectLayer.h"

#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "UObject/ConstructorHelpers.h"

#include "B2Misc/Utility.h"

const FString EFFECTS_LAYER_BLUEPRINT = "WidgetBlueprint'/Game/BladeIIGame/Blueprints/UI/BP_EffectsLayerWidget'";

UB2UIEffectLayer::UB2UIEffectLayer()
{
	ConstructorHelpers::FClassFinder<UEffectsLayerWidget> ClassFinder(*EFFECTS_LAYER_BLUEPRINT);
	if (ClassFinder.Succeeded())
	{
		EffectsLayerWidgetClass = ClassFinder.Class;
	}
}

void UB2UIEffectLayer::Initialise()
{
	if (EffectsLayerWidgetClass)
	{
		EffectsLayerWidget = CreateWidget<UEffectsLayerWidget>(GetWorld()->GetGameInstance(), EffectsLayerWidgetClass, TEXT("Effects Layer Widget"));
		if (EffectsLayerWidget)
		{
			EffectsLayerWidget->AddToViewport();
		}
	}

	// TODO finish implementation, add test stuff to widget so we can actually see it etc.
}

void UB2UIEffectLayer::Tick(float DeltaSeconds)
{

}
