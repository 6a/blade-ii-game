#pragma once

#include "CoreMinimal.h"

#include "B2Game/CardSlot.h"

#include "HandSlot.generated.h"

UCLASS()
class BLADEIIGAME_API UHandSlot : public UCardSlot
{
	GENERATED_BODY()
		
public:
	/* Add the specified card */
	virtual void Add(ACard* Card) override;

	/**
	* Remove the card at index N
	* @param N - The index of the card to remove
	* @returns A pointer to the removed card
	*/
	virtual ACard* RemoveByIndex(uint32 N) override;

	/**
	 * Remove the card with the specified ID
	 * @param ID - The ID of the card to remove
	 * @returns A pointer to the removed card
	 */
	virtual ACard* RemoveByID(FString ID) override;

	/**
	 * Return the position + rotation that a card with the index "Index" should have.
	 * @param Index - The index for which the transform will be returned
	 * @returns FB2Transform for index "Index"
	 */
	const virtual FB2Transform GetTransformForIndex(uint32 Index) const override;

protected:

	/* The positions for each possible card location, when there is an odd number of cards in the slot */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Configuration)
	TArray<FB2Transform> CardTransformsOffset;

};
