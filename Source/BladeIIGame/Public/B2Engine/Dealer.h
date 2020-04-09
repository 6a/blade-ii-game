#pragma once

#include "CoreMinimal.h"

#include "B2Game/Arena.h"
#include "B2Enum/DealerEventEnum.h"

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
	 * @param Arc - The extra offset to add to the transition
	 * @param bUseWaitGroup - Whether the move should use a waitgroup (makes the animation exclusive so subsequent onces will have to wait)
	 */
	void Move(UCardSlot* SourceSlot, uint32 SourceIndex, UCardSlot* TargetSlot, const FVector& Arc, bool bUseWaitGroup = true);

	/**
	 * Move the players current card from the hand up to the pre-effect offset
	 * @param Card - The card
	 */
	void PlayerEffectCard(ACard* Card);

	/**
	 * Move the opponents current card from the hand up to the pre-effect offset
	 * @param Card - The card
	 */
	void OpponentEffectCard(ACard* Card);

	/* Clears the field of any cards */
	void ClearField();

	/* Tick the dealer so that it can perform tasks such as calling-back after transitions are finished etc. */
	void Tick(float DeltaSeconds);
private:
	
	/* Whether or not the cards have already been dealt (to avoid dealing twice) */
	bool bCardsDealt;

	/* Waitgroups for firing various events */
	B2WaitGroup WaitGroupDealFinished;
	B2WaitGroup WaitGroupCardMoveFinished;
	B2WaitGroup WaitGroupClearFinished;
	B2WaitGroup WaitGroupEffectReady;

	/* The type of event to fire for the next effect activation event */
	EDealerEvent NextEffectEvent;

	/**
	 * Helper function for effect transition
	 * @param Card - The bolt card
	 * @param Card - The bolt card
	 */
	void EffectCard(ACard* Card, FVector Offset);
};

