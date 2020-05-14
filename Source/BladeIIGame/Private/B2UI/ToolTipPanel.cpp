#include "Components/CanvasPanelSlot.h"

#include "B2UI/ToolTipPanel.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"

#include "B2Utility/Log.h"

const FVector2D ALIGNMENT_TOP_LEFT = FVector2D(0, -0.75f);
const FVector2D ALIGNMENT_TOP_RIGHT = FVector2D(1, -0.75f);
const FVector2D ALIGNMENT_BOTTOM_LEFT = FVector2D(0, 1);
const FVector2D ALIGNMENT_BOTTOM_RIGHT = FVector2D(1, 1);

const float TOOL_TIP_DELAY = 0.5f;

UToolTipPanel::UToolTipPanel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	LastMouseMoveTime = 1000000;
}

void UToolTipPanel::SetLocalPlayerInput(ALocalPlayerInput* InLocalPlayerInput)
{
	LocalPlayerInput = InLocalPlayerInput;
}

void UToolTipPanel::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (LocalPlayerInput && ToolTip)
	{
		bool bShowTooltip = ShowToolTip();
		ToolTip->SetVisible(bShowTooltip);

		if (bShowTooltip)
		{
			ToolTip->SetVisible(bShowTooltip);

			APlayerController* LocalPlayerController = static_cast<APlayerController*>(LocalPlayerInput->GetController());
			if (LocalPlayerController)
			{
				FVector2D MousePosition = LocalPlayerInput->GetCurrentMousePosition();

				float DPIScale = GetUIScale(LocalPlayerController);
				UCanvasPanelSlot* ToolTipSlot = static_cast<UCanvasPanelSlot*>(ToolTip->Slot);

				ToolTipSlot->SetAlignment(GetAlignment(LocalPlayerController, MousePosition));
				ToolTipSlot->SetPosition(MousePosition / DPIScale);
			}
		}

		if (!LocalPlayerInput->MouseIsStationary())
		{
			UWorld* World = GetWorld();
			if (World)
			{
				LastMouseMoveTime = World->GetRealTimeSeconds();
			}
		}
	}
}

float UToolTipPanel::GetUIScale(const APlayerController* LocalPlayerController) const
{
	// Ref: https://gist.github.com/JonathanADaley/151f26b145981336371b73def45209e7

	FIntVector ViewportSize;

	LocalPlayerController->GetViewportSize(ViewportSize.X, ViewportSize.Y);

	return GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));
}

FVector2D UToolTipPanel::GetAlignment(const APlayerController* LocalPlayerController, const FVector2D& MousePosition) const
{
	FIntVector ViewportSize;

	LocalPlayerController->GetViewportSize(ViewportSize.X, ViewportSize.Y);

	FVector2D OutVector;

	// If Y is greater, then its on the bottom half of the screen.
	// If X is greater, then its on the right side of the screen.
	if (MousePosition.Y > ViewportSize.Y / 2)
	{
		if (MousePosition.X > ViewportSize.X / 2)
		{
			OutVector = ALIGNMENT_BOTTOM_RIGHT;
		}
		else
		{
			OutVector = ALIGNMENT_BOTTOM_LEFT;
		}
	}
	else
	{
		if (MousePosition.X > ViewportSize.X / 2)
		{
			OutVector = ALIGNMENT_TOP_RIGHT;
		}
		else
		{
			OutVector = ALIGNMENT_TOP_LEFT;
		}
	}

	return OutVector;
}

bool UToolTipPanel::ShowToolTip() const
{
	UWorld* World = GetWorld();
	if (World)
	{
		return World->GetRealTimeSeconds() - LastMouseMoveTime > TOOL_TIP_DELAY;
	}

	return false;
}