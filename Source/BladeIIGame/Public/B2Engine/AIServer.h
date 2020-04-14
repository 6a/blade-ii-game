#pragma once

#include "B2Engine/Server.h"
#include "B2Engine/Cards.h"
#include "B2Enum/PlayerEnum.h"

class B2AIServer : public B2Server
{
	/**
	 * Get the next update from the server.
	 * @return The update
	 */
	virtual const FB2ServerUpdate GetNextUpdate() override;

	/* Tick the server */
	virtual void Tick(float DeltaSeconds) override;

private:
	/* The maximum value (in terms of enum value) that an effect card can have */
	const uint32 MAX_ECARD_VALUE = 10;

	/* The maximum size of the hand slot */
	const uint32 HAND_SLOT_MAX = 10;

	/* The maximum (and initial) size of the deck slot */
	const uint32 DECK_SLOT_MAX = 15;

	/* The value to remove from a bolted card (non effect) to determine its non flipped type */
	const uint32 BOLTED_CARD_OFFSET = 11;

	/* Prevents the initial deck from being generated and sent more than once */
	bool bCardsSent = false;

	/* The cards in the game */
	FB2Cards Cards;

	/* Player and Opponent (AI) Score */
	uint32 PlayerScore;
	uint32 OpponentScore;

	/* Current turn */
	EPlayer Turn;

	/* The winner of the match (if one has been found, else undecided) */
	EPlayer Winner;

	/* Helper function that sets up the internal state of the game after the cards have first been generated */
	void ConfigureInitialState();

	/* Updates the internal state of the game based on the received update */
	void UpdateState(const FB2ServerUpdate& Update);

	/* Removes and returns the last item in the specified array */
	ECard RemoveLast(TArray<ECard>& FromArray);

	/**
	 * Removes and returns the card (as an out ref) from the hand that the AI has decided to play
	 * @return true if a card was found
	 */
	bool RemoveCardToPlayFromHand(ECard& OutCard);

	/* Returns true if the specified card is bolted (inactive) */
	bool IsBolted(ECard Card) const;

	/* Returns the value of the specified if it wasnt bolted */
	uint32 GetBoltedCardRealValue(ECard Card) const;

	/* Testing card setups */
	FB2Cards BoltTest() const;
	FB2Cards BlastTest() const;
	FB2Cards ForceTest() const;
	FB2Cards RodTest() const;
	FB2Cards MirrorTest() const;
	FB2Cards StandardCardsOnlyTest() const;
	FB2Cards CardOverlapTest() const;
	FB2Cards DupeTest() const;
	FB2Cards PlayerFirstTest() const;
};