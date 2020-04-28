#pragma once

#include "CoreMinimal.h"

#include "B2Game/Card.h"
#include "B2Game/Arena.h"
#include "B2Enum/CardEnum.h"

#include "CardFactory.generated.h"

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
UCLASS()
class BLADEIIGAME_API UB2CardFactory : public UObject
{
	GENERATED_BODY()

public:

	~UB2CardFactory();
	UB2CardFactory(const FObjectInitializer& ObjectInitialiser);

	/**
	 * Initialize this card factory with the specified configuration.
	 * @param CardFactoryConfig - The configuration for this card factory
	 * @param Arena - A pointer to the arena
	 */
	void Initialise(const B2CardFactoryConfig& CardFactoryConfig, AArena* InArena);

	/**
	 * Spawn and return a pointer to a card actor.
	 * @param Card - The type of card to create.
	 * @param StartingPosition - The world position at which the spawned card will be placed.
	 * @param StartingRotation - The rotation that the card will start with.
	 * @return a pointer to the card actor that was spawned
	 */
	ACard* Make(const ECard& Card, const FVector& StartingPosition, const FRotator& StartingRotation);

private:
	/* Card textures */
	UPROPERTY()
	TArray<UTexture*> CardFrontTextures;

	UPROPERTY()
	UTexture* CardBackTexture;

	UPROPERTY()
	UTexture* CardFrontMRSTexture;
	
	UPROPERTY()
	UTexture* CardBackMRSTexture;

	/* Card actor */
	TSubclassOf<ACard> CardActorClass;

	/* Pointer to current world object */
	UPROPERTY()
	UWorld* World;

	/* Pointer to the current arena actor */
	UPROPERTY()
	AArena* Arena;

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

