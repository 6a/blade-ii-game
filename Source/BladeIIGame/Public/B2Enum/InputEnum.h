#pragma once

#include "CoreMinimal.h"

/* EInput represents different buttons that the player can press to interect with the game */
UENUM(BlueprintType)
enum class EInput : uint8
{
	NavigateLeft UMETA(DisplayName = "Navigate Left"),
	NavigateRight UMETA(DisplayName = "Navigate Right"),
	Select UMETA(DisplayName = "Select")
};