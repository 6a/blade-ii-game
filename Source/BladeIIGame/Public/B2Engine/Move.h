#pragma once

#include "CoreMinimal.h"

#include "B2Misc/Enum.h"
#include "Move.generated.h"

USTRUCT()
struct FB2Move
{
	GENERATED_BODY()

	/* The type of the card being moved */
	ECard Card;

	/* The current location of the card being moved */
	ECardSlot CurrentSlot;

	/* The target location of the card being moved */
	ECardSlot TargetSlot;
};

