#include "Utility.h"

const void Utility::LogInfo(const FString& message)
{
#if QDEBUG
	UE_LOG(LogTemp, Warning, TEXT("[INFO   ]%s %s"), *Utility::GetTimestamp(), *message);
#endif // QDEBUG
}

const void Utility::LogWarning(const FString& message)
{
#if QDEBUG
	UE_LOG(LogTemp, Warning, TEXT("[WARNING]%s %s"), *Utility::GetTimestamp(), *message);
#endif // QDEBUG
}

const FString Utility::GetTimestamp()
{
	auto utcnow = FDateTime::UtcNow();
	return FString::Printf(TEXT("[%02d:%02d:%02d:%03d]"), utcnow.GetHour(), utcnow.GetMinute(), utcnow.GetSecond(), utcnow.GetMillisecond());
}