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
	 * @param GameMode - A pointer to the current game mode
	 * @param LaunchConfig - The launch config read the settings from
	 */
	void Initialise(class ABladeIIGameMode* GameMode, const B2LaunchConfig& LaunchConfig);

	/**
	 * Get the specified float setting
	 * @param Setting - The setting to get
	 * @return The float setting
	 */
	float GetFloatSetting(EFloatSetting Setting) const;

	/**
	 * Set the specified float setting with the specified value
	 * @param Setting - The setting to set
	 * @param Value - The new value
	 */
	void SetFloatSetting(EFloatSetting Setting, float Value);

	/**
	 * Get the specified string setting
	 * @param Setting - The setting to get
	 * @return The string setting
	 */
	FString GetStringSetting(EStringSetting Setting) const;


	/**
	 * Set the specified string setting with the specified value
	 * @param Setting - The setting to set
	 * @param Value - The new value
	 */
	void SetStringSetting(EStringSetting Setting, const FString& Value);

	/* Returns true if the game is vs the AI */
	bool IsVersusAI() const;

private:

	/* Current settings data cache - changes only in memory, not on disc */
	B2LaunchConfig SettingsCache;

	/* The version string */
	FString VersionString;

	/* Reference to the game mode instance */
	class ABladeIIGameMode* GameModeInstance;
};
