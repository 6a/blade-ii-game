/**
 * Note that this file is called ECard.h instead of Card.h. This is due to the unreal header tool detecting a circular dependency
 * when its included in another file called Card.h (from a different subfolder).
 */

#pragma once

#include "CoreMinimal.h"

/* ECard represents different types of card */
UENUM(BlueprintType)
enum class ECard : uint8
{
	ElliotsOrbalStaff UMETA(DisplayName = "Elliot's Orbal Staff"),
	FiesTwinGunswords UMETA(DisplayName = "Fie's Twin Gunswords"),
	AlisasOrbalBow UMETA(DisplayName = "Alisa's Orbal Bow"),
	JusisSword UMETA(DisplayName = "Jusis' Sword"),
	MachiasOrbalShotgun UMETA(DisplayName = "Machias' Orbal Shotgun"),
	GaiusSpear UMETA(DisplayName = "Gaius' Spear"),
	LaurasGreatsword UMETA(DisplayName = "Laura's Greatsword"),
	ReansTachi UMETA(DisplayName = "Rean's Tachi"),
	EmmasOrbalStaff UMETA(DisplayName = "Emma's Orbal Staff"),
	ElisesRapier UMETA(DisplayName = "Elise's Rapier"),
	SarasSwordAndGun UMETA(DisplayName = "Sara's Sword and Gun")
};