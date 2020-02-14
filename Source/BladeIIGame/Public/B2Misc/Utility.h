#pragma once

#include "CoreMinimal.h"

#include "Misc/DateTime.h"

#define B2_SHOW_DEBUG 1

/* ETableSlot represents different slots on the table (deck, hand, field or discard) */
UENUM(BlueprintType)
enum class ETableSlot : uint8
{
	Deck UMETA(DisplayName = "Deck"),
	Hand UMETA(DisplayName = "Hand"),
	Field UMETA(DisplayName = "Field"),
	Discard UMETA(DisplayName = "Discard")
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

static class B2Utility
{
public:
	/**
	 * Logs a timestamped info message to the debug console.
	 * @param Message - The text to display.
	 */
	const static void LogInfo(const FString& Message);

	/**
	 * Logs a timestamped warning message to the debug console.
	 * @param Message - The text to display.
	 */
	const static void LogWarning(const FString& Message);

	/* Returns the current timestap as string in the following format: hh:mm:ss:MMM */
	const static FString GetTimestamp();

	/**
	 * Static helper function that returns the string representation of a card enum.
	 * @param Card - The card enum to return as a string.
	 * @return the card enum as a string
	 */
	const static FString EnumToString(ECard Card);

};