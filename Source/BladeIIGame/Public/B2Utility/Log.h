#pragma once

#include "CoreMinimal.h"

#include "Misc/DateTime.h"

#define B2_SHOW_DEBUG 1

namespace B2Utility
{
	/**
	 * Logs a timestamped info message to the debug console.
	 * @param Message - The text to display.
	 */
	void LogInfo(const FString& Message);

	/**
	 * Logs a timestamped warning message to the debug console.
	 * @param Message - The text to display.
	 */
	void LogWarning(const FString& Message);

	/**
	 * Logs a timestamped bool to the debug console.
	 * @param bInBool - The bool to display.
	 */
	void LogBool(bool bInBool);
}