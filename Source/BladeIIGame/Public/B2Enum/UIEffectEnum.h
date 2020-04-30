#pragma once

#include "CoreMinimal.h"

/* EEffect represents different UI effects, such as blast, and victory/defeat */
UENUM(BlueprintType)
enum class EUIEffect : uint8
{
	Rod UMETA(DisplayName = "Rod"),
	Bolt UMETA(DisplayName = "Bolt Target"),
	Mirror UMETA(DisplayName = "Mirror"),
	Blast UMETA(DisplayName = "Blast"),
	BlastTarget UMETA(DisplayName = "Blast Target"),
	Force UMETA(DisplayName = "Force"),
	Draw UMETA(DisplayName = "Draw"),
	Victory UMETA(DisplayName = "Win"),
	Defeat UMETA(DisplayName = "Loss"),
};