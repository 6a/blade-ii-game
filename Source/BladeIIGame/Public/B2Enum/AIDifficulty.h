#pragma once

#include "CoreMinimal.h"

/* EAIDifficulty represents different difficulties for the AI opponent */
UENUM(BlueprintType)
enum class EAIDifficulty : uint8
{
	Intermediate UMETA(DisplayName = "Intermediate"),
	Expert UMETA(DisplayName = "Expert")
};