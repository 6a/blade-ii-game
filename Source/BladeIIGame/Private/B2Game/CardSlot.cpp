#include "B2Game/CardSlot.h"

#include "Algo/Reverse.h"

#include "B2Predicate/SortCardsByDistanceAscending.h"
#include "B2Predicate/SortCardsByTypeAscending.h"

UCardSlot::UCardSlot()
{
	BaseTransform = FB2Transform(GetComponentLocation(), GetComponentRotation());
}

UINT UCardSlot::Count() const
{
	return Cards.Num();
}

TArray<FString> UCardSlot::GetSortedIDsAscending()
{
	TArray<ACard*> CardsCopy(Cards);
	CardsCopy.Sort(B2Predicate_SortCardsByTypeAscending());

	TArray<FString> Strings;

	for (auto Iter = CardsCopy.CreateConstIterator(); Iter; ++Iter)
	{
		Strings.Add((*Iter)->GetID());
	}

	CardsCopy.Empty();

	return Strings;
}

TArray<FString> UCardSlot::GetSortedIDsDescending()
{
	TArray<FString> OutArray = GetSortedIDsAscending();

	Algo::Reverse(OutArray);

	return OutArray;
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

int UCardSlot::GetIndexByID(FString ID)
{
	int OutIndex = -1;

	for (size_t i = 0; i < Cards.Num(); i++)
	{
		if (Cards[i]->GetID().Compare(ID) == 0)
		{
			OutIndex = i;
			break;
		}
	}

	return OutIndex;
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
	FB2Transform Transform = BaseTransform;
	
	if (CardTransforms.IsValidIndex(Index))
	{
		Transform = CardTransforms[Index];
	}

	return Transform;
}

void UCardSlot::UpdateCardOrder()
{
	// This actually sorts by doing a distance check from the position of the 0th card, for all the cards in the slot
	// Lowest distance is closer to index 0 and so on

	FVector RootLocation = CardTransforms[0].Position;
	Cards.Sort(B2Predicate_SortCardsByDistanceAscending(RootLocation));
}

void UCardSlot::SetType(ECardSlot SlotType)
{
	Type = SlotType;
}

ECardSlot UCardSlot::GetType() const
{
	return Type;
}
