#pragma once
#include "CoreMinimal.h"
#include "Misc/DateTime.h"

namespace B2Utility
{
#define QDEBUG 1
	/**
	 * Logs a timestamped info message to the debug console.
	 * @param Message - The text to display.
	 */
	const void LogInfo(const FString& Message);

	/**
	 * Logs a timestamped warning message to the debug console.
	 * @param Message - The text to display.
	 */
	const void LogWarning(const FString& Message);

	/**
	 * Logs a timestamped warning message to the debug console.
	 * @param Message - The text to display.
	 */
	const FString GetTimestamp();
}