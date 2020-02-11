#pragma once
#include "CoreMinimal.h"
#include "Misc/DateTime.h"

namespace Utility
{
#define QDEBUG 1

	const void LogInfo(const FString& message);
	const void LogWarning(const FString& message);

	const FString GetTimestamp();
}