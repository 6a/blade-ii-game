#include "B2Engine/LocalPlayerInput.h"

#include "Engine/World.h"
#include "Components/InputComponent.h"

#include "B2Utility/Log.h"

const float NAV_POLL_DELAY = 0.25f;
const float NAV_POLL_INTERVAL = 0.125f;

ALocalPlayerInput::ALocalPlayerInput()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsLeftNavDown = bIsRightNavDown = false;
	NavButtonPriority = ENavButton::None;
	NextPollTime = 0;
}

void ALocalPlayerInput::BeginPlay()
{
	Super::BeginPlay();
}

void ALocalPlayerInput::OnMenuPressed()
{
	if (OnMenuButtonPressed.IsBound()) OnMenuButtonPressed.Broadcast();

	B2Utility::LogInfo(TEXT("Menu Pressed"));
}

void ALocalPlayerInput::OnMouseButtonLeft()
{
	B2Utility::LogInfo(FString::Format(TEXT("Mouse Clicked @ [{0} {1}]"), { PreviousMousePosition.X, PreviousMousePosition.Y }));
}

void ALocalPlayerInput::OnNavigateLeftPressed()
{
	ButtonInputQueue.Enqueue(EInput::NavigateLeft);

	NavButtonPriority = ENavButton::Left;
	bIsLeftNavDown = true;

	if (!bIsRightNavDown) NextPollTime = GetWorld()->GetTimeSeconds() + NAV_POLL_INTERVAL + NAV_POLL_DELAY;

	B2Utility::LogInfo(TEXT("Navigate Left Pressed"));
}

void ALocalPlayerInput::OnNavigateRightPressed()
{
	ButtonInputQueue.Enqueue(EInput::NavigateRight);

	NavButtonPriority = ENavButton::Right;
	bIsRightNavDown = true;

	if (!bIsLeftNavDown) NextPollTime = GetWorld()->GetTimeSeconds() + NAV_POLL_INTERVAL + NAV_POLL_DELAY;

	B2Utility::LogInfo(TEXT("Navigate Right Pressed"));
}

void ALocalPlayerInput::OnNavigateLeftReleased()
{
	bIsLeftNavDown = false;
	if (bIsRightNavDown)
	{
		NavButtonPriority = ENavButton::Right;
	}
	else
	{
		NavButtonPriority = ENavButton::None;
	}
}

void ALocalPlayerInput::OnNavigateRightReleased()
{
	bIsRightNavDown = false;
	if (bIsLeftNavDown)
	{
		NavButtonPriority = ENavButton::Left;
	}
	else
	{
		NavButtonPriority = ENavButton::None;
	}
}

void ALocalPlayerInput::OnSelectPressed()
{
	ButtonInputQueue.Enqueue(EInput::Select);

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

void ALocalPlayerInput::HandleNavigationPolling()
{
	if (NavButtonPriority == ENavButton::Left)
	{
		if (GetWorld()->GetTimeSeconds() > NextPollTime)
		{
			ButtonInputQueue.Enqueue(EInput::NavigateLeft);
			NextPollTime = GetWorld()->GetTimeSeconds() + NAV_POLL_INTERVAL;
		}
	}
	else if (NavButtonPriority == ENavButton::Right)
	{
		if (GetWorld()->GetTimeSeconds() > NextPollTime)
		{
			ButtonInputQueue.Enqueue(EInput::NavigateRight);
			NextPollTime = GetWorld()->GetTimeSeconds() + NAV_POLL_INTERVAL;
		}
	}
}

void ALocalPlayerInput::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateMousePosition();

	HandleNavigationPolling();
}

void ALocalPlayerInput::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* Set up mouse controls */
	PlayerController = Cast<APlayerController>(GetController());

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputMode);

	/* Bind the left mouse click button */
	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &ALocalPlayerInput::OnMouseButtonLeft);

    /* Bind the Menu button */
    InputComponent->BindAction("Menu", IE_Pressed, this, &ALocalPlayerInput::OnMenuPressed);

	/* Bind the Navigation buttons */
	InputComponent->BindAction("NavigateRight", IE_Pressed, this, &ALocalPlayerInput::OnNavigateRightPressed);
	InputComponent->BindAction("NavigateLeft", IE_Pressed, this, &ALocalPlayerInput::OnNavigateLeftPressed);
	InputComponent->BindAction("NavigateRight", IE_Released, this, &ALocalPlayerInput::OnNavigateRightReleased);
	InputComponent->BindAction("NavigateLeft", IE_Released, this, &ALocalPlayerInput::OnNavigateLeftReleased);

	/* Bind Selct button */
	InputComponent->BindAction("Select", IE_Pressed, this, &ALocalPlayerInput::OnSelectPressed);

}

