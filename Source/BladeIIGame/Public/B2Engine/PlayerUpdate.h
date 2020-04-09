#pragma once

#include "CoreMinimal.h"

#include "B2Engine/Move.h"
#include "B2Engine/Cards.h"
#include "B2Enum/PayloadEnum.h"
#include "B2Enum/InstructionEnum.h"

struct B2PlayerUpdate
{
	/* The type dictates the payload if you check the wrong payload you will end up with a default value container / enum */
	EPayload Type;

	// Instruction type updates
	EInstruction Instruction;

	// Move type updates
	FB2Move Move;
};

