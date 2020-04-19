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

const FB2Transform UHandSlot::GetCurrentCenterTransform() const
{
	// Grab the original position and rotation without the offset that is applied by the overriden version above
	FVector CenterPosition = FMath::Lerp(UCardSlot::GetTransformForIndex(3).Position, UCardSlot::GetTransformForIndex(4).Position, 0.5f);
	FRotator CenterRotation = FMath::Lerp(UCardSlot::GetTransformForIndex(4).Rotation, UCardSlot::GetTransformForIndex(5).Rotation, 0.5f);

	return FB2Transform(CenterPosition, CenterRotation);
}
