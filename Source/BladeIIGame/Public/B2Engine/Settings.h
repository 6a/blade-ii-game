#pragma once

#include "CoreMinimal.h"

#include "B2Enum/SettingsEnum.h"
#include "B2Engine/LaunchConfig.h"

#include "Settings.generated.h"

UCLASS()
class BLADEIIGAME_API USettings : public UObject
{
	GENERATED_BODY()
	
public:
	USettings() {}

	/**
	 * Initialise this settings objects
	 * @param LaunchConfig - The launch config read the settings from
	 */
	void Initialise(const B2LaunchConfig& LaunchConfig);

	/**
	 * Get the specified float setting
	 * @param Setting - The setting to get
	 * @return The float setting
	 */
	float GetFloatSetting(EFloatSetting Setting) const;

	/**
	 * Get the specified string setting
	 * @param Setting - The setting to get
	 * @return The string setting
	 */
	FString GetStringSetting(EStringSetting Setting) const;

	/* Returns true if the game is vs the AI */
	bool IsVersusAI() const;

private:

	/* Current settings data cache - changes only in memory, not on disc */
	B2LaunchConfig SettingsCache;

	/* The version string */
	FString VersionString;
};
