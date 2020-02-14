#pragma once
#include "CoreMinimal.h"

/* ETableSlot represents different slots on the table (deck, hand, field or discard) */
UENUM(BlueprintType)
enum class EInstruction : uint8
{
	Win UMETA(DisplayName = "Game Win"),
	Draw UMETA(DisplayName = "Game Draw"),
	Loss UMETA(DisplayName = "Game Loss"),
	OpponentQuit UMETA(DisplayName = "Opponent Quit")
};

