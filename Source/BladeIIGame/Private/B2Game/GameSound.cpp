#include "B2Game/GameSound.h"

#include "Components/AudioComponent.h"

AGameSound::AGameSound()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	BGMAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGM Audio Component"));
	BGMAudioComponent->SetupAttachment(RootComponent);

	SFXAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SFX Audio Component"));
	SFXAudioComponent->SetupAttachment(RootComponent);
}

void AGameSound::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGameSound::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

