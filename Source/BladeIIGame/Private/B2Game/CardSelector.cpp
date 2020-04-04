#include "B2Game/CardSelector.h"

#include "UObject/ConstructorHelpers.h"

ACardSelector::ACardSelector()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Selector Mesh"));
	RootComponent = Mesh;
}
