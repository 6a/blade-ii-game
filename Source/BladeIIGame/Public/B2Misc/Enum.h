#pragma once

#include "CoreMinimal.h"

/* EAIDifficulty represents different difficulties for the AI opponent */
UENUM(BlueprintType)
enum class EAIDifficulty : uint8
{
	Intermediate UMETA(DisplayName = "Intermediate"),
	Expert UMETA(DisplayName = "Expert")
};

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

/* ETableSlot represents different slots on the table (deck, hand, field or discard) */
UENUM(BlueprintType)
enum class EInstruction : uint8
{
	Win UMETA(DisplayName = "Game Win"),
	Draw UMETA(DisplayName = "Game Draw"),
	Loss UMETA(DisplayName = "Game Loss"),
	OpponentQuit UMETA(DisplayName = "Opponent Quit")
};

/* ETableSlot represents different slots on the table (deck, hand, field or discard) */
UENUM(BlueprintType)
enum class ETableSlot : uint8
{
	Deck UMETA(DisplayName = "Deck"),
	Hand UMETA(DisplayName = "Hand"),
	Field UMETA(DisplayName = "Field"),
	Discard UMETA(DisplayName = "Discard")
};