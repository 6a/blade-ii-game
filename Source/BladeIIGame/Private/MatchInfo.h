#pragma once
#include "CoreMinimal.h"

class B2MatchInfo
{
public:
	B2MatchInfo(const FString& GameInfoFileName);
	~B2MatchInfo();

private:
	/**
	 * Loads the specified file.
	 * @param FilePath - The absolute path to the file that should be loaded.
	 * @warning Will close the program if the file cannot be loaded properly.
	 */
	FString LoadFile(const FString& FilePath) const;
};

