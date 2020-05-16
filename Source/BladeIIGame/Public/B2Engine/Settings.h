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

	/**
	 * Get the specified int setting
	 * @param Setting - The setting to get
	 * @return The int setting
	 */
	uint64 GetIntSetting(EIntSetting Setting) const;

	/**
	 * Set the specified int setting with the specified value
	 * @param Setting - The setting to set
	 * @param Value - The new value
	 */
	void SetIntSetting(EIntSetting Setting, uint64 Value);

	/* Returns true if the game is vs the AI */
	bool IsVersusAI() const;

	/* Returns true if the game is a tutorial */
	bool IsTutorial() const;

	/* Applies all the stored settings */
	void ApplyAll();

	/* Writes all the settings to the output settings file */
	bool SaveSettings();

	/* Updates the settings so that they no longer return true for IsTutorial() */
	void TutorialFinished();

private:

	/* Utility consts */
	const FString SHORT_LOCALE_EN = "en";
	const FString SHORT_LOCALE_JP = "ja-JP";
	const FString SHORT_LOCALE_DEFAULT = "en";
	const FString LOCALE_EN = "English";
	const FString LOCALE_JP = TEXT("日本語");
	const FString LOCALE_DEFAULT = "English";

	/* Current settings data cache - changes only in memory, not on disc */
	B2LaunchConfig SettingsCache;

	/* The version string */
	FString VersionString;

	/* Reference to the game mode instance */
	UPROPERTY()
	class ABladeIIGameMode* GameModeInstance;

	/**
	 * Returns the specified short locale string as a full string (the full word, i.e. en -> English)
	 * @param LocaleString - The locale string to convert
	 * @return The full locale string
	 */
	FString ShortLocaleStringToFull(const FString& ShortLocaleString) const;

	/**
	 * Returns the specified full locale string as a short string (the full word, i.e. English -> en)
	 * @param LocaleString - The locale string to convert
	 * @return The full locale string
	 */
	FString FullLocaleStringToShort(const FString& FullLocaleString) const;
};
