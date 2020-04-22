#pragma once

#include "CoreMinimal.h"

/* EAudioChannel represents different audio channels, such as master or bgm */
UENUM(BlueprintType)
enum class EAudioChannel : uint8
{
	Master UMETA(DisplayName = "Master"),
	BGM UMETA(DisplayName = "BGM"),
	SFX UMETA(DisplayName = "SFX"),
};