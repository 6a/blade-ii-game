#include "B2Engine/LocalPlayerInput.h"

#include "Components/InputComponent.h"

#include "B2Misc/Utility.h"

ALocalPlayerInput::ALocalPlayerInput()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ALocalPlayerInput::BeginPlay()
{
	Super::BeginPlay();
}

void ALocalPlayerInput::OnMenuPressed()
{
	ButtonInputQueue.Enqueue(EButton::Menu);

	B2Utility::LogInfo(TEXT("Menu Pressed"));
}

void ALocalPlayerInput::OnMouseButtonLeft()
{
	B2Utility::LogInfo(FString::Format(TEXT("Mouse Clicked @ [{0} {1}]"), { PreviousMousePosition.X, PreviousMousePosition.Y }));
}

void ALocalPlayerInput::OnNavivateLeftPressed()
{
	ButtonInputQueue.Enqueue(EButton::NavigateLeft);

	B2Utility::LogInfo(TEXT("Navigate Left Pressed"));
}

void ALocalPlayerInput::OnNavivateRightPressed()
{
	ButtonInputQueue.Enqueue(EButton::NavigateRight);

	B2Utility::LogInfo(TEXT("Navigate Right Pressed"));
}

void ALocalPlayerInput::OnSelectPressed()
{
	ButtonInputQueue.Enqueue(EButton::Select);

	B2Utility::LogInfo(TEXT("Select Pressed"));
}

void ALocalPlayerInput::OnMouseMoved(const FVector2D& NewMousePosition)
{

}

FVector2D ALocalPlayerInput::GetCurrentMousePosition() const
{
	FVector2D CurrentMousePosition;
	PlayerController->GetMousePosition(CurrentMousePosition.X, CurrentMousePosition.Y);

	return CurrentMousePosition;
}

void ALocalPlayerInput::UpdateMousePosition()
{
	FVector2D CurrentMousePosition = GetCurrentMousePosition();

	if (CurrentMousePosition != PreviousMousePosition)
	{
		PreviousMousePosition = CurrentMousePosition;
		OnMouseMoved(CurrentMousePosition);
	}
}

void ALocalPlayerInput::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateMousePosition();
}

void ALocalPlayerInput::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* Set up mouse controls */
	PlayerController = Cast<APlayerController>(GetController());
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;

	/* Bind the left mouse click button */
	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &ALocalPlayerInput::OnMouseButtonLeft);

    /* Bind the Menu button */
    InputComponent->BindAction("Menu", IE_Pressed, this, &ALocalPlayerInput::OnMenuPressed);

	/* Bind the Navigation buttons */
	InputComponent->BindAction("NavigateRight", IE_Pressed, this, &ALocalPlayerInput::OnNavivateRightPressed);
	InputComponent->BindAction("NavigateLeft", IE_Pressed, this, &ALocalPlayerInput::OnNavivateLeftPressed);

	/* Bind Selct button */
	InputComponent->BindAction("Select", IE_Pressed, this, &ALocalPlayerInput::OnSelectPressed);

}

