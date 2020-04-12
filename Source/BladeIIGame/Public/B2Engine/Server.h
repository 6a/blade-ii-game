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
	virtual const B2ServerUpdate GetNextUpdate();

	/**
	 * Send an update to the server
	 * @param Update - the type of update
	 * @param Metadata - optional metadata, such as the blast target for the blast effect
	 */
	virtual void SendUpdate(EServerUpdate Update, const FString& MetaData = FString()) const;

private:
	TArray<B2ServerUpdate> InboundQueue;
	TArray<B2ServerUpdate> OutBoundQueue;
};

