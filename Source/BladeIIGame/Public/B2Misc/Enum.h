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
	Bolt UMETA(DisplayName = "Bolt"),
	Mirror UMETA(DisplayName = "Emma's Orbal Staff"),
	Blast UMETA(DisplayName = "Elise's Rapier"),
	Force UMETA(DisplayName = "Sara's Sword and Gun")
};

/* EEase represents different ease types */
UENUM(BlueprintType)
enum class EEase : uint8
{
	EaseIn UMETA(DisplayName = "Ease In"),
	EaseInOut UMETA(DisplayName = "Ease In Out"),
	EaseOut UMETA(DisplayName = "Ease Out"),
	Linear UMETA(DisplayName = "Linear"),
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
enum class ECardSlot : uint8
{
	PlayerDeck UMETA(DisplayName = "Player Deck"),
	PlayerHand UMETA(DisplayName = "Player Hand"),
	PlayerField UMETA(DisplayName = "Player Field"),
	PlayerDiscard UMETA(DisplayName = "Player Discard"),
	OpponentDeck UMETA(DisplayName = "Opponent Deck"),
	OpponentHand UMETA(DisplayName = "Opponent Hand"),
	OpponentField UMETA(DisplayName = "Opponent Field"),
	OpponentDiscard UMETA(DisplayName = "Opponent Discard"),
};

/* EDealerEvent represents different dealer events (such as on deck dealt, on card placed) */
UENUM(BlueprintType)
enum class EDealerEvent : uint8
{
	CardsDealt UMETA(DisplayName = "Cards Dealt"),
	CardPlaced UMETA(DisplayName = "Card Placed")
};

/* EEngineState represents the current state of the engine (as in, what it is doing - init, dealing, in-play etc */
UENUM(BlueprintType)
enum class EEngineState : uint8
{
	Initialisation UMETA(DisplayName = "Initialisation"),
	Dealing UMETA(DisplayName = "Dealing"),
	InPlay UMETA(DisplayName = "In Play"),
	PostGame UMETA(DisplayName = "Post Game"),
};

/* EGamePhase represents different phases of the actual game, such as both players drawing onto the field, or waiting for the opponents move */
UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	None UMETA(DisplayName = "Default phase"),
	DrawToEmptyField UMETA(DisplayName = "Drawing from the deck to an empty field"),
	PlayerTurn UMETA(DisplayName = "The players turn"),
	WaitingForOpponentMove UMETA(DisplayName = "Waiting for the opponents move to be received"),
	SelectingFromPlayerHand UMETA(DisplayName = "Selecting a card from the local players hand"),
	SelectingFromOpponentHand UMETA(DisplayName = "Selecting a card from the opponents hand")
};

/* EPayload represents the various types of data once can send/receive to and from a server */
UENUM(BlueprintType)
enum class EPayload : uint8
{
	None UMETA(DisplayName = "None"),
	Cards UMETA(DisplayName = "Cards"),
	Move UMETA(DisplayName = "Move"),
	Instruction UMETA(DisplayName = "Instruction")
};

/* EButton represents different buttons that the player can press to interect with the game */
UENUM(BlueprintType)
enum class EButton : uint8
{
	Menu UMETA(DisplayName = "Menu"),
	NavigateLeft UMETA(DisplayName = "Navigate Left"),
	NavigateRight UMETA(DisplayName = "Navigate Right"),
	Select UMETA(DisplayName = "Select")
};

/* ETurn represents different dealer events (such as on deck dealt, on card placed) */
UENUM(BlueprintType)
enum class ETurn : uint8
{
	Undecided UMETA(DisplayName = "Undecided"),
	Player UMETA(DisplayName = "Player"),
	Opponent UMETA(DisplayName = "Opponent")
};