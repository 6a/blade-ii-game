#include "B2Game/HandSlot.h"

const uint32 MAX_CAPACITY = 10;

void UHandSlot::Add(ACard* Card)
{
	Super::Add(Card);

	if (!bCardsDealt && Num() == MAX_CAPACITY)
	{
		bCardsDealt = true;
	}
}

const FB2Transform UHandSlot::GetTransformForIndex(uint32 Index) const
{
	if (!bCardsDealt)
	{
		return Super::GetTransformForIndex(Index);
	}

	FB2Transform Transform = GetBaseTransform();

	TArray<FB2Transform> TransformsSource = Num() % 2 == 0 ? CardTransforms : CardTransformsOffset;
	uint32 Offset = Num() < 9 ? (TransformsSource.Num() - Num()) / 2 : 0;
	uint32 NewIndex = Index + Offset;

	if (TransformsSource.IsValidIndex(NewIndex))
	{
		Transform = TransformsSource[NewIndex];
	}

	return Transform;
}
