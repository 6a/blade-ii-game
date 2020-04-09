#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "Card.h"
#include "Transform.h"
#include "B2Misc/Enum/CardSlotEnum.h"

#include "CardSlot.generated.h"

UCLASS(BlueprintType)
class BLADEIIGAME_API UCardSlot : public USceneComponent
{
	GENERATED_BODY()
	
public:	
	UCardSlot();

	/* Returns the current stack size (i.e. the number of cards on this anchor) */
	UINT Count() const;

	/* Returns an array of the card ID's, in ascending order */
	TArray<FString> GetSortedIDsAscending();

	/* Returns an array of the card ID's, in descending order */
	TArray<FString> GetSortedIDsDescending();

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
	 * Get the index of the card with the specified ID
	 * @param ID - The ID of the card for which the index will be returned
	 * @returns the index as an int - returns -1 if the card with the specified ID was not found
	 */
	virtual int GetIndexByID(FString ID);

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
	 * Get a reference to the last card in the slot. For the field, its the lastest one placed, for hand its the furthest from the deck, for the deck its the top one
	 * @returns A pointer to the card
	 */
	virtual ACard* GetLast();

	/**
	 * Return the position + rotation that a card with the index "Index" should have.
	 * @param Index - The index for which the transform will be returned
	 * @returns FB2Transform for index "Index"
	 */
	const virtual FB2Transform GetTransformForIndex(UINT Index) const;

	/* Updates the internal container so that the cards are in the right order */
	virtual void UpdateCardOrder();

	/* Set this instance's slot type */
	void SetType(ECardSlot SlotType);

	/* Get this instance's slot type */
	ECardSlot GetType() const;

protected:
	/* The positions for each possible card location */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Configuration)
	TArray<FB2Transform> CardTransforms;

	/* A container for the current stack of cards on this anchor */
	TArray<ACard*> Cards;

	/* The slot type for this instance. Should be set after creation */
	ECardSlot Type;

	/* Returns the transform of the first slot */
	const FB2Transform GetBaseTransform()const;
};
