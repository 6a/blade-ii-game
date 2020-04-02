#include "B2Game/DeckSlot.h"

void UDeckSlot::Add(const ACard* Card)
{

	Super::Add(Card);
}

const ACard* UDeckSlot::RemoveByIndex(UINT N)
{

	return Super::RemoveByIndex(N);
}

const ACard* UDeckSlot::RemoveByID(FString ID)
{

	return Super::RemoveByID(ID);
}

const FTransform UDeckSlot::GetTransformForIndex(UINT Index) const
{
	return FTransform(GetComponentTransform());
}
