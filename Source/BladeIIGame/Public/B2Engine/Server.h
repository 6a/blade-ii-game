#pragma once

#include "CoreMinimal.h"

#include "ServerUpdate.h"
#include "PlayerUpdate.h"

class B2Server
{
public:
	virtual ~B2Server();

	/**
	 * Get the next update from the server. Call this on derived classes to get a default (no update) return value
	 * @return The update
	 */
	virtual const B2ServerUpdate GetNextUpdate();

private:
	TArray<B2ServerUpdate> InboundQueue;
	TArray<B2PlayerUpdate> OutBoundQueue;
};

