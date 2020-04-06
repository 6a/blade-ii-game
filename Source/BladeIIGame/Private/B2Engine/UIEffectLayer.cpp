#include "B2Engine/UIEffectLayer.h"

#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "UObject/ConstructorHelpers.h"

#include "B2Misc/Utility.h"

const FString EFFECTS_LAYER_BLUEPRINT = "WidgetBlueprint'/Game/BladeIIGame/Blueprints/UI/BP_EffectsLayerWidget.BP_EffectsLayerWidget'";

void UB2UIEffectLayer::Initialise()
{
	ConstructorHelpers::FClassFinder<UEffectsLayerWidget> ClassFinder(*EFFECTS_LAYER_BLUEPRINT);

	if (ClassFinder.Succeeded())
	{
		EffectsLayerWidgetClass = ClassFinder.Class;
	}

	//EffectsLayerWidget = CreateWidget<UEffectsLayerWidget>(this, EffectsLayerWidgetClass, TEXT("Effects Layer Widget"));
	//if (EffectsLayerWidget)
	//{
	//	EffectsLayerWidget->AddToViewport();
	//}
}

void UB2UIEffectLayer::Tick(float DeltaSeconds)
{

}
