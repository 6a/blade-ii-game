#pragma once

#include "CoreMinimal.h"

/* EPlayer represents different dealer events (such as on deck dealt, on card placed) */
UENUM(BlueprintType)
enum class EPlayer : uint8
{
	Undecided UMETA(DisplayName = "Undecided"),
	Player UMETA(DisplayName = "Player"),
	Opponent UMETA(DisplayName = "Opponent")
};