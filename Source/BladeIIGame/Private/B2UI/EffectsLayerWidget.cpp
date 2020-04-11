#include "B2UI/EffectsLayerWidget.h"

#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"

#include "B2Utility/Log.h"

void UEffectsLayerWidget::Initialise()
{
    Blast->OnEffectEvent.AddDynamic(this, &UEffectsLayerWidget::HandleEffectEvent);
    Bolt->OnEffectEvent.AddDynamic(this, &UEffectsLayerWidget::HandleEffectEvent);
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
        Force->Play(Position, StartDelay, PostDelay);
        EffectSwitcher->SetActiveWidget(Force);
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

void UEffectsLayerWidget::HandleEffectEvent(EUIEffectEvent Event)
{
    if (OnEffectEvent.IsBound()) OnEffectEvent.Broadcast(Event);
}

FVector2D UEffectsLayerWidget::GetCenterOffset() const
{
    // All the effect widgets should be centered now so a position of [0, 0] is the default
    return FVector2D::ZeroVector;
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

    ScreenPosition.X -= ScreenWidth / 2;
    ScreenPosition.Y -= ScreenHeight / 2;
    ScreenPosition /= GetUIScale(LocalPlayerController);

    B2Utility::LogInfo(ScreenPosition.ToString());

    return ScreenPosition;
}

float UEffectsLayerWidget::GetUIScale(APlayerController *LocalPlayerController) const
{
    // Ref: https://gist.github.com/JonathanADaley/151f26b145981336371b73def45209e7

    FIntVector ViewportSize;

    LocalPlayerController->GetViewportSize(ViewportSize.X, ViewportSize.Y);

    return GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));
}