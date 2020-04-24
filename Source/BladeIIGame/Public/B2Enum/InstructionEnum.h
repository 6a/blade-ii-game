#pragma once

#include "CoreMinimal.h"

/* EInstruction represents instructions.. Actually im not sure TODO is this used? What for? */
UENUM(BlueprintType)
enum class EInstruction : uint8
{
	Win UMETA(DisplayName = "Game Win"),
	Draw UMETA(DisplayName = "Game Draw"),
	Loss UMETA(DisplayName = "Game Loss"),
	OpponentQuit UMETA(DisplayName = "Opponent Quit")
};