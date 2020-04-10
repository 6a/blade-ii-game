#include "B2Engine/UIEffectLayer.h"

#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "UObject/ConstructorHelpers.h"

#include "B2Utility/Log.h"

const FString EFFECTS_LAYER_BLUEPRINT = "WidgetBlueprint'/Game/BladeIIGame/Blueprints/UI/BP_EffectsLayer'";

UB2UIEffectLayer::UB2UIEffectLayer()
{
	ConstructorHelpers::FClassFinder<UEffectsLayerWidget> ClassFinder(*EFFECTS_LAYER_BLUEPRINT);
	if (ClassFinder.Succeeded())
	{
		EffectsLayerWidgetClass = ClassFinder.Class;
	}

	if (EffectsLayerWidget) EffectsLayerWidget->OnEffectEvent.AddDynamic(this, &UB2UIEffectLayer::HandleEffectEvent);
}

void UB2UIEffectLayer::Initialise()
{
	if (EffectsLayerWidgetClass)
	{
		EffectsLayerWidget = CreateWidget<UEffectsLayerWidget>(GetWorld()->GetGameInstance(), EffectsLayerWidgetClass, TEXT("UI Effects Layer"));
		if (EffectsLayerWidget)
		{
			EffectsLayerWidget->AddToViewport();
			EffectsLayerWidget->Initialise();
		}
	}

	// Event listeners
	EffectsLayerWidget->OnEffectEvent.AddDynamic(this, &UB2UIEffectLayer::HandleEffectEvent);
}

void UB2UIEffectLayer::Tick(float DeltaSeconds)
{

}

void UB2UIEffectLayer::Play(EUIEffect Effect, const FVector* TargetWorldPosition, float StartDelay, float PostDelay)
{
	EffectsLayerWidget->Play(Effect, TargetWorldPosition, StartDelay, PostDelay);
}

void UB2UIEffectLayer::HandleEffectEvent(EUIEffectEvent Event)
{
	EventQueue.Enqueue(Event);
}
