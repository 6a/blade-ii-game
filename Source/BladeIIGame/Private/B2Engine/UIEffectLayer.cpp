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

	if (EffectsLayerWidget) EffectsLayerWidget->OnEffectFinished.AddDynamic(this, &UB2UIEffectLayer::HandleEffectFinishedEvent);
}

void UB2UIEffectLayer::Initialise()
{
	if (EffectsLayerWidgetClass)
	{
		EffectsLayerWidget = CreateWidget<UEffectsLayerWidget>(GetWorld()->GetGameInstance(), EffectsLayerWidgetClass, TEXT("Effects Layer Widget"));
		if (EffectsLayerWidget)
		{
			EffectsLayerWidget->AddToViewport();
			EffectsLayerWidget->Initialise();
		}
	}

	// Event listeners
	EffectsLayerWidget->OnEffectFinished.AddDynamic(this, &UB2UIEffectLayer::HandleEffectFinishedEvent);
}

void UB2UIEffectLayer::Tick(float DeltaSeconds)
{

}

void UB2UIEffectLayer::Play(EEffect Effect, const FVector* TargetWorldPosition, float StartDelay, float PostDelay)
{
	EffectsLayerWidget->Play(Effect, TargetWorldPosition, StartDelay, PostDelay);
}

void UB2UIEffectLayer::HandleEffectFinishedEvent()
{
	if (OnEffectFinished.IsBound()) OnEffectFinished.Broadcast();
}
