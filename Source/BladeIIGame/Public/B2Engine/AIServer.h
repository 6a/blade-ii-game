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

	/* Testing card setups */
	FB2Cards BoltTest() const;
	FB2Cards BlastTest() const;
	FB2Cards ForceTest() const;
	FB2Cards RodTest() const;
	FB2Cards MirrorTest() const;
};