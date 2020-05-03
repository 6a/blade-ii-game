#pragma once

#include "CoreMinimal.h"

#include "Containers/Queue.h"

#include "B2Engine/ServerUpdate.h"

#include "Server.generated.h"

UCLASS()
class UB2Server : public UObject
{
	GENERATED_BODY()

public:
	virtual ~UB2Server();

	/**
	 * Get the next update from the server. Keep calling this until the payload is none.
	 * @return The update
	 */
	virtual const FB2ServerUpdate GetNextUpdate();

	/**
	 * Send an update to the server
	 * @param Update - the type of update
	 * @param Metadata - optional metadata, such as the blast target for the blast effect
	 */
	virtual void SendUpdate(EServerUpdate Update, const FString& MetaData = FString());

	/* Tick the server */
	virtual void Tick(float DeltaSeconds);

	/**
	 * Attempt to connect to the game server again - only valid if the client has not yet connected
	 * @return false if the conditions are invalid (such as if the client has disconnected after being connected already)
	 */
	virtual bool Connect();

	/* Kill the server, so that it stops processing and sending/receiving messages */
	virtual void Kill();

protected:

	/* Message queues */
	TQueue<FB2ServerUpdate> OutBoundQueue;
	TQueue<FB2ServerUpdate> InBoundQueue;

	/* Flag to determine whether or not to actually do stuff */
	bool bIgnoreAllEvents;
};

