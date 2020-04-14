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
	ProcessingBoardState UMETA(DisplayName = "Processing the board state"),
	SelectingFromPlayerHand UMETA(DisplayName = "Selecting a card from the local players hand"),
	SelectingFromOpponentHand UMETA(DisplayName = "Selecting a card from the opponents hand"),
	PlayerRod UMETA(DisplayName = "Player Rod"),
	OpponentRod UMETA(DisplayName = "Opponent Rod"),
	PlayerBolt UMETA(DisplayName = "Player Bolt"),
	OpponentBolt UMETA(DisplayName = "Opponent Bolt"),
	PlayerMirror UMETA(DisplayName = "Player Mirror"),
	OpponentMirror UMETA(DisplayName = "Opponent Mirror"),
	PlayerBlast UMETA(DisplayName = "Player Blast"),
	OpponentBlast UMETA(DisplayName = "Opponent Blast"),
	PlayerBlastTarget UMETA(DisplayName = "Player Blast Target"),
	OpponentBlastTarget UMETA(DisplayName = "Opponent Blast Target"),
	PlayerForce UMETA(DisplayName = "Player Force"),
	OpponentForce UMETA(DisplayName = "Opponent Force"),
};