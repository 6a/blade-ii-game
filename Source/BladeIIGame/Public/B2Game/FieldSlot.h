#pragma once

#include "CoreMinimal.h"

#include "B2Game/CardSlot.h"

#include "FieldSlot.generated.h"

UCLASS()
class BLADEIIGAME_API UFieldSlot : public UCardSlot
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
};
