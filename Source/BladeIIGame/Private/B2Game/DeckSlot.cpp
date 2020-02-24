#include "B2Game/DeckSlot.h"

UDeckSlot::UDeckSlot()
{
	BasePosition = GetComponentLocation();
	BaseRotation = GetComponentRotation();
}

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

const FTransform UDeckSlot::GetTransformForIndex(UINT Index)
{
	return FTransform(GetComponentTransform());
}
