#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "Card.h"
#include "Transform.h"

#include "CardSlot.generated.h"

UCLASS(BlueprintType)
class BLADEIIGAME_API UCardSlot : public USceneComponent
{
	GENERATED_BODY()
	
public:	
	/* The positions for each possible card location */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Configuration)
	TArray<FB2Transform> CardTransforms;

	UCardSlot();

	/* Returns the current stack size (i.e. the number of cards on this anchor) */
	UINT Size() const;

	/* Add the specified card */
	virtual void Add(ACard* Card);

	/**
	 * Get the card at index N
	 * @param N - The index of the card to get
	 * @returns A pointer to the card
	 */
	virtual ACard* GetCardByIndex(UINT N);

	/**
	 * Get the card with the specified ID
	 * @param ID - The ID of the card to get
	 * @returns A pointer to the card
	 */
	virtual ACard* GetCardByID(FString ID);

	/**
	 * Remove the card at index N 
	 * @param N - The index of the card to remove
	 * @returns A pointer to the removed card
	 */
	virtual ACard* RemoveByIndex(UINT N);

	/**
	 * Remove the card with the specified ID
	 * @param ID - The ID of the card to remove
	 * @returns A pointer to the removed card
	 */
	virtual ACard* RemoveByID(FString ID);

	/**
	 * Return the position + rotation that a card with the index "Index" should have.
	 * @param Index - The index for which the transform will be returned
	 * @returns FB2Transform for index "Index"
	 */
	const virtual FB2Transform GetTransformForIndex(UINT Index) const;

protected:
	/* A container for the current stack of cards on this anchor */
	TArray<ACard*> Cards;

	/* Transform from which all positional calculations will be based on */
	FB2Transform BaseTransform;
};
