#pragma once

#include "CoreMinimal.h"

/* EEngineState represents the current state of the engine (as in, what it is doing - init, dealing, in-play etc */
UENUM(BlueprintType)
enum class EEngineState : uint8
{
	Initialisation UMETA(DisplayName = "Initialisation"),
	Dealing UMETA(DisplayName = "Dealing"),
	InPlay UMETA(DisplayName = "In Play"),
	PostGame UMETA(DisplayName = "Post Game"),
};