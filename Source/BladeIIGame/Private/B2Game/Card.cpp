#include "B2Game/Card.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Misc/Guid.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"

#include "B2Game/GameSound.h"
#include "B2Utility/Log.h"

ACard::ACard()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Card Mesh"));
	Mesh->SetupAttachment(RootComponent);

	ID = FGuid::NewGuid().ToString();

	bIsActive = true;

	PrimaryActorTick.bCanEverTick = true;

	OpacityTransitionAlpha = 1;
	FadeDuration = 0;
	FadeState = EFadeState::None;
}

void ACard::BeginPlay()
{
	Super::BeginPlay();

	// Find and store a reference to the game sound actor
	for (TActorIterator<AGameSound> GameSoundIter(GetWorld()); GameSoundIter; ++GameSoundIter)
	{
		if (GameSoundIter)
		{
			GameSound = *GameSoundIter;
		}
	}
}

void ACard::FadeIn(float Duration)
{
	FadeDuration = Duration;
	FadeState = EFadeState::FadingIn;
}

void ACard::FadeOut(float Duration)
{
	FadeDuration = Duration;
	FadeState = EFadeState::FadingOut;
}

const FString ACard::GetID() const
{
	return ID;
}

void ACard::SetActive(bool bNewActive)
{
	bIsActive = bNewActive;
}

bool ACard::IsActive() const
{
	return bIsActive;
}

bool ACard::IsFaceDown() const
{
	return FVector::DotProduct(GetActorUpVector(), FVector::UpVector) < 0;
}

uint32 ACard::TypeToValue(ECard CardType)
{
	uint32 Value;
	if (CardType > ECard::LaurasGreatsword)
	{
		if (CardType > ECard::Force)
		{
			Value = 0;
		}
		else
		{
			Value = 1;
		}
	}
	else
	{
		Value = static_cast<uint32>(CardType) + 1;
	}

	return Value;
}

// Called every frame
void ACard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FadeState != EFadeState::None)
	{
		UMaterialInstanceDynamic* M0 = static_cast<UMaterialInstanceDynamic*>(this->Mesh->GetMaterial(0));
		UMaterialInstanceDynamic* M1 = static_cast<UMaterialInstanceDynamic*>(this->Mesh->GetMaterial(1));

		float Step = LERP_MAX / (FadeDuration / DeltaTime);

		if (FadeState == EFadeState::FadingOut)
		{
			Step *= -1;
		}

		OpacityTransitionAlpha = FMath::Clamp(OpacityTransitionAlpha + Step, LERP_MIN, LERP_MAX);

		float CurrentOpacity = FMath::InterpEaseIn(LERP_MIN, LERP_MAX, OpacityTransitionAlpha, EASE_EXPONENT);

		M0->SetScalarParameterValue(OPACITY_PARAMETER, CurrentOpacity);
		M1->SetScalarParameterValue(OPACITY_PARAMETER, CurrentOpacity);

		if (OpacityTransitionAlpha == LERP_MAX || OpacityTransitionAlpha == LERP_MIN)
		{
			FadeState = EFadeState::None;
		}
	}
}

