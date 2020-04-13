#include "B2Game/Card.h"

#include "Materials/MaterialInstanceDynamic.h"

#include "Misc/Guid.h"
#include "UObject/ConstructorHelpers.h"

ACard::ACard()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Card Mesh"));
	Mesh->SetupAttachment(RootComponent);

	ID = FGuid::NewGuid().ToString();

	bIsActive = true;

	PrimaryActorTick.bCanEverTick = true;
}

void ACard::BeginPlay()
{
	Super::BeginPlay();

	auto M0 = static_cast<UMaterialInstanceDynamic*>(this->Mesh->GetMaterial(0));
	auto M1 = static_cast<UMaterialInstanceDynamic*>(this->Mesh->GetMaterial(1));
	
	MaterialInstances.Add(M0);
	MaterialInstances.Add(M1);
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
		/* Tick the transition */
		Transitions.Peek()->Tick(DeltaTime);

		/* Apply the new values to the card */
		SetActorLocationAndRotation(Transitions.Peek()->CurrentPosition, Transitions.Peek()->CurrentRotation);

		/* If the transition has now finished, remove it and exit early */
		if (Transitions.Peek()->Done())
		{
			//B2Utility::LogInfo(FString::Printf(TEXT("Card with WG [ %d ] Finished"), Transitions.Peek()->WaitGroup));

			Transitions.Pop();
			return;
		}
	}
}

