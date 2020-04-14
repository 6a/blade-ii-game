#pragma once

#include "CoreMinimal.h"

/* EPlayer represents different dealer events (such as on deck dealt, on card placed) */
UENUM(BlueprintType)
enum class EWinCondition : uint8
{
	None UMETA(DisplayName = "No Win"),
	OpponentOnlyHasEffectCards UMETA(DisplayName = "Opponent Only Has Effect Cards"),
	ScoreVictory UMETA(DisplayName = "Score Victory"),
};