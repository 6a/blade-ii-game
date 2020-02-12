#include "MatchInfo.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Utility.h"

B2MatchInfo::B2MatchInfo(const FString& GameInfoFileName)
{
	FString GameInfoFile = LoadFile(GameInfoFileName);
	ensureMsgf(!GameInfoFile.IsEmpty(), TEXT("Match info file could not be loaded"));


	B2Utility::LogInfo("Match info loaded and parsed successfully");
}

B2MatchInfo::~B2MatchInfo()
{
}

FString B2MatchInfo::LoadFile(const FString & FilePath) const
{
	// Determine full file path
	FString Directory = FPaths::Combine(FPaths::GameContentDir(), TEXT("Data"));

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
