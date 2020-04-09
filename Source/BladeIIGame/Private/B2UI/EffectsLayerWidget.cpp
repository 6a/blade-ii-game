#include "B2UI/EffectsLayerWidget.h"

#include "B2Utility/Log.h"

void UEffectsLayerWidget::Initialise()
{
    Blast->OnEffectFinished.AddDynamic(this, &UEffectsLayerWidget::HandleEffectFinishedEvent);
    Bolt->OnEffectFinished.AddDynamic(this, &UEffectsLayerWidget::HandleEffectFinishedEvent);
}

void UEffectsLayerWidget::Play(EUIEffect Effect, const FVector* TargetWorldPosition, float StartDelay, float PostDelay)
{
    FVector2D Position = TargetWorldPosition ? WorldToScreenOffset(*TargetWorldPosition) : GetCenterOffset();

	switch (Effect)
	{
    case EUIEffect::Rod:
        break;
    case EUIEffect::Bolt:
        Bolt->Play(Position, StartDelay, PostDelay);
        EffectSwitcher->SetActiveWidget(Bolt);
        break;
    case EUIEffect::Mirror:
        break;
    case EUIEffect::Blast:
        Blast->Play(Position, StartDelay, PostDelay);
        EffectSwitcher->SetActiveWidget(Blast);
        break;
    case EUIEffect::BlastTarget:
        break;
    case EUIEffect::Force:
        break;
    case EUIEffect::Draw:
        break;
    case EUIEffect::Win:
        break;
    case EUIEffect::Loss:
        break;
    default:
        break;
	}
}

void UEffectsLayerWidget::HandleEffectFinishedEvent()
{
    if (OnEffectFinished.IsBound()) OnEffectFinished.Broadcast();
}

FVector2D UEffectsLayerWidget::GetCenterOffset() const
{
    APlayerController* LocalPlayerController = GetWorld()->GetFirstPlayerController();

    FVector2D ScreenPosition;

    if (LocalPlayerController)
    {
        int32 ScreenWidth;
        int32 ScreenHeight;

        LocalPlayerController->GetViewportSize(ScreenWidth, ScreenHeight);

        ScreenPosition.X = ScreenWidth / 2.0f;
        ScreenPosition.Y = ScreenHeight - (ScreenHeight / 2.0f);
    }

    return ScreenPosition;
}

FVector2D UEffectsLayerWidget::WorldToScreenOffset(FVector WorldPosition) const
{
    APlayerController* LocalPlayerController = GetWorld()->GetFirstPlayerController();

    FVector2D ScreenPosition;

    if (LocalPlayerController)
    {
        LocalPlayerController->ProjectWorldLocationToScreen(WorldPosition, ScreenPosition, true);
    }

    int32 ScreenWidth;
    int32 ScreenHeight;

    LocalPlayerController->GetViewportSize(ScreenWidth, ScreenHeight);

    ScreenPosition.Y = -(ScreenHeight - ScreenPosition.Y);

    return ScreenPosition;
}