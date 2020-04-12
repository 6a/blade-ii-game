#pragma once

#include "CoreMinimal.h"

#include "B2Enum/CardEnum.h"
#include "Cards.generated.h"

USTRUCT()
struct FB2Cards
{
	GENERATED_BODY()

	TArray<ECard> PlayerDeck;
	TArray<ECard> PlayerHand;
	TArray<ECard> PlayerField;
	TArray<ECard> PlayerDiscard;

	TArray<ECard> OpponentDeck;
	TArray<ECard> OpponentHand;
	TArray<ECard> OpponentField;
	TArray<ECard> OpponentDiscard;

	FB2Cards();

	/* Construct a cards object using an encoded string */
	FB2Cards(const FString& Cards);

	/**
	 * Returns the serialised version of the decks only - TODO remove when tested -
	 * @param PlayerNumber - The player number for the recipeint of this message (probably 0, the local player)
	 */
	const FString GetSerialised(uint32 PlayerNumber);
};

