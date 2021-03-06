#pragma once

#include "CoreMinimal.h"

#include "Containers/Queue.h"

#include "B2Engine/Server.h"
#include "B2Engine/Cards.h"
#include "B2Engine/ServerUpdate.h"

#include "Opponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerInstructionReceivedDelegate, const FB2ServerUpdate&, Instruction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCardsReceivedDelegate, const FB2Cards&, Cards);

UCLASS()
class UB2Opponent : public UObject
{
	GENERATED_BODY()

public:
	/* Callback for receiving cards from the server */
	FCardsReceivedDelegate OnCardsReceived;

	/* Callback for when a non-card update is received from the server */
	FServerInstructionReceivedDelegate OnInstructionReceived;

	/* Queue for incoming move updates from the opponent */
	TQueue<FB2ServerUpdate> MoveUpdateQueue;

	/**
	 * Tick this opponent instance.
	 * @param DeltaSeconds - DeltaSeconds for the previous frame.
	 */
	virtual void Tick(float DeltaSeconds);

	/**
	 * Send an update to the server
	 * @param Update - the type of update
	 * @param Metadata - optional metadata, such as the blast target for the blast effect
	 */
	void SendUpdate(EServerUpdate Update, const FString& MetaData = FString()) const;

	/**
	 * Attempt to connect to the game server again - only valid if the client has not yet connected
	 * @return false if the conditions are invalid (such as if the client has disconnected after being connected already)
	 */
	bool ReConnect();

	/* Deactivate the back end - no more messages will be processed, sent or received */
	void Deactivate();

protected:
	/* The back end for this opponent. Could be an AI opponent, or a network opponent */
	UPROPERTY()
	UB2Server* BackEnd;
};

