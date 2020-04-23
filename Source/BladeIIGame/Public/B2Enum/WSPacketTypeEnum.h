#pragma once

#include "CoreMinimal.h"

/* EWSPacketType represents websocket packet types */
UENUM(BlueprintType)
enum class EWSPacketType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Move = 100 UMETA(DisplayName = "Move"),
	Forfeit = 101 UMETA(DisplayName = "Forfeit"),
	Message = 102 UMETA(DisplayName = "Message"),
};
