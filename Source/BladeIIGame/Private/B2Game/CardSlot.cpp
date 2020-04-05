#include "B2Game/CardSlot.h"
#include "Algo/Reverse.h"

UCardSlot::UCardSlot()
{
	BaseTransform = FB2Transform(GetComponentLocation(), GetComponentRotation());
}

UINT UCardSlot::Size() const
{
	return Cards.Num();
}

TArray<FString> UCardSlot::GetSortedIDsAscending()
{
	TArray<ACard*> CardsCopy(Cards);
	CardsCopy.Sort([](const ACard& a, const ACard& b) { return a.Type > b.Type; });

	TArray<FString> Strings;

	for (auto Iter = CardsCopy.CreateConstIterator(); Iter; ++Iter)
	{
		Strings.Add((*Iter)->GetID());
	}

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

void UCardSlot::UpdateCardOrder()
{
	FVector RootLocation = CardTransforms[0].Position;
	Cards.Sort([RootLocation](const ACard& a, const ACard& b)
	{
		float SquareDistanceA = FVector::DistSquared(RootLocation, a.GetActorLocation());
		float SquareDistanceB = FVector::DistSquared(RootLocation, b.GetActorLocation());

		return SquareDistanceA < SquareDistanceB;
	});

	for (size_t i = 0; i < Cards.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Card: %s"), *Cards[i]->GetFName().ToString());
	}
}

void UCardSlot::SetType(ETableSlot SlotType)
{
	Type = SlotType;
}

ETableSlot UCardSlot::GetType() const
{
	return Type;
}
