#pragma once

#include "CoreMinimal.h"

/* EPayload represents the various types of data once can send/receive to and from a server */
UENUM(BlueprintType)
enum class EPayload : uint8
{
	WaitingForInitialDeal UMETA(DisplayName = "None"),
	Cards UMETA(DisplayName = "Cards"),
	Move UMETA(DisplayName = "Move"),
	Instruction UMETA(DisplayName = "Instruction")
};