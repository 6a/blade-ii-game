#include "B2Game/CardSlot.h"

UCardSlot::UCardSlot()
{
	BaseTransform = FB2Transform(GetComponentLocation(), GetComponentRotation());


}

UINT UCardSlot::Size() const
{
	return Cards.Num();
}

void UCardSlot::Add(ACard* Card)
{
	Cards.Add(Card);
}

ACard* UCardSlot::GetCardByIndex(UINT N)
{
	ACard* Card = nullptr;

	if (Cards.IsValidIndex(N))
	{
		Card = Cards[N];
	}

	return Card;
}

ACard* UCardSlot::GetCardByID(FString ID)
{
	ACard* Card = nullptr;

	for (size_t i = 0; i < Cards.Num(); i++)
	{
		if (Cards[i]->GetID().Compare(ID) == 0)
		{
			Card = Cards[i];
			break;
		}
	}

	return Card;
}

ACard* UCardSlot::RemoveByIndex(UINT N)
{
	ACard* Card = nullptr;

	if (Cards.IsValidIndex(N))
	{
		Card = Cards[N];
		Cards.RemoveAt(N);
	}

	return Card;
}

ACard* UCardSlot::RemoveByID(FString ID)
{
	ACard* Card = nullptr;
	
	for (size_t i = 0; i < Cards.Num(); i++)
	{
		if (Cards[i]->GetID().Compare(ID) == 0)
		{
			Card = Cards[i];
			Cards.RemoveAt(i);
			break;
		}
	}

	return Card;
}

const FB2Transform UCardSlot::GetTransformForIndex(UINT Index) const
{
	FB2Transform transform;
	
	if (CardTransforms.IsValidIndex(Index))
	{
		transform = CardTransforms[Index];
	}

	return transform;
}
