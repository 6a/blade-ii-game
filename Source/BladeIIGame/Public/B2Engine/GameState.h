#pragma once

#include "CoreMinimal.h"

#include "Cards.h"
#include "B2Misc/Enum.h"

struct B2GameState
{
	B2GameState(const FB2Cards& Cards);

	// Cards
	FB2Cards Cards;

	// Scores
	uint32 PlayerScore;
	uint32 OpponentScore;

	// Input
	bool bAcceptPlayerInput;
	ECardSlot CursorPosition;
	uint32 CursorSlotIndex;

	// Turn
	ETurn Turn;
};

