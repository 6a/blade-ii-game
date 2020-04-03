#pragma once

#include "CoreMinimal.h"

#include "B2Game/Arena.h"

class B2Dealer
{
public:
	void Deal();

	/* Pointer to the arena */
	AArena* Arena;

private:
	
	/* Whether or not the cards have already been dealt (to avoid dealing twice) */
	bool bCardsDealt;
};

