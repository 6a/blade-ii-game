#pragma once

#include "CoreMinimal.h"

#include "B2Game/Arena.h"
#include "B2Misc/Enum.h"

#include "Dealer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDealerEventDelegate, EDealerEvent, Update);

UCLASS()
class UB2Dealer : public UObject
{
	GENERATED_BODY()

public:	
	/* Callback for when a transition has finished */
	FDealerEventDelegate OnDealerEvent;
	
	/* Pointer to the arena */
	AArena* Arena;

	UB2Dealer();
	
	/* Deal the cards out onto the arena. Can only be called once */
	void Deal();

	/**
	 * Move a card from the source slot + index to the target slot (at the next available position).
	 * @param SourceSlot - The CardSlot from which to get the card
	 * @param SourceIndex - The index in the specified card slot from which to get the card
	 * @param TargetSlot - The CardSlot to where the card will go (at the next available position)
	 * @param bUseWaitGroup - Whether the move should use a waitgroup (makes the animation exclusive so subsequent onces will have to wait)
	 * @returns FB2Transform for index "Index"
	 */
	void MoveFromDeck(UCardSlot* SourceSlot, uint32 SourceIndex, UCardSlot* TargetSlot, bool bUseWaitGroup = true);

	/* Tick the dealer so that it can perform tasks such as calling-back after transitions are finished etc. */
	void Tick(float DeltaSeconds);
private:
	
	/* Whether or not the cards have already been dealt (to avoid dealing twice) */
	bool bCardsDealt;

	/* Waitgroups for firing various events */
	B2WaitGroup WaitGroupDealFinished;
	B2WaitGroup WaitGroupCardMoveFinished;
};

