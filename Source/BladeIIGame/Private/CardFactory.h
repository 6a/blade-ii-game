#pragma once

#include "CoreMinimal.h"
#include "Card.h"
#include "Utility.h"

struct B2CardFactoryConfig
{
public:
	B2CardFactoryConfig() {}
	~B2CardFactoryConfig() {}
};

class B2CardFactory
{
public:
	B2CardFactory(B2CardFactoryConfig& Config);
	~B2CardFactory();

	/**
	 * Spawn and return a pointer to a card actor.
	 * @param Card - The type of card to create.
	 * @param TargetPosition - The world position at which the spawned card will be placed.
	 * @return a pointer to the card actor that was spawned
	 */
	ACard* Make(ECard Card, const FVector& TargetPosition);

private:

};

