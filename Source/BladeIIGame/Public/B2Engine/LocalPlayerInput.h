#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Containers/Queue.h"

#include "B2Misc/Enum.h"

#include "LocalPlayerInput.generated.h"

UCLASS()
class BLADEIIGAME_API ALocalPlayerInput : public APawn
{
	GENERATED_BODY()

public:
	TQueue<EButton> ButtonInputQueue;

	ALocalPlayerInput();

	virtual void Tick(float DeltaTime) override;

	/* Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Returns the current cursor position. Top left is (0, 0), bottom right is (MAX, MAX) */
	FVector2D GetCurrentMousePosition() const;

protected:
	virtual void BeginPlay() override;

private:
	enum ENavButton
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

	/* Updates the current mouse position */
	void UpdateMousePosition();

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
