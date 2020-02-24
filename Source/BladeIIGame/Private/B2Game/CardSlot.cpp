#include "B2Game/CardSlot.h"

UCardSlot::UCardSlot()
{
	//PrimaryActorTick.bCanEverTick = true;


}
UINT UCardSlot::Size() const
{
	return Cards.Num();
}

void UCardSlot::Add(const ACard* Card)
{
	Cards.Add(Card);
}

const ACard* UCardSlot::RemoveByIndex(UINT N)
{
	if (!Cards.IsValidIndex(N))
	{
		return nullptr;
	}

	const ACard* Card = Cards[N];
	Cards.RemoveAt(N);

	return Card;
}

const ACard* UCardSlot::RemoveByID(FString ID)
{
	const ACard* Card = nullptr;
	
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