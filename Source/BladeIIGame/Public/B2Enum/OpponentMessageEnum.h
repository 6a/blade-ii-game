#pragma once

#include "CoreMinimal.h"

/* EOpponentMessage represents different "things" that the opponent can say */
UENUM(BlueprintType)
enum class EOpponentMessage : uint8
{
	Greeting UMETA(DisplayName = "Greeting"),
	Draw UMETA(DisplayName = "Draw"),
	PlayerGoesFirst UMETA(DisplayName = "Player Goes First"),
	OpponentGoesFirst UMETA(DisplayName = "Opponent Goes First"),
	MatchedScores UMETA(DisplayName = "Matched Scores"),
	Bolt UMETA(DisplayName = "Bolt"),
	Mirror UMETA(DisplayName = "Mirror"),
	Blast UMETA(DisplayName = "Blast"),
	Force UMETA(DisplayName = "Force"),
	Reaction UMETA(DisplayName = "Reaction"),
	Victory UMETA(DisplayName = "Victory"),
	Defeat UMETA(DisplayName = "Defeat"),
};