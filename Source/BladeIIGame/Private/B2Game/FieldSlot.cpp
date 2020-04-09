#include "B2Game/FieldSlot.h"

void UFieldSlot::Add(ACard* Card)
{

	Super::Add(Card);
}

ACard* UFieldSlot::RemoveByIndex(uint32 N)
{

	return Super::RemoveByIndex(N);
}

ACard* UFieldSlot::RemoveByID(FString ID)
{

	return Super::RemoveByID(ID);
}
