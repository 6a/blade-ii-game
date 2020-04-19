#include "B2Game/AvatarCaptureRig.h"

#include "Components/SkeletalMeshComponent.h"

// Sets default values
AAvatarCaptureRig::AAvatarCaptureRig()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	Avatar = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Avatar Mesh"));
	Avatar->SetupAttachment(RootComponent);

	Boom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Boom"));
	Boom->SetupAttachment(Avatar);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Boom);
}

// Called when the game starts or when spawned
void AAvatarCaptureRig::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAvatarCaptureRig::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

