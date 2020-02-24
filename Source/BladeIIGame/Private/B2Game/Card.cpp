#include "B2Game/Card.h"

#include "Misc/Guid.h"
#include "UObject/ConstructorHelpers.h"

ACard::ACard()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Card Mesh"));
	RootComponent = Mesh;

	ID = FGuid::NewGuid().ToString();

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

const UCardSlot* ACard::GetAnchor() const
{
	return Anchor;
}

// Called every frame
void ACard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* If there is an active transition */
	if (IsTransitioning())
	{
		/* Tick the transition */
		Transitions.Peek()->Tick(DeltaTime);

		/* Apply the new values to the card */
		SetActorLocationAndRotation(Transitions.Peek()->CurrentPosition, Transitions.Peek()->CurrentRotation);

		/* If the transition has now finished, remove it */
		if (Transitions.Peek()->Done())
		{
			Transitions.Pop();
		}
	}
}

