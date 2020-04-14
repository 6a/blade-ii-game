#pragma once

#include "CoreMinimal.h"

#include "ServerUpdate.h"

class B2Server
{
public:
	virtual ~B2Server();

	/**
	 * Get the next update from the server. Call this on derived classes to get a default (no update) return value
	 * @return The update
	 */
	virtual const FB2ServerUpdate GetNextUpdate();

	/**
	 * Send an update to the server
	 * @param Update - the type of update
	 * @param Metadata - optional metadata, such as the blast target for the blast effect
	 */
	virtual void SendUpdate(EServerUpdate Update, const FString& MetaData = FString()) const;

private:
	TArray<FB2ServerUpdate> InboundQueue;
	TArray<FB2ServerUpdate> OutBoundQueue;
};

