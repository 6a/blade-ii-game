#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "B2UI/ToolTip.h"
#include "B2Engine/LocalPlayerInput.h"

#include "ToolTipPanel.generated.h"

UCLASS()
class BLADEIIGAME_API UToolTipPanel : public UUserWidget
{
	GENERATED_BODY()

public:

	/**
	 * Set the local pointer to the local player input instance
	 * @param InLocalPlayerInput - a pointer to the local player input instance
	 */
	void SetLocalPlayerInput(ALocalPlayerInput* InLocalPlayerInput);

	UToolTipPanel(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:

	/* Pointer to the local player input receiver */
	UPROPERTY()
	ALocalPlayerInput* LocalPlayerInput;

	/* Tooltip widget */
	UPROPERTY(meta = (BindWidget))
	UToolTip* ToolTip;

	/* The time at which the mouse last moved */
	float LastMouseMoveTime;

	/* Returns the current UI scale */
	float GetUIScale(const APlayerController* LocalPlayerController) const;

	/* Returns the current alignment */
	FVector2D GetAlignment(const APlayerController* LocalPlayerController, const FVector2D& MousePosition) const;

	/* Returns true when the tooltip should be shown */
	bool ShowToolTip() const;
};
