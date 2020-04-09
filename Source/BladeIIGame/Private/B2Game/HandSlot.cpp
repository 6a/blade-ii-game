#include "B2Game/HandSlot.h"

void UHandSlot::Add(ACard* Card)
{

	Super::Add(Card);
}

ACard* UHandSlot::RemoveByIndex(uint32 N)
{

	return Super::RemoveByIndex(N);
}

ACard* UHandSlot::RemoveByID(FString ID)
{

	return Super::RemoveByID(ID);
}