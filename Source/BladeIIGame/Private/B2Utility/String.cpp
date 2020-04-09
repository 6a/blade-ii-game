#include "B2Utility/String.h"

const FString B2Utility::GetTimestamp()
{
	auto Now = FDateTime::UtcNow();
	return FString::Printf(TEXT("%02d:%02d:%02d:%03d"), Now.GetHour(), Now.GetMinute(), Now.GetSecond(), Now.GetMillisecond());
}

const FString B2Utility::CardEnumToString(ECard Card)
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
	case ECard::Bolt:
		return "08 Bolt";
	case ECard::Mirror:
		return "09 Mirror";
	case ECard::Blast:
		return "10 Blast";
	case ECard::Force:
		return "11 Force";
	}

	return "00 Unknown";
}
