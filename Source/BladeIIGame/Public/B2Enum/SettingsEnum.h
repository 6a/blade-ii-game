#pragma once

#include "CoreMinimal.h"

/* EFloatSetting represents different float settings, such as volume */
UENUM(BlueprintType)
enum class EFloatSetting : uint8
{
	MasterVolume UMETA(DisplayName = "Master Volume"),
	BGMVolume UMETA(DisplayName = "BGM Volume"),
	SFXVolume UMETA(DisplayName = "SFX Volume"),
};

/* EStringSetting represents different string settings, such as language */
UENUM(BlueprintType)
enum class EStringSetting : uint8
{
	Version UMETA(DisplayName = "Version"),
	Language UMETA(DisplayName = "Language"),
	PublicID UMETA(DisplayName = "Public ID"),
	AuthToken UMETA(DisplayName = "AuthToken"),
};

/* EIntSetting represents different uint64 settings, such as match id */
UENUM(BlueprintType)
enum class EIntSetting : uint8
{
	MatchID UMETA(DisplayName = "Match ID"),
};