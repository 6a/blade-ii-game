#pragma once

#include "CoreMinimal.h"

/* ETurn represents different dealer events (such as on deck dealt, on card placed) */
UENUM(BlueprintType)
enum class ETurn : uint8
{
	Undecided UMETA(DisplayName = "Undecided"),
	Player UMETA(DisplayName = "Player"),
	Opponent UMETA(DisplayName = "Opponent")
};