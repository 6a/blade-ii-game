#pragma once

#include "CoreMinimal.h"

#include "Misc/DateTime.h"

#include "../B2Enum/CardEnum.h"

#define B2_SHOW_DEBUG 1

static class B2Utility
{
public:
	/**
	 * Logs a timestamped info message to the debug console.
	 * @param Message - The text to display.
	 */
	const static void LogInfo(const FString& Message);

	/**
	 * Logs a timestamped warning message to the debug console.
	 * @param Message - The text to display.
	 */
	const static void LogWarning(const FString& Message);

	/**
	 * Logs a timestamped bool to the debug console.
	 * @param bInBool - The bool to display.
	 */
	const static void LogBool(bool bInBool);

	/* Returns the current timestap as string in the following format: hh:mm:ss:MMM */
	const static FString GetTimestamp();

	/**
	 * Static helper function that returns the string representation of a card enum.
	 * @param Card - The card enum to return as a string.
	 * @return the card enum as a string
	 */
	const static FString EnumToString(ECard Card);

};