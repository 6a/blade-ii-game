#pragma once

#include "CoreMinimal.h"

#include "Cards.h"
#include "B2Game/CardSlot.h"

struct B2GameState
{
	~B2GameState();

	// Cards
	FB2Cards Cards;

	// Scores
	uint32 PlayerScore;
	uint32 OpponentScore;

	// Input
	bool bAcceptPlayerInput;
	UCardSlot* CursorPosition;
	uint32 CursorSlotIndex;
};

