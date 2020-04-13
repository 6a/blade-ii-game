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

const FB2Transform UHandSlot::GetTransformForIndex(uint32 Index) const
{
	FB2Transform Transform = GetBaseTransform();

	TArray<FB2Transform> TransformsSource = Num() % 2 == 0 ? CardTransforms : CardTransformsOffset;
	uint32 Offset = Num() < 9 ? (TransformsSource.Num()) / 2 - Num() : 0;
	uint32 NewIndex = Index + Offset;

	if (CardTransforms.IsValidIndex(NewIndex))
	{
		Transform = CardTransforms[NewIndex];
	}

	return Transform;
}
