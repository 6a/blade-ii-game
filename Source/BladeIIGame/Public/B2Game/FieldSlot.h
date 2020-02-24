#pragma once

#include "CoreMinimal.h"

#include "B2Game/CardSlot.h"

#include "FieldSlot.generated.h"

UCLASS()
class BLADEIIGAME_API UFieldSlot : public UCardSlot
{
	GENERATED_BODY()
	
public:
	UFieldSlot();

	/* Add the specified card */
	virtual void Add(const ACard* Card) override;

	/**
	* Remove the card at index N
	* @param N - The index of the card to remove
	* @returns A pointer to the removed card
	*/
	const virtual ACard* RemoveByIndex(UINT N) override;

	/**
	 * Remove the card with the specified ID
	 * @param ID - The ID of the card to remove
	 * @returns A pointer to the removed card
	 */
	const virtual ACard* RemoveByID(FString ID) override;

	/**
	 * Return the transform that a card with the index "Index" should have.
	 * @param Index - The index for which the transform will be returned
	 * @returns A transform for index "Index"
	 */
	const virtual FTransform GetTransformForIndex(UINT Index) override;
};
