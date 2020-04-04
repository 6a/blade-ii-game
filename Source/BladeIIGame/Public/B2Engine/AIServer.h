#pragma once

#include "Server.h"

class B2AIServer : public B2Server
{
	/**
	 * Get the next update from the server.
	 * @return The update
	 */
	virtual const B2ServerUpdate GetNextUpdate() override;

private:
	/* TODO testing only - remove later  */
	bool bCardsSent = false;
};