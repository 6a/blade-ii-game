#include "B2UI/EffectsLayerWidget.h"

#include "B2Misc/Utility.h"

void UEffectsLayerWidget::Initialise()
{
    Blast->OnEffectFinished.AddDynamic(this, &UEffectsLayerWidget::HandleEffectFinishedEvent);
    Bolt->OnEffectFinished.AddDynamic(this, &UEffectsLayerWidget::HandleEffectFinishedEvent);

    B2Utility::LogWarning("!");
}

void UEffectsLayerWidget::Play(EEffect Effect, const FVector* TargetWorldPosition, float StartDelay, float PostDelay)
{
    FVector2D Position = TargetWorldPosition ? WorldToScreenOffset(*TargetWorldPosition) : GetCenterOffset();

    B2Utility::LogWarning(*Position.ToString());

	switch (Effect)
	{
    case EEffect::Rod:
        break;
    case EEffect::Bolt:
        Bolt->Play(Position, StartDelay, PostDelay);
        EffectSwitcher->SetActiveWidget(Bolt);
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