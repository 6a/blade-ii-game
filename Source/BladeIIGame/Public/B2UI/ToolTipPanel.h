#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"

#include "B2UI/ToolTip.h"
#include "B2Engine/LocalPlayerInput.h"
#include "B2Game/Card.h"

#include "ToolTipPanel.generated.h"

struct ToolTipTextPair
{
	FText Header;
	FText Information;
};

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

	/**
	 * Set the tooltip panel to active / inactive
	 * @param bNewVisible - Whether the tooltip should be active or not after the call to this function
	 */
	void SetActive(bool bNewActive);

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

	/* Whether this tooltip panel is active (visible, tracking mouse movement) or not */
	bool bActive;

	/* Whether there is currently a delayed call to activate the tooltip */
	bool bToolTipActivationPending;

	/* The type for the most recently hovered card */
	ACard* CurrentHoveredCard;

	/* Whether the tooltip text requires an update (due to the hovered card changing etc.) */
	bool bToolTipTextRequiresUpdate;

	/* Returns the current UI scale */
	float GetUIScale(const APlayerController* LocalPlayerController) const;

	/* Returns the current alignment */
	FVector2D GetAlignment(const APlayerController* LocalPlayerController, const FVector2D& MousePosition, const UCanvasPanelSlot* CanvasPanelSlot) const;

	/* Returns true when the tooltip should be shown */
	bool ShouldShowToolTip() const;

	/**
	 * Get the text pair (Header + Information) associated with the specified card
	 * @param Card - Card type to get the pair for
	 * @return The text pair
	 */
	ToolTipTextPair GetTextPair(ECard Card) const;

	/* Get the textpair for face-down cards (that are not bolted) */
	ToolTipTextPair GetFaceDownCardTextPair() const;

	/* Callback for the 1-frame delay for enabling the visiblity on the tooltip */
	UFUNCTION()
	void SetToolTipVisibleDelayed();
};
