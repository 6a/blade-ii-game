#pragma once

#include "CoreMinimal.h"

/* EFloatSetting represents different slots on the table (deck, hand, field or discard) */
UENUM(BlueprintType)
enum class EFloatSetting : uint8
{
	MasterVolume UMETA(DisplayName = "Master Volume"),
	BGMVolume UMETA(DisplayName = "BGM Volume"),
	SFXVolume UMETA(DisplayName = "SFX Volume"),
};

/* EStringSetting represents different slots on the table (deck, hand, field or discard) */
UENUM(BlueprintType)
enum class EStringSetting : uint8
{
	Version UMETA(DisplayName = "Version"),
	Language UMETA(DisplayName = "Version"),
};