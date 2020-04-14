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
	Bolt UMETA(DisplayName = "Bolt"),
	Mirror UMETA(DisplayName = "Emma's Orbal Staff"),
	Blast UMETA(DisplayName = "Elise's Rapier"),
	Force UMETA(DisplayName = "Sara's Sword and Gun"),
	InactiveElliotsOrbalStaff UMETA(DisplayName = "Inactive Elliot's Orbal Staff"),
	InactiveFiesTwinGunswords UMETA(DisplayName = "Inactive Fie's Twin Gunswords"),
	InactiveAlisasOrbalBow UMETA(DisplayName = "Inactive Alisa's Orbal Bow"),
	InactiveJusisSword UMETA(DisplayName = "Inactive Jusis' Sword"),
	InactiveMachiasOrbalShotgun UMETA(DisplayName = "Inactive Machias' Orbal Shotgun"),
	InactiveGaiusSpear UMETA(DisplayName = "Inactive Gaius' Spear"),
	InactiveLaurasGreatsword UMETA(DisplayName = "Inactive Laura's Greatsword"),
	InactiveBolt UMETA(DisplayName = "Inactive Bolt"),
	InactiveMirror UMETA(DisplayName = "Inactive Emma's Orbal Staff"),
	InactiveBlast UMETA(DisplayName = "Inactive Elise's Rapier"),
	InactiveForce UMETA(DisplayName = "Inactive Sara's Sword and Gun"),
};