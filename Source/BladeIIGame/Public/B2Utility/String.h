#pragma once

#include "CoreMinimal.h"

#include "Misc/DateTime.h"

#include "../B2Enum/CardEnum.h"

namespace B2Utility
{
	/* Returns the current timestap as an FString with the following format: hh:mm:ss:MMM */
	const FString GetTimestamp();

	/**
	 * Static helper function that returns the string representation of a card enum.
	 * @param Card - The card enum to return as a string.
	 * @return the card enum as a string
	 */
	const FString CardEnumToString(ECard Card);

	const FString UInt32ToHexString(uint32 InInt);
}