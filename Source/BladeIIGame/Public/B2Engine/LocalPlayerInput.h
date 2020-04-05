#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

#include "B2Misc/Enum.h"

#include "LocalPlayerInput.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FButtonEventDelegate, const EButton, Button);

UCLASS()
class BLADEIIGAME_API ALocalPlayerInput : public APawn
{
	GENERATED_BODY()

public:
	/* Callback for player button inputs */
	FButtonEventDelegate HandleButtonPressed;

	ALocalPlayerInput();

	virtual void Tick(float DeltaTime) override;

	/* Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Returns the current cursor position. Top left is (0, 0), bottom right is (MAX, MAX) */
	FVector2D GetCurrentMousePosition() const;

protected:
	virtual void BeginPlay() override;

private:
	/* Casted version of APawns standard controller  */
	APlayerController* PlayerController;

	/* Mouse position during the previous frame */
	FVector2D PreviousMousePosition;

	/* Updates the current mouse position */
	void UpdateMousePosition();

	// Various callbacks

	void OnMenuPressed();
	void OnMouseButtonLeft();
	void OnNavivateLeftPressed();
	void OnNavivateRightPressed();
	void OnSelectPressed();
	void OnMouseMoved(const FVector2D& NewMousePosition);

};
