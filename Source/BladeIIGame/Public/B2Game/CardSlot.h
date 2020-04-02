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
	UCardSlot();

	/* Returns the current stack size (i.e. the number of cards on this anchor) */
	UINT Size() const;

	/* Set the card offset for each placed card (offset from the previous) */
	void SetCardOffset(B2Transform Offset);

	/* Add the specified card */
	virtual void Add(const ACard* Card);

	/**
	 * Remove the card at index N 
	 * @param N - The index of the card to remove
	 * @returns A pointer to the removed card
	 */
	const virtual ACard* RemoveByIndex(UINT N);

	/**
	 * Remove the card with the specified ID
	 * @param ID - The ID of the card to remove
	 * @returns A pointer to the removed card
	 */
	const virtual ACard* RemoveByID(FString ID);

protected:
	/* A container for the current stack of cards on this anchor */
	TArray<const ACard*> Cards;

	/* Transform from which all positional calculations will be based on */
	B2Transform BaseTransform;

	/* The offset for each card from the previous */
	B2Transform CardOffset;

	/**
	 * Return the transform that a card with the index "Index" should have.
	 * @param Index - The index for which the transform will be returned
	 * @returns A transform for index "Index"
	 * @warning Base class has no implementation - override and do not call base class
	 */
	const virtual FTransform GetTransformForIndex(UINT Index) const;

private:
	TArray<const B2Transform> CardPositions;
};
