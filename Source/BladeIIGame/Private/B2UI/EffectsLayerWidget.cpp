#include "B2UI/EffectsLayerWidget.h"

void UEffectsLayerWidget::Initialise()
{
    Blast->OnEffectFinished.AddDynamic(this, &UEffectsLayerWidget::HandleEffectFinishedEvent);
}

void UEffectsLayerWidget::Play(EEffect Effect, const FVector& TargetWorldPosition, float StartDelay, float PostDelay)
{
	switch (Effect)
	{
    case EEffect::Rod:
        break;
    case EEffect::Bolt:
        break;
    case EEffect::Mirror:
        break;
    case EEffect::Blast:
        break;
    case EEffect::BlastTarget:
        break;
    case EEffect::Force:
        break;
    case EEffect::Draw:
        break;
    case EEffect::Win:
        break;
    case EEffect::Loss:
        break;
    default:
        break;
	}
}

void UEffectsLayerWidget::HandleEffectFinishedEvent()
{
    if (OnEffectFinished.IsBound()) OnEffectFinished.Broadcast();
}
