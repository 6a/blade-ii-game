#include "B2Game/HandSlot.h"

void UHandSlot::Add(const ACard* Card)
{

	Super::Add(Card);
}

const ACard* UHandSlot::RemoveByIndex(UINT N)
{

	return Super::RemoveByIndex(N);
}

const ACard* UHandSlot::RemoveByID(FString ID)
{

	return Super::RemoveByID(ID);
}

const FTransform UHandSlot::GetTransformForIndex(UINT Index) const
{
	return FTransform(GetComponentTransform());
}
