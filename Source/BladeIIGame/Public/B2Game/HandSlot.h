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
	 * Return the position + rotation that a card with the index "Index" should have.
	 * @param Index - The index for which the transform will be returned
	 * @returns FB2Transform for index "Index"
	 */
	const virtual FB2Transform GetTransformForIndex(uint32 Index) const override;

	/**
	 * Return the position + rotation for the center of the slot.
	 * @returns FB2Transform for the center slot
	 */
	const virtual FB2Transform GetCurrentCenterTransform() const override;

protected:

	/* The positions for each possible card location, when there is an odd number of cards in the slot */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Configuration)
	TArray<FB2Transform> CardTransformsOffset;

private:
	
	/* Flag to check if the cards have been dealt. If not, dont offset the card transforms */
	bool bCardsDealt;

};
