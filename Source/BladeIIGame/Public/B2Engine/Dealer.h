#pragma once

#include "CoreMinimal.h"

#include "B2Game/Arena.h"

class B2Dealer
{
public:	
	
	/* Pointer to the arena */
	AArena* Arena;

	B2Dealer();

	/* Deal the cards out onto the arena. Can only be called once */
	void Deal();
private:
	
	/* Whether or not the cards have already been dealt (to avoid dealing twice) */
	bool bCardsDealt;
};

