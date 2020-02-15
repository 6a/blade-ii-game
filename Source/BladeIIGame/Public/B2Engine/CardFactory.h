#pragma once

#include "CoreMinimal.h"

#include "B2Game/Card.h"
#include "B2Misc/Enum.h"

struct B2CardFactoryConfig
{
	TArray<FString> CardFrontPaths;
	FString CardBackPath;
	FString CardFrontMRSPath;
	FString CardBackMRSPath;

	FString CardActorPath;

	UWorld* World;
};

class B2CardFactory
{
public:
	B2CardFactory(const B2CardFactoryConfig& CardFactoryConfig);
	~B2CardFactory();

	/**
	 * Spawn and return a pointer to a card actor.
	 * @param Card - The type of card to create.
	 * @param TargetPosition - The world position at which the spawned card will be placed.
	 * @return a pointer to the card actor that was spawned
	 */
	ACard* Make(const ECard& Card, const FVector& TargetPosition);

	void LoadConfig(const B2CardFactoryConfig& CardFactoryConfig);

private:
	TArray<UTexture*> CardFrontTextures;
	UTexture* CardBackTexture;
	UTexture* CardFrontMRSTexture;
	UTexture* CardBackMRSTexture;

	UClass* CardActorClass;

	UWorld* World;



	UTexture* GetTexture(const ECard& Card) const;
};

