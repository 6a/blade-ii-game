#pragma once

#include "CoreMinimal.h"

/* EPlayer represents different dealer events (such as on deck dealt, on card placed) */
UENUM(BlueprintType)
enum class EServerUpdate : uint8
{
	None UMETA(DisplayName = "No update"),
	CardElliotsOrbalStaff UMETA(DisplayName = "Played Elliot's Orbal Staff"),
	CardFiesTwinGunswords UMETA(DisplayName = "Played Fie's Twin Gunswords"),
	CardAlisasOrbalBow UMETA(DisplayName = "Played Alisa's Orbal Bow"),
	CardJusisSword UMETA(DisplayName = "Played Jusis' Sword"),
	CardMachiasOrbalShotgun UMETA(DisplayName = "Played Machias' Orbal Shotgun"),
	CardGaiusSpear UMETA(DisplayName = "Played Gaius' Spear"),
	CardLaurasGreatsword UMETA(DisplayName = "Played Laura's Greatsword"),
	CardBolt UMETA(DisplayName = "Played Bolt"),
	CardMirror UMETA(DisplayName = "Played Emma's Orbal Staff"),
	CardBlast UMETA(DisplayName = "Played Elise's Rapier"),
	CardForce UMETA(DisplayName = "Played Sara's Sword and Gun"),
	InstructionCards UMETA(DisplayName = "Initial Cards"),
	InstructionForfeit UMETA(DisplayName = "Forfeit"),
	InstructionMessage UMETA(DisplayName = "Message"),
	InstructionConnectionFailed UMETA(DisplayName = "Connection Failed"),
};