#include "B2Game/Card.h"

#include "UObject/ConstructorHelpers.h"

ACard::ACard()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Card Mesh"));
	RootComponent = Mesh;
}

void ACard::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACard::StartTransitionAsync(const B2Transition& Transition)
{

}

bool ACard::IsTransitioning() const
{
	return !CurrentTransition.Done();
}

// Called every frame
void ACard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsTransitioning())
	{
		CurrentTransition.Tick(DeltaTime);
		SetActorLocationAndRotation(CurrentTransition.CurrentPosition, CurrentTransition.CurrentRotation);
	}
}

