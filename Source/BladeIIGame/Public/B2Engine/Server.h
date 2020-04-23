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

protected:
	TQueue<FB2ServerUpdate> OutBoundQueue;
	TQueue<FB2ServerUpdate> InBoundQueue;
};

