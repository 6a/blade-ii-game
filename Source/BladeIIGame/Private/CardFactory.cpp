#include "CardFactory.h"
#include "Utility.h"

QCardFactory::QCardFactory(CardFactoryConfig& config)
{
	Utility::LogInfo("Card Factory Initialized");
}

QCardFactory::~QCardFactory()
{

}

ACard* QCardFactory::Make(ECard card, const FVector& targetPosition)
{
	Utility::LogInfo(FString::Format(TEXT("Card Factory created card: [{0}]"), { CardEnumToString(card) }));
	return nullptr;
}

// Helpers

FString QCardFactory::CardEnumToString(ECard card)
{
	switch (card)
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

	return "";
}
