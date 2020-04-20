#pragma once

#include "CoreMinimal.h"

/* EPlayer represents different dealer events (such as on deck dealt, on card placed) */
UENUM(BlueprintType)
enum class EOpponentMessage : uint8
{
	Greeting UMETA(DisplayName = "Greeting"),
	Draw UMETA(DisplayName = "Draw"),
	PlayerGoesFirst UMETA(DisplayName = "Player Goes First"),
	OpponentGoesFirst UMETA(DisplayName = "Opponent Goes First"),
	MatchedScores UMETA(DisplayName = "Matched Scores"),
	Rod UMETA(DisplayName = "Rod"),
	Bolt UMETA(DisplayName = "Bolt"),
	Mirror UMETA(DisplayName = "Mirror"),
	Blast UMETA(DisplayName = "Blast"),
	Force UMETA(DisplayName = "Force"),
	Ouch UMETA(DisplayName = "Ouch"),
	Victory UMETA(DisplayName = "Victory"),
	Defeat UMETA(DisplayName = "Defeat"),
};