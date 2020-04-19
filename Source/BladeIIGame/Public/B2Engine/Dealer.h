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

	/* Deal onto the board immediately with minimal delay - for testing */
	void FastDeal();

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
	 * Move the specified card from the hand up to the pre-effect offset
	 * @param Card - The card
	 */
	void PlayerEffectCard(ACard* Card);

	/**
	 * Move the specified current card from the hand up to the pre-effect offset
	 * @param Card - The card
	 */
	void OpponentEffectCard(ACard* Card);

	/**
	 * Flip the specified card - Should only be called for cards on the field
	 * @param Card - The card
	 * @param bNewActive - Where the card will be active or not after the flip
	 */
	void FlipFieldCard(ACard* Card, bool bNewActive, float Delay = 0.f);

	/**
	 * Performs the mirror flip transition immediately
	 * @note This also updates the card positions internally, and immediately, so the score can be recalculated as soon as its called
	 */
	void Mirror();

	/**
	 * Moves the specified players hand to the pre blast select position, and shuffles if its the players hand
	 * @param Target - The player whos hand will be moved
	 */
	void PreBlastSelect(EPlayer Target);

	/**
	 * Moves the specified players hand up to the blast selection position
	 * @param Target - The player whos hand will be moved
	 */
	void BlastSelect(EPlayer Target);

	/**
	 * Returns the specified players hand to the original position, after shuffling
	 * @param Target - The player whos hand will be moved
	 */
	void BlastCleanup(EPlayer Target);

	/**
	 * Performs the first part of the force animation (flying upwards)
	 * @param Card - The card to which the animation will be performed
	 */
	void ForceOut(ACard* Card);

	/**
	 * Performs the second part of the force animation (flying downwards)
	 * @param Card - The card to which the animation will be performed
	 */
	void ForceIn(ACard* Card);

	/**
	 * Moves all the cards in the specified player's hand to their correct location - use after removing a card from the hand
	 * @param Target - The player whos hand will updated
	 */
	void UpdateHandPositions(EPlayer Target);

	/**
	 * Clears the specified card off of the field
	 * @param Card - The card to clear
	 */
	void ClearSingleFromField(ACard* Card) const;

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
	B2WaitGroup WaitGroupBlastFinished;
	B2WaitGroup WaitGroupHandPositionUpdateFinished;

	/**
	 * Helper function for effect card transition
	 * @param Card - The card
	 * @param Offset - The offset
	 */
	void EffectCard(ACard* Card, FVector Offset);

	/**
	 * Helper function that returns the forward direction of the specified card
	 * @param Card - The card
	 */
	FVector GetDirectionNormalized(const ACard* Card) const;

	/**
	 * Helper function that returns true if the specified card is on the players side of the field
	 * @param Card - The card
	 * @warning This is pretty hacky - might need to revisit this but it should be fine unless the arena moves
	 */
	bool CardIsFromPlayerField(const ACard* Card) const;
};

