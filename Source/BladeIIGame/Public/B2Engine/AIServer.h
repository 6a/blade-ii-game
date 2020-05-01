#pragma once

#include "B2Engine/Server.h"
#include "B2Engine/Cards.h"
#include "B2Enum/PlayerEnum.h"

#include "AIServer.generated.h"

UCLASS()
class UB2AIServer : public UB2Server
{
	GENERATED_BODY()

public:

	/**
	 * Get the next update from the server.
	 * @return The update
	 */
	virtual const FB2ServerUpdate GetNextUpdate() override;

	/* Tick the server */
	virtual void Tick(float DeltaSeconds) override;

	/**
	 * Attempt to connect to the game server again - only valid if the client has not yet connected
	 * @note For the AI server this will always return false
	 * @return false if the conditions are invalid (such as if the client has disconnected after being connected already)
	 */
	virtual bool Connect() override { return false; }

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

	/* Maximum number of draws a game can have when first drawing from the deck */
	const uint32 MAX_DRAW_ON_START = 3;

	/* The size of the deck after the initial draw to hand is performed */
	const uint32 POST_INIT_DECK_SIZE = 5;

	/* Prevents the initial deck from being generated and sent more than once */
	bool bCardsSent = false;

	/* The cards in the game */
	FB2Cards Cards;

	/* Player and AI Score */
	uint32 PlayerScore;
	uint32 AIScore;

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
	 * Removes and returns the card (as an out ref) from the AI's hand, that the AI has decided to play
	 * @return true if a card was found
	 */
	bool GetNextMove(ECard& OutCard);

	/**
	 * Executes the next move based on the specified card
	 * @param ChosenCard - The card for which the move will be executed
	 * @return The update to send to the player
	 */
	FB2ServerUpdate ExecuteMove(ECard ChosenCard);

	/* Helper function that executes the AI's entire turn. Returns false if a valid move was not found */
	bool ExecuteTurn();

	/* Update the current turn based on the state of the board - call only after the AI has played a card */
	void UpdateTurn();

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
	uint32 CalculateScore(TArray<ECard>& Field) const;

	/* Returns the corresponding server update enum for the specified card */
	EServerUpdate CardToServerUpdate(ECard Card) const;

	/* Returns the corresponding card for the specified server update */
	ECard ServerUpdateToCard(EServerUpdate Update) const;

	/* Updates both players scores based on the current board state */
	void UpdateScores();

	/* Do whatever is required to get the gamestate back to being either the player turn, or an undecided state */
	void ResolveAITurn();

	/* Set the AI to the loss state */
	void SetAILoss(const FString& Reason);

	/* Generate the set of cards to be used for this AI game */
	FB2Cards GenerateCards() const;

	/**
	 * Validates the passed in set of cards, to ensure that the game will not immediately end due to a bad state etc.
	  *@param InCards - The cards to check
	 * @return true if the cards are valid
	 */
	bool ValidateCards(const FB2Cards& InCards) const;

	/**
	 * Returns true if the specified card set contains a card that can beat the target card
	  *@param CardSet - The cards set to check - typically a players hand
	  *@param CardToBeatOrMatch - The card on the other side of the field that needs to be beaten or matched
	  *@param CurrentScore - The current score that the player (the one for which we are checking for a valid first move) has
	 * @return true if there is a valid first move that can be made
	 */
	bool ValidFirstMoveAvailable(const TArray<ECard>& CardSet, ECard CardToBeatOrMatch, uint32 CurrentScore) const;

	/* Testing card setups */
	FB2Cards BoltTest() const;
	FB2Cards OpponentBoltTest() const;

	FB2Cards BlastTest() const;
	FB2Cards OpponentBlastTest() const;

	FB2Cards ForceTest() const;
	FB2Cards OpponentForceTest() const;

	FB2Cards RodTest() const;
	FB2Cards OpponentRodTest() const;

	FB2Cards MirrorTest() const;
	FB2Cards OpponentMirrorTest() const;

	FB2Cards StandardCardsOnlyTest() const;
	FB2Cards CardOverlapTest() const;
	FB2Cards DupeTest() const;
	FB2Cards PlayerFirstTest() const;
	FB2Cards AIFirstTest() const;
	FB2Cards AllRandomTest() const;
};