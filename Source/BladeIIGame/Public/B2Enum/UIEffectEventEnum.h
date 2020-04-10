#pragma once

#include "CoreMinimal.h"

/* EUIEffectEvent represents different effects from the UI Effect widget */
UENUM(BlueprintType)
enum class EUIEffectEvent : uint8
{
	Ready UMETA(DisplayName = "Read"),
	Finished UMETA(DisplayName = "Finished"),
};