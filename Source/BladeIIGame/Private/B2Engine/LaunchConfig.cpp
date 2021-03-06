#include "B2Engine/LaunchConfig.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

#include "B2Utility/Log.h"

const FString CONFIG_DIRECTORY = TEXT("BladeIIGame/Data");
const TCHAR* DELIMITER_CONFIG = TEXT(":");
const FString DELIMITER_RESOLUTION = TEXT("x");
const int32 CONFIG_VALUES_NUM = 13;

const FString PARSE_ERROR_MISSING_OR_EMPTY_VALUES = TEXT("Some values were missing or empty");
const FString PARSE_ERROR_MATCH_ID_NOT_INT = TEXT("Match ID must be an integer");
const FString PARSE_ERROR_SCREEN_MODE_NOT_INT = TEXT("Screen mode must be an integer");
const FString PARSE_ERROR_VSYNC_NOT_INT = TEXT("VSync must be an integer");
const FString PARSE_ERROR_ANTI_ALIASING_NOT_INT = TEXT("Anti aliasing must be an integer");
const FString PARSE_ERROR_SHADOW_QUALITY_NOT_INT = TEXT("Shadow quality must be an integer");
const FString PARSE_ERROR_POST_PROCESSING_NOT_INT = TEXT("Post-processing must be an integer");
const FString PARSE_ERROR_MASTER_VOLUME_NOT_FLOAT = TEXT("Master volume must be an float");
const FString PARSE_ERROR_BGM_VOLUME_NOT_FLOAT = TEXT("BGM volume must be an float");
const FString PARSE_ERROR_SFX_VOLUME_NOT_FLOAT = TEXT("SFX volume must be an float");
const FString PARSE_ERROR_RESOLUTION_BAD_FORMAT = TEXT("Resolution format is invalid");
const FString PARSE_ERROR_NONE = TEXT("");
const FString DEFAULT_LAUNCH_CONFIG = TEXT("x:x:0:en:1920x1080:1:0:3:3:3:80:80:80");
const FString OUTPUT_FILE_SUFFIX = TEXT("_Out");

B2LaunchConfig::B2LaunchConfig(const FString& InLaunchConfigFile)
{
	LaunchConfigFileName = InLaunchConfigFile;

	// Get the raw content of the launch config file
	FString LaunchConfigRaw = FromFile(InLaunchConfigFile);

	// Exit if the contents was not loaded properly
	if (LaunchConfigRaw.IsEmpty())
	{
		B2Utility::LogWarning(TEXT("Launch config file could not be loaded - loading default values"));
		LaunchConfigRaw = DEFAULT_LAUNCH_CONFIG;
	}

	// Parse the launch config file
	FString Error = Parse(LaunchConfigRaw);

	// Exit if the values could not be parsed properly, with the returned error message
	ensureMsgf(Error.IsEmpty(), TEXT("Launch config file could not be parsed: %s"), *Error);

	bIsBotGame = MatchID <= MATCH_ID_AI_GAME_THRESHOLD;
	bIsTutorialGame = MatchID == MATCH_ID_AI_GAME_THRESHOLD;

	B2Utility::LogInfo("Launch config loaded and parsed successfully");
}

B2LaunchConfig::~B2LaunchConfig()
{
	SaveSettings();
}

bool B2LaunchConfig::SaveSettings()
{
	if (!WITH_EDITOR &&!LaunchConfigFileName.IsEmpty())
	{
		// Determine full file path
		FString Directory = FPaths::Combine(FPaths::ProjectContentDir(), CONFIG_DIRECTORY);

		// Create a PlatformFile instance
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

		// Check that the directory exists
		if (PlatformFile.CreateDirectory(*Directory)) {
			// Determine the absolute path for the file
			FString AbsoluteFileName = Directory + "/" + LaunchConfigFileName;

			// Determine the file name for the output file
			FString Extension = FPaths::GetExtension(AbsoluteFileName);
			FString FinalFileName;
			if (Extension.IsEmpty()) 
			{
				FinalFileName = AbsoluteFileName.Append(OUTPUT_FILE_SUFFIX);
			} 
			else
			{
				Extension = FString(".").Append(Extension);

				FinalFileName = AbsoluteFileName.Replace(*Extension, *OUTPUT_FILE_SUFFIX);
				FinalFileName.Append(Extension);
			}

			// Write the file at the specified path
			return FFileHelper::SaveStringToFile(MakeSettingsOutputString(), *FinalFileName);
		}
	}

	return false;
}

