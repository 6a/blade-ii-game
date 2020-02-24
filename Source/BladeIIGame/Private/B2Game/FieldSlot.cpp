#include "B2Game/FieldSlot.h"

UFieldSlot::UFieldSlot()
{
	BasePosition = GetComponentLocation();
	BaseRotation = GetComponentRotation();
}

void UFieldSlot::Add(const ACard* Card)
{

	Super::Add(Card);
}

const ACard* UFieldSlot::RemoveByIndex(UINT N)
{

	return Super::RemoveByIndex(N);
}

const ACard* UFieldSlot::RemoveByID(FString ID)
{

	return Super::RemoveByID(ID);
}

const FTransform UFieldSlot::GetTransformForIndex(UINT Index)
{
	return FTransform(GetComponentTransform());
}