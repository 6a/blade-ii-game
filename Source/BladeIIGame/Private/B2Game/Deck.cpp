#include "B2Game/Deck.h"
#include "..\..\Public\B2Game\Deck.h"

UDeck::UDeck()
{
	Super::Super();

	BasePosition = GetComponentLocation();
	BaseRotation = GetComponentRotation();
}

void UDeck::Add(const ACard* Card)
{

	Super::Add(Card);
}

const ACard* UDeck::RemoveByIndex(UINT N)
{

	return Super::RemoveByIndex(N);
}

const ACard* UDeck::RemoveByID(FString ID)
{

	return Super::RemoveByID(ID);
}

const FTransform UDeck::GetTransformForIndex(UINT Index)
{
	return FTransform(GetComponentTransform());
}
