#pragma once

#include "CoreMinimal.h"

/* EResponseType represents different types of response from, for example, a yes/no prompt modal */
UENUM(BlueprintType)
enum class EResponseType : uint8
{
	Positive UMETA(DisplayName = "Positive"),
	Negative UMETA(DisplayName = "Negative"),
};