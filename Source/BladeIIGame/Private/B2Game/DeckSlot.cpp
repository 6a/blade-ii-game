#include "B2Game/DeckSlot.h"

void UDeckSlot::Add(ACard* Card)
{

	Super::Add(Card);
}

ACard* UDeckSlot::RemoveByIndex(UINT N)
{

	return Super::RemoveByIndex(N);
}

ACard* UDeckSlot::RemoveByID(FString ID)
{

	return Super::RemoveByID(ID);
}