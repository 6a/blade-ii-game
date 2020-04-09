#pragma once

#include "CoreMinimal.h"

#include "B2Misc/Enum/CardEnum.h"
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
};

