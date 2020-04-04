#pragma once

#include "CoreMinimal.h"

#include "Cards.h"

struct B2BoardState
{
	FB2Cards Cards;

	uint32 PlayerScore;
	uint32 OpponentScore;
};

