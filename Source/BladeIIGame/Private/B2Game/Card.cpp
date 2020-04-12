#include "B2Game/Card.h"

#include "Misc/Guid.h"
#include "UObject/ConstructorHelpers.h"

ACard::ACard()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Card Mesh"));
	RootComponent = Mesh;

	ID = FGuid::NewGuid().ToString();

	bIsActive = true;

	PrimaryActorTick.bCanEverTick = true;
}

void ACard::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACard::QueueTransition(const B2Transition& Transition)
{
	Transitions.Enqueue(Transition);
}

bool ACard::IsTransitioning() const
{
	return !Transitions.IsEmpty();
}

const FString ACard::GetID() const
{
	return ID;
}

void ACard::SetActive(bool bNewActive)
{
	bIsActive = bNewActive;
}

bool ACard::IsActive()
{
	return bIsActive;
}

bool ACard::IsFaceDown()
{
	return FVector::DotProduct(GetActorUpVector(), FVector::UpVector) < 0;
}

// Called every frame
void ACard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* If there is an active transition */
	if (IsTransitioning())
	{
		/* If the transition has now finished, remove it and exit early */
		if (Transitions.Peek()->Done())
		{
			Transitions.Pop();
			return;
		}

		/* Tick the transition */
		Transitions.Peek()->Tick(DeltaTime);

		/* Apply the new values to the card */
		SetActorLocationAndRotation(Transitions.Peek()->CurrentPosition, Transitions.Peek()->CurrentRotation);
	}
}

