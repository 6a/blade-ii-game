#pragma once

#include "CoreMinimal.h"

#include "B2Game/Card.h"
#include "B2Misc/Enum.h"

/* A wrapper for a card factory's initialization parameters */
struct B2CardFactoryConfig
{
	TArray<FString> CardFrontPaths;
	FString CardBackPath;
	FString CardFrontMRSPath;
	FString CardBackMRSPath;
	FString CardActorPath;
	UWorld* World;
};

/* Spawns and places cards */
class B2CardFactory
{
public:

	/**
	 * Initialize this card factory with the specified configuration.
	 * @param CardFactoryConfig - The configuration for this card factory
	 */
	B2CardFactory(const B2CardFactoryConfig& CardFactoryConfig);

	~B2CardFactory();

	/**
	 * Spawn and return a pointer to a card actor.
	 * @param Card - The type of card to create.
	 * @param TargetPosition - The world position at which the spawned card will be placed.
	 * @return a pointer to the card actor that was spawned
	 */
	ACard* Make(const ECard& Card, const FVector& TargetPosition);

private:
	/* Card textures */
	TArray<UTexture*> CardFrontTextures;
	UTexture* CardBackTexture;
	UTexture* CardFrontMRSTexture;
	UTexture* CardBackMRSTexture;

	/* Card actor */
	UClass* CardActorClass;

	/* Pointer to current world object */
	UWorld* World;

	/**
	 * Load all the required classes, textures etc.
	 * @param CardFactoryConfig - The configuration for this card factory
	 */
	void LoadConfig(const B2CardFactoryConfig& CardFactoryConfig);

	/**
	 * Get the texture for the front of the specified card 
	 * @param Card - The type of card for which the texture should be returned
	 * @return a pointer to the texture 
	 */
	UTexture* GetTexture(const ECard& Card) const;
};

