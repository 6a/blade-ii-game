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

	/* Min, max, and offset values for converting server messages to and from cards */
	const uint32 SERVER_MESSAGE_CARD_MIN = 0;
	const uint32 SERVER_MESSAGE_CARD_MAX = 10;
	const uint32 SERVER_MESSAGE_CARD_UPDATE_MIN = 1;
	const uint32 SERVER_MESSAGE_CARD_UPDATE_MAX = 11;
	const uint32 SERVER_MESSAGE_CARD_OFFSET = 1;

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

	/* Flag for whether or not we are waiting for the opponents move (drawing from the hand after a tie on the field and no cards in the deck) */
	bool bIsWaitingForOpponentsDrawFromHand;

	/* Helper function that sets up the internal state of the game after the cards have first been generated */
	void ConfigureInitialState();

	/* Updates the internal state of the game based on the received update */
	void UpdateState(const FB2ServerUpdate& Update);

	/* Removes and returns the last item in the specified array */
	ECard RemoveLast(TArray<ECard>& FromArray);

	/**
	 * Removes and returns the card (as an out ref) from the opponent hand, that the AI has decided to play
	 * @return true if a card was found
	 */
	bool GetNextMove(ECard& OutCard);

	/**
	 * Executes the next move based on the specified card
	 * @param ChosenCard - The card for which the move will be executed
	 * @return The update to send to the player
	 */
	FB2ServerUpdate ExecuteNextMove(ECard ChosenCard);

	/* Helper function that executes the opponents entire turn. Returns false if we couldnt find a valid move */
	bool ExecuteTurn();

	/* Handle a tied board state. Returns true if the tie was successfully solved */
	bool HandleTie();

	/* Bolts (flips) the last card in the specified field */
	void Bolt(TArray<ECard>& Field);

	/* Unbolts (heals) the last card in the specified field */
	void UnBolt(TArray<ECard>& Field);

	/* Returns true if the specified card is bolted (inactive) */
	bool IsBolted(ECard Card) const;

	/* Returns the value of the specified if it wasnt bolted */
	uint32 GetBoltedCardRealValue(ECard Card) const;

	/* Calculates the score for the specified field */
	uint32 CalculateScore(TArray<ECard>& Field);

	/* Returns the corresponding server update enum for the specified card */
	EServerUpdate CardToServerUpdate(ECard Card) const;

	/* Returns the corresponding card for the specified server update */
	ECard ServerUpdateToCard(EServerUpdate Update) const;

	/* Updates both players scores based on the current board state */
	void UpdateScores();

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