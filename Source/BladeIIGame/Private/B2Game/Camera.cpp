#include "B2Game/Camera.h"

#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"

const float NUM_DIR_CHANGES = 5;
const float DIR_CHANGE_INTERVAL = 1.f / NUM_DIR_CHANGES;

ACamera::ACamera(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TransformRoot = CreateDefaultSubobject<USceneComponent>(TEXT("TransformRoot"));
	RootComponent = TransformRoot;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->FieldOfView = 40.0f;
	CameraComponent->bConstrainAspectRatio = true;
	CameraComponent->AspectRatio = 1.777778f;
	CameraComponent->PostProcessBlendWeight = 1.0f;

	CameraComponent->SetupAttachment(TransformRoot);
}

void ACamera::Shake(float Delay)
{
	if (Delay > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(DelayedShakeHandle, this, &ACamera::ShakeTimerCallback, Delay, false);
	}
	else
	{
		if (CameraShakeClass)
		{
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShakeClass, 1.0f);
		}
	}
}

void ACamera::BeginPlay()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		PlayerController->SetViewTarget(this);
	}

	PrimaryActorTick.SetTickFunctionEnable(false);

	Super::BeginPlay();
}

void ACamera::ShakeTimerCallback()
{
	Shake();
}
