#include "Utility.h"

const void B2Utility::LogInfo(const FString& message)
{
#if B2_SHOW_DEBUG
	UE_LOG(LogTemp, Warning, TEXT("[INFO   ][%s] %s"), *B2Utility::GetTimestamp(), *message);
#endif
}

const void B2Utility::LogWarning(const FString& message)
{
#if B2_SHOW_DEBUG
	UE_LOG(LogTemp, Warning, TEXT("[WARNING][%s] %s"), *B2Utility::GetTimestamp(), *message);
#endif
}

const FString B2Utility::GetTimestamp()
{
	auto Now = FDateTime::UtcNow();
	return FString::Printf(TEXT("%02d:%02d:%02d:%03d"), Now.GetHour(), Now.GetMinute(), Now.GetSecond(), Now.GetMillisecond());
}