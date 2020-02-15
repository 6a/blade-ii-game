#pragma once

#include "CoreMinimal.h"

/* ETableSlot represents different slots on the table (deck, hand, field or discard) */
UENUM(BlueprintType)
enum class ETableSlot : uint8
{
	Deck UMETA(DisplayName = "Deck"),
	Hand UMETA(DisplayName = "Hand"),
	Field UMETA(DisplayName = "Field"),
	Discard UMETA(DisplayName = "Discard")
};