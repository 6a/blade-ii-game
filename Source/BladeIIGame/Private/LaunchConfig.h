#pragma once
#include "CoreMinimal.h"
#include "GenericPlatform/GenericWindow.h"

class B2LaunchConfig
{
public:
	FString Handle;
	FString PublicID;
	FString AuthToken;
	uint64 MatchID;
	FString Language;
	FIntPoint Resolution;
	EWindowMode::Type ScreenMode;
	bool VSyncOn;
	int32 AntiAliasing;
	int32 ShadowQuality;
	int32 PostProcessing;
	float MasterVolume;
	float BGMVolume;
	float SFXVolume;

	B2LaunchConfig(const FString& GameInfoFileName);
	~B2LaunchConfig();

private:
	/**
	 * Loads the specified file.
	 * @param FilePath - The absolute path to the file that should be loaded.
	 * @warning Will close the program if the file cannot be loaded properly.
	 */
	const FString FromFile(const FString& FilePath) const;

	/**
	 * Parses a string that represents the contents of a launch config file
	 * @param LaunchConfigRaw - The file contents as a string.
	 * @returns An error message, or an empty string if parsing was successful.
	 */
	const FString Parse(const FString& LaunchConfigRaw);
};

