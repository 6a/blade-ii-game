#include "B2Game/CardSelector.h"

#include "UObject/ConstructorHelpers.h"

const FVector ACardSelector::OFFSET_WHEN_SELECTED = FVector(0, 0.8f, 6.f);

ACardSelector::ACardSelector()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Selector Mesh"));
	RootComponent = Mesh;

	ToggleActorVisibility(false);
}

void ACardSelector::ToggleActorVisibility(bool bVisible)
{
	SetActorHiddenInGame(!bVisible);

	bIsVisible = bVisible;
}

bool ACardSelector::IsVisible() const
{
	return bIsVisible;
}
