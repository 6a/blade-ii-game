#pragma once
#include "CoreMinimal.h"
#include "Misc/DateTime.h"

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

	/* Returns the current timestap as string in the following format: hh:mm:ss:MMM */
	const static FString GetTimestamp();

};