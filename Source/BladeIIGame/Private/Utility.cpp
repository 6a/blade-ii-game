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

const FString B2Utility::EnumToString(ECard Card)
{
	switch (Card)
	{
	case ECard::ElliotsOrbalStaff:
		return "01 Elliot's Orbal Staff";
	case ECard::FiesTwinGunswords:
		return "02 Fie's Twin Gunswords";
	case ECard::AlisasOrbalBow:
		return "03 Alisa's Orbal Bow";
	case ECard::JusisSword:
		return "04 Jusis' Sword";
	case ECard::MachiasOrbalShotgun:
		return "05 Machias' Orbal Shotgun";
	case ECard::GaiusSpear:
		return "06 Gaius' Spear";
	case ECard::LaurasGreatsword:
		return "07 Laura's Greatsword";
	case ECard::ReansTachi:
		return "08 Rean's Tachi";
	case ECard::EmmasOrbalStaff:
		return "09 Emma's Orbal Staff";
	case ECard::ElisesRapier:
		return "10 Elise's Rapier";
	case ECard::SarasSwordAndGun:
		return "11 Sara's Sword and Gun";
	}

	return "00 Unknown";
}