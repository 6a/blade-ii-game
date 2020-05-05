#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericWindow.h"

/* Utility class for reading and parsing the launch config */
class B2LaunchConfig
{
public:
	FString PublicID;
	FString AuthToken;
	uint64 MatchID;
	FString Language;
	FIntPoint Resolution;
	EWindowMode::Type ScreenMode;
	bool VSyncOn;
	int8 AntiAliasing;
	int8 ShadowQuality;
	int8 PostProcessing;
	float MasterVolume;
	float BGMVolume;
	float SFXVolume;
	bool bIsBotGame;
	bool bIsTutorialGame;
	 
	/* 
	 * A MatchID less than this value indicates that the match is a bot game of (value) difficulty.
	 * A value equal to this value indicates that the match is a tutorial game
	 * @note - Currently the AI opponent has only one difficulty, so any value under this value
	 * will be an AI game of the standard difficulty
	*/
	const static uint64 MATCH_ID_AI_GAME_THRESHOLD = 10;

	B2LaunchConfig() {}
	B2LaunchConfig(const FString& GameInfoFileName);
	~B2LaunchConfig();

private:
	/**
	 * Loads the specified file.
	 * @param FilePath - The absolute path to the file that should be loaded.
	 * @returns The content of the file
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

