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
	QCardFactory(CardFactoryConfig& config);
	~QCardFactory();

	ACard* Make(ECard card, const FVector& targetPosition);

private:
	FString CardEnumToString(ECard card);
};

