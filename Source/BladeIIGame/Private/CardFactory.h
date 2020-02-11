#pragma once

#include "CoreMinimal.h"
#include "Card.h"

struct CardFactoryConfig
{
public:
	CardFactoryConfig() {}
	~CardFactoryConfig() {}
};

class QCardFactory
{
public:
	QCardFactory() {}
	QCardFactory(CardFactoryConfig& config);
	~QCardFactory();

	ACard* Make(ECard card, const FVector& targetPosition);

private:
	bool bIsInitialised;
};

