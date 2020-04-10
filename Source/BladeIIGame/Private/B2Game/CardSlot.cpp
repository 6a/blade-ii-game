#include "B2Game/CardSlot.h"

#include "Algo/Reverse.h"

#include "B2Predicate/SortCardsByDistanceAscending.h"
#include "B2Predicate/SortCardsByTypeAscending.h"

UCardSlot::UCardSlot()
{

}

uint32 UCardSlot::Num() const
{
	// Lets just pray that num never returns less than zero or larger than (MAX INT)
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

ACard* UCardSlot::GetCardByIndex(uint32 N)
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

ACard* UCardSlot::RemoveByIndex(uint32 N)
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

ACard* UCardSlot::RemoveLast()
{
	ACard* Card = nullptr;

	if (Num() > 0)
	{
		Card = RemoveByIndex(Num() - 1);
	}

	return Card;

}

ACard* UCardSlot::GetLast()
{
	ACard* Card = nullptr;

	if (Num() > 0)
	{
		Card = Cards.Last();
	}

	return Card;
}

ACard* UCardSlot::GetFirst()
{
	ACard* Card = nullptr;

	if (Num() > 0)
	{
		Card = Cards[0];
	}

	return Card;
}

const FB2Transform UCardSlot::GetTransformForIndex(uint32 Index) const
{
	FB2Transform Transform = GetBaseTransform();
	
	if (CardTransforms.IsValidIndex(Index))
	{
		Transform = CardTransforms[Index];
	}

	return Transform;
}

const FB2Transform UCardSlot::GetNextTransform() const
{
	uint32 TransformIndex = Num();

	if (!CardTransforms.IsValidIndex(TransformIndex))
	{
		TransformIndex = CardTransforms.Num() - 1;
	}


	return CardTransforms[TransformIndex];
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

const FB2Transform UCardSlot::GetBaseTransform() const
{
	return FB2Transform(GetComponentLocation(), GetComponentRotation());
}
