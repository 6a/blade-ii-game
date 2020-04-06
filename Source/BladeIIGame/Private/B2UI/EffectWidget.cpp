#include "B2UI/EffectWidget.h"

void UEffectWidget::Play(float StartDelay, float InPostDelay)
{
	TargetScreenPosition = GetScreenCenter();
	PostDelay = InPostDelay;
}

void UEffectWidget::Play(const FVector& TargetWorldPosition, float StartDelay, float InPostDelay)
{
	TargetScreenPosition = WorldToScreenPosition(TargetWorldPosition);
	PostDelay = InPostDelay;
}

void UEffectWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (OnEffectFinished.IsBound()) OnEffectFinished.Broadcast();
}

FVector2D UEffectWidget::WorldToScreenPosition(FVector WorldPosition) const
{
	APlayerController* LocalPlayerController = GetWorld()->GetFirstPlayerController();

	FVector2D ScreenPosition;

	if (LocalPlayerController)
	{
		LocalPlayerController->ProjectWorldLocationToScreen(WorldPosition, ScreenPosition);
	}

	return ScreenPosition;
}

FVector2D UEffectWidget::GetScreenCenter() const
{
	APlayerController* LocalPlayerController = GetWorld()->GetFirstPlayerController();

	FVector2D ScreenPosition;

	if (LocalPlayerController)
	{
		int32 ScreenWidth;
		int32 ScreenHeight;

		LocalPlayerController->GetViewportSize(ScreenWidth, ScreenHeight);

		ScreenPosition.X = ScreenWidth / 2.0f;
		ScreenPosition.Y = ScreenHeight / 2.0f;
	}

	return ScreenPosition;
}
