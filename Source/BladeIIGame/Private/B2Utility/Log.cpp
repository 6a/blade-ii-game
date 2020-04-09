#include "B2Utility/Log.h"
#include "B2Utility/String.h"

void B2Utility::LogInfo(const FString& message)
{
#if B2_SHOW_DEBUG
	UE_LOG(LogTemp, Warning, TEXT("[INFO][%s] %s"), *GetTimestamp(), *message);
#endif
}

void B2Utility::LogWarning(const FString& message)
{
#if B2_SHOW_DEBUG
	UE_LOG(LogTemp, Warning, TEXT("[WARNING][%s] %s"), *GetTimestamp(), *message);
#endif
}

void B2Utility::LogBool(bool bInBool)
{
	LogWarning(FString::Printf(TEXT("Bool Value: %s"), bInBool ? TEXT("True") : TEXT("False")));
}