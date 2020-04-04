#pragma once

#include "CoreMinimal.h"

#include "B2Engine/Move.h"
#include "B2Engine/Cards.h"
#include "B2Misc/Enum.h"

struct B2ServerUpdate
{
	/* The type dictates the payload if you check the wrong payload you will end up with a default value container / enum */
	EPayload Type;

	// Instruction type updates
	EInstruction Instruction;

	// Move type updates
	FB2Move Move;

	// Cards type updates
	FB2Cards Cards;
};

