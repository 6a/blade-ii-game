#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Containers/Queue.h"

#include "B2Game/Card.h"
#include "B2Enum/InputEnum.h"

#include "LocalPlayerInput.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMenuButtonPressedDelegate);

UCLASS()
class BLADEIIGAME_API ALocalPlayerInput : public APawn
{
	GENERATED_BODY()

public:
	TQueue<EInput> ButtonInputQueue;

	FMenuButtonPressedDelegate OnMenuButtonPressed;

	ALocalPlayerInput();

	virtual void Tick(float DeltaTime) override;

	/* Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Returns the current cursor position. Top left is (0, 0), bottom right is (MAX, MAX) */
	FVector2D GetCurrentMousePosition() const;

	/* Prevent this input controller for receiving and caching any more inputs */
	void BlockInputs();

	/* Returns true if the mouse has not moved for at least one frame */
	bool MouseIsStationary() const;

	/* Returns a pointer to the first card (if any) that the mouse is hovering over */
	ACard* GetHoveredCard() const;

protected:
	virtual void BeginPlay() override;

private:
	enum class ENavButton : uint8
	{
		None,
		Left,
		Right
	};

	/* Casted version of APawns standard controller  */
	APlayerController* PlayerController;

	/* Mouse position during the previous frame */
	FVector2D PreviousMousePosition;

	/* For nav button polling */
	bool bIsLeftNavDown;
	bool bIsRightNavDown;
	ENavButton NavButtonPriority;
	float NextPollTime;

	/* Flag to set when the mouse has not moved for at least one frame */
	bool bMouseIsStationary;

	/* For disabling input */
	bool bIsCheckingForInput;

	/* The currently hovered card, if any */
	UPROPERTY()
	ACard* HoveredCard;

	/* Updates the current mouse position */
	void UpdateMousePosition();

	/* Updates the pointer to the current hovered card (if any) */
	void UpdateHoveredCard();

	/* Handle polling for the navigation buttons so they can "repeat" when held down */
	void HandleNavigationPolling();

	// Various callbacks
	void OnMenuPressed();
	void OnMouseButtonLeft();
	void OnNavigateLeftPressed();
	void OnNavigateRightPressed();
	void OnNavigateLeftReleased();
	void OnNavigateRightReleased();
	void OnSelectPressed();
	void OnMouseMoved(const FVector2D& NewMousePosition);
};
