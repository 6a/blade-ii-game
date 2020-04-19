#include "B2Game/CardSlot.h"

#include "Algo/Reverse.h"

#include "B2Predicate/SortCardsByDistanceAscending.h"
#include "B2Predicate/SortCardsByTypeAscending.h"
#include "B2Predicate/MatchCardType.h"
#include "B2Utility/String.h"
#include "..\..\Public\B2Game\CardSlot.h"

UCardSlot::UCardSlot()
{

}

uint32 UCardSlot::Num() const
{
	// Lets just pray that num never returns less than zero or larger than (MAX INT)
	return Cards.Num();
}

TArray<FString> UCardSlot::GetSortedIDsAscending() const
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

TArray<FString> UCardSlot::GetSortedIDsDescending() const
{
	TArray<FString> OutArray = GetSortedIDsAscending();

	Algo::Reverse(OutArray);

	return OutArray;
}

void UCardSlot::Add(ACard* Card)
{
	Cards.Add(Card);
}

void UCardSlot::Add(TArray<ACard*> InCards)
{
	Cards.Append(InCards);
}

ACard* UCardSlot::GetCardByIndex(uint32 N) const
{
	ACard* Card = nullptr;

	if (Cards.IsValidIndex(N))
	{
		Card = Cards[N];
	}

	return Card;
}

ACard* UCardSlot::GetCardByID(FString ID) const
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

int UCardSlot::GetIndexByID(FString ID) const
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

ACard* UCardSlot::GetLast() const
{
	ACard* Card = nullptr;

	if (Num() > 0)
	{
		Card = Cards.Last();
	}

	return Card;
}

ACard* UCardSlot::GetFirst() const
{
	ACard* Card = nullptr;

	if (Num() > 0)
	{
		Card = Cards[0];
	}

	return Card;
}

ACard* UCardSlot::GetFirstOfType(ECard CardType) const
{
	ACard* Card = nullptr;

	Card = *(Cards.FindByPredicate(B2Predicate_MatchCardActorType(CardType)));

	return Card;
}

ACard* UCardSlot::GetRandomOfType(ECard CardType) const
{
	ACard* Card = nullptr;

	TArray<ACard*> CardsOfType = Cards.FilterByPredicate(B2Predicate_MatchCardActorType(CardType));
	if (CardsOfType.Num() > 0)
	{
		Card = CardsOfType[FMath::RandRange(0, CardsOfType.Num() -1)];
	}

	return Card;
}

TArray<ACard*> UCardSlot::GetAll() const
{
	return TArray<ACard*>(Cards);
}

ACard* UCardSlot::RemoveFirstOfType(const ECard& CardType)
{
	ACard* Card = nullptr;

	int32 Index = Cards.IndexOfByPredicate(B2Predicate_MatchCardActorType(CardType));
	if (Index != INDEX_NONE)
	{
		Card = RemoveByIndex(Index);
	}

	return Card;
}

TArray<ACard*> UCardSlot::RemoveAll()
{
	TArray<ACard*> OutArray = TArray<ACard*>(Cards);

	Cards.Empty();

	return OutArray;
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

	return GetTransformForIndex(TransformIndex);
}

int32 UCardSlot::GetFirstIndexOfType(ECard CardType) const
{
	int32 Index = -1;

	for (size_t i = 0; i < Cards.Num(); i++)
	{
		if (Cards[i]->Type == CardType)
		{
			Index = i;
			break;
		}
	}

	return Index;
}

void UCardSlot::UpdateCardOrder()
{
	// This actually sorts by doing a distance check from the position of the 0th card, for all the cards in the slot
	// Lowest distance is closer to index 0 and so on

	FVector RootLocation = CardTransforms[0].Position;
	Cards.Sort(B2Predicate_SortCardsByDistanceAscending(RootLocation));
}

FString UCardSlot::Stringify() const
{
	FString SlotString;

	for (size_t i = 0; i < Num(); i++)
	{
		SlotString.Append(B2Utility::CardEnumToString(Cards[i]->Type));
		if (i < Num() - 1)
		{
			SlotString.Append(" | ");
		}
	}

	return SlotString;
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