const FString B2LaunchConfig::FromFile(const FString& FilePath) const
{
	// Determine full file path
	FString Directory = FPaths::Combine(FPaths::ProjectContentDir(), CONFIG_DIRECTORY);

	// Create a PlatformFile instance
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// Check that the directory exists
	if (PlatformFile.CreateDirectory(*Directory)) {
		// Determine the absolute path for the file
		FString AbsoluteFileName = Directory + "/" + FilePath;

		// Load the file at the specified path
		FString FileContents;
		FFileHelper::LoadFileToString(FileContents, *AbsoluteFileName);

		return FileContents;
	}

	return FString();
}

const FString B2LaunchConfig::Parse(const FString& LaunchConfigRaw)
{
	// Parse into a string array
	TArray<FString> StringValues;
	LaunchConfigRaw.ParseIntoArray(StringValues, DELIMITER_CONFIG);

	if (StringValues.Num() != CONFIG_VALUES_NUM)
	{
		return PARSE_ERROR_MISSING_OR_EMPTY_VALUES;
	}

	// Set all the string values first
	PublicID = StringValues[0];
	AuthToken = StringValues[1];
	Language = StringValues[3];

	// Set the numerical values after validating
	// Early exit if the value is not numeric, and return an appropriate error message

	// Match ID
	if (!StringValues[2].IsNumeric()) return PARSE_ERROR_MATCH_ID_NOT_INT;
	MatchID = FCString::Atoi(*StringValues[2]);

	// Screen mode
	if (!StringValues[5].IsNumeric()) return PARSE_ERROR_SCREEN_MODE_NOT_INT;
	ScreenMode = EWindowMode::ConvertIntToWindowMode(FCString::Atoi(*StringValues[5]));

	// VSync
	if (!StringValues[6].IsNumeric()) return PARSE_ERROR_VSYNC_NOT_INT;
	VSyncOn = FCString::Atoi(*StringValues[6]) == 1;

	// Anti Aliasing
	if (!StringValues[7].IsNumeric()) return PARSE_ERROR_ANTI_ALIASING_NOT_INT;
	AntiAliasing = FCString::Atoi(*StringValues[7]);

	// Shadow Quality
	if (!StringValues[8].IsNumeric()) return PARSE_ERROR_SHADOW_QUALITY_NOT_INT;
	ShadowQuality = FCString::Atoi(*StringValues[8]);

	// Post Processing
	if (!StringValues[9].IsNumeric()) return PARSE_ERROR_POST_PROCESSING_NOT_INT;
	PostProcessing = FCString::Atoi(*StringValues[9]);

	// Master Volume
	if (!StringValues[10].IsNumeric()) return PARSE_ERROR_MASTER_VOLUME_NOT_FLOAT;
	MasterVolume = FCString::Atof(*StringValues[10]);

	// BGM Volume
	if (!StringValues[11].IsNumeric()) return PARSE_ERROR_BGM_VOLUME_NOT_FLOAT;
	BGMVolume = FCString::Atof(*StringValues[11]);

	// SFX Volume
	if (!StringValues[12].IsNumeric()) return PARSE_ERROR_SFX_VOLUME_NOT_FLOAT;
	SFXVolume = FCString::Atof(*StringValues[12]);

	// Parse resolution in two steps - first, split the config value at the delimiter, and then validate + parse the individual values
	FString ResolutionX, ResolutionY;
	if (!StringValues[4].Split(DELIMITER_RESOLUTION, &ResolutionX, &ResolutionY)) return PARSE_ERROR_RESOLUTION_BAD_FORMAT;
	if (!ResolutionX.IsNumeric() || !ResolutionY.IsNumeric()) return PARSE_ERROR_RESOLUTION_BAD_FORMAT;
	Resolution = FIntPoint(FCString::Atoi(*ResolutionX), FCString::Atoi(*ResolutionY));

	// If we reached this point the parsing was successful, so we return and inform the caller that the parsing succeeded
	return PARSE_ERROR_NONE;
}

const FString B2LaunchConfig::MakeSettingsOutputString() const
{
	FString OutString = FString::Printf(TEXT("%s:%f:%f:%f"), *Language, MasterVolume, BGMVolume, SFXVolume);

	return OutString;
}
