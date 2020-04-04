#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

#include "LocalPlayerInput.generated.h"

UCLASS()
class BLADEIIGAME_API ALocalPlayerInput : public APawn
{
	GENERATED_BODY()

public:
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
	void OnMouseMoved(const FVector2D& NewMousePosition);

};
