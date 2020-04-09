#pragma once

#include "CoreMinimal.h"

/* ETableSlot represents different slots on the table (deck, hand, field or discard) */
UENUM(BlueprintType)
enum class ECardSlot : uint8
{
	PlayerDeck UMETA(DisplayName = "Player Deck"),
	PlayerHand UMETA(DisplayName = "Player Hand"),
	PlayerField UMETA(DisplayName = "Player Field"),
	PlayerDiscard UMETA(DisplayName = "Player Discard"),
	OpponentDeck UMETA(DisplayName = "Opponent Deck"),
	OpponentHand UMETA(DisplayName = "Opponent Hand"),
	OpponentField UMETA(DisplayName = "Opponent Field"),
	OpponentDiscard UMETA(DisplayName = "Opponent Discard"),
};