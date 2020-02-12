#pragma once

#include "CoreMinimal.h"
#include "Card.h"

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

	ACard* Make(ECard Card, const FVector& TargetPosition);

private:

};

