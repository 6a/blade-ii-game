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

	// Blast stuff

	// Blast effect animation (card destruction) pending bool (should be set to false once checked)
	bool bBlastAnimationPending;

	// The ID of the most recently played blast card (so that it can be removed from the hand it is in)
	FString MostRecentBlastCardID;

	// The type of card that has most recently been blased (so that it can be removed from the hand that it is in)
	ECard MostRecentBlastedCard;

	// Whether or not we game state processing state needs to handle the edge case for post-blast select states
	bool bHandleBlastEdgeCase;

	// Timer carry-over for the separate blast states. Set to -1 to ignore.
	float BlastTurnEndTime;
};

