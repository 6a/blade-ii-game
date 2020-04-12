#pragma once

#include "CoreMinimal.h"

#include "Cards.h"
#include "B2Enum/CardSlotEnum.h"
#include "B2Enum/PlayerEnum.h"

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
	EPlayer Turn;

	// Blast effect animation (card destruction) pending bool (should be set to false once checked)
	bool bBlastAnimationPending;

	// The ID of the most recently played blast card (so that it can be removed from the hand it is in)
	FString MostRecentBlastCardID;
};

