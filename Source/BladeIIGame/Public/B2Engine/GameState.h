#pragma once

#include "CoreMinimal.h"

#include "Cards.h"
#include "B2Misc/Enum.h"

struct B2GameState
{
	// Cards
	FB2Cards Cards;

	// Scores
	uint32 PlayerScore;
	uint32 OpponentScore;

	// Input
	bool bAcceptPlayerInput;
	ETableSlot CursorPosition;
	uint32 CursorSlotIndex;
};
