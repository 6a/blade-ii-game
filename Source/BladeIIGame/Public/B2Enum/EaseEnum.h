#pragma once

#include "CoreMinimal.h"

/* EEase represents different ease types */
UENUM(BlueprintType)
enum class EEase : uint8
{
	EaseIn UMETA(DisplayName = "Ease In"),
	EaseInOut UMETA(DisplayName = "Ease In Out"),
	EaseOut UMETA(DisplayName = "Ease Out"),
	Linear UMETA(DisplayName = "Linear"),
};