#pragma once

#include "B2Engine/Server.h"
#include "B2Engine/Cards.h"

class B2AIServer : public B2Server
{
	/**
	 * Get the next update from the server.
	 * @return The update
	 */
	virtual const FB2ServerUpdate GetNextUpdate() override;

private:
	/* TODO testing only - remove later  */
	bool bCardsSent = false;

	/* Testing card setups */
	FB2Cards BoltTest() const;
	FB2Cards BlastTest() const;
	FB2Cards ForceTest() const;
	FB2Cards RodTest() const;
	FB2Cards MirrorTest() const;
	FB2Cards StandardCardsOnlyTest() const;
	FB2Cards CardOverlapTest() const;
	FB2Cards DupeTest() const;
};