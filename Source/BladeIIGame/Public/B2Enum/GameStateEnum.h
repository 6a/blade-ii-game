#pragma once

#include "CoreMinimal.h"

/* EGameState represents different phases of the actual game, such as both players drawing onto the field, or waiting for the opponents move */
UENUM(BlueprintType)
enum class EGameState : uint8
{
	WaitingForInitialDeal UMETA(DisplayName = "Waiting for initial deal"),
	DrawToEmptyField UMETA(DisplayName = "Drawing from the deck to an empty field"),
	PlayerTurn UMETA(DisplayName = "The players turn"),
	WaitingForOpponentMove UMETA(DisplayName = "Waiting for the opponents move to be received"),
	SelectingFromPlayerHand UMETA(DisplayName = "Selecting a card from the local players hand"),
	SelectingFromOpponentHand UMETA(DisplayName = "Selecting a card from the opponents hand"),
	PlayerBolt UMETA(DisplayName = "Player Bolt"),
	OpponentBolt UMETA(DisplayName = "Opponent Bolt"),
	PlayerBlast UMETA(DisplayName = "Player Blast"),
	OpponentBlast UMETA(DisplayName = "Opponent Blast"),
	PlayerForce UMETA(DisplayName = "Player Force"),
	OpponentForce UMETA(DisplayName = "Opponent Force"),
};