#include "B2Game/Card.h"
#include "B2Predicate/FilterFirstOfType.h"
#include "B2Predicate/IsNotEffectCard.h"

#include "B2Engine/AIServer.h"

const FB2ServerUpdate B2AIServer::GetNextUpdate()
{
	FB2ServerUpdate Payload = B2Server::GetNextUpdate();

	if (!bCardsSent)
	{
		FB2Cards OutCards = PlayerFirstTest();
		Cards = OutCards;

		// Set up the initial internal state based on the cards
		ConfigureInitialState();

		Payload.Update = EServerUpdate::InstructionCards;
		Payload.Metadata = OutCards.GetSerialised(0);

		bCardsSent = true;
	}

	return Payload;
}

void B2AIServer::Tick(float DeltaSeconds)
{
	// Handle messages "to" the server (rather, to us)
	FB2ServerUpdate OutUpdate;
	while (OutBoundQueue.Dequeue(OutUpdate))
	{
		// Update the internal card state
		UpdateState(OutUpdate);

		// Calculate a move to make

		// Send the move back to the player
	}
}

void B2AIServer::ConfigureInitialState()
{
	// Note - The AI player is always player, or in this case, the "opponent"

	// Place the cards from the decks onto the hands
	for (int32 i = static_cast<int32>(DECK_SLOT_MAX) - 1; i >= static_cast<int32>(DECK_SLOT_MAX) - static_cast<int32>(HAND_SLOT_MAX); i--)
	{
		ECard PlayerCard = RemoveLast(Cards.PlayerDeck);
		Cards.PlayerHand.Add(PlayerCard);

		ECard OpponentCard = RemoveLast(Cards.OpponentDeck);
		Cards.OpponentHand.Add(OpponentCard);
	}

	// Set the initial scores
	PlayerScore = OpponentScore = 0;

	// Set initial turn
	Turn = EPlayer::Undecided;

	// Set current winner (undecided)
	Winner = EPlayer::Undecided;

	// Draw cards from both decks to the field until a player can start
	bool bNoValidMove = false;
		
	while (true)
	{
		// If there are cards in the decks, we draw from the deck - else from the hand
		// We only need to check one of the decks as at this stage and (always ?) they should be uniform in size
		if (Cards.PlayerDeck.Num() <= 0)
		{
			// Choose a card to play
			ECard ChosenCard;
			bool bValidMoveFound = RemoveCardToPlayFromHand(ChosenCard);

			if (bValidMoveFound)
			{
				// Add the card to the field
				Cards.OpponentField.Add(ChosenCard);

				// Update the score
				OpponentScore = ACard::TypeToValue(ChosenCard);
			}
			else
			{
				// If we ended up here, there was no valid move found - so we lost
				bNoValidMove = true;
			}

			// Set to undecided state
			Turn = EPlayer::Undecided;

			// Break as we will now wait for the players opening card
			break;
		}

		// Draw from deck to field
		ECard PlayerCard = RemoveLast(Cards.PlayerDeck);
		ECard OpponentCard = RemoveLast(Cards.OpponentDeck);

		uint32 PlayerCardValue = ACard::TypeToValue(PlayerCard);
		uint32 OpponentCardValue = ACard::TypeToValue(OpponentCard);

		// If the drawn cards are equal in point value, throw them in the discard and loop again
		if (PlayerCardValue == OpponentCardValue)
		{
			Cards.PlayerDiscard.Add(PlayerCard);
			Cards.OpponentDiscard.Add(OpponentCard);
			continue;
		}
		else // Otherwise, we can determine whos turn it is
		{
			Cards.PlayerField.Add(PlayerCard);
			Cards.OpponentField.Add(OpponentCard);

			if (PlayerCardValue > OpponentCardValue)
			{
				Turn = EPlayer::Player;
			}
			else
			{
				Turn = EPlayer::Opponent;
			}
			break;
		}
	}

	// If we couldnt find a valid move to play... We lost!
	// Dont need to do anthing other than set the internal winner, as the player should eventually work this out by themselves
	if (bNoValidMove)
	{
		Winner = EPlayer::Player;
	}
	else if (Turn == EPlayer::Opponent) // Otherwise, if its our turn, choose a card to play and play it - sending the play to the player afterwards
	{
		// Choose a card to play
		ECard ChosenCard;
		bool bValidMoveFound = RemoveCardToPlayFromHand(ChosenCard);

		// If a valid move was not found, this means that we lost somehow. This is probably impossible at this stage, unless our entire deck is 
		// full of effect cards or something, but this should be prevented by intelligently generating cards in the first place
		if (!bValidMoveFound)
		{
			Winner = EPlayer::Player;
		}
		else
		{
			
		}
	}
}

void B2AIServer::UpdateState(const FB2ServerUpdate& Update)
{

}

ECard B2AIServer::RemoveLast(TArray<ECard>& FromArray)
{
	ECard Card = ECard::InactiveForce;

	if (FromArray.Num() > 0)
	{
		Card = FromArray.Last();
		FromArray.RemoveAt(FromArray.Num() - 1);
	}
		
	return Card;
}

bool B2AIServer::RemoveCardToPlayFromHand(ECard& OutCard)
{
	// TODO make this smart - At the moment we just choose a random card that wont make use auto lose

	TArray<ECard> ValidCards;
	uint32 ScoreDifference = PlayerScore - OpponentScore;
	
	for (ECard Card : Cards.OpponentHand)
	{
		TArray<ECard> HandWithoutCurrentCard = Cards.OpponentHand.FilterByPredicate(B2Predicate_FilterFirstOfType(Card));

		// Check if removing this card would NOT us with only effect cards (auto lose)
		if (HandWithoutCurrentCard.ContainsByPredicate(B2Predicate_IsNotEffectCard()))
		{
			// Check if this card has a high enough value to overcome the current score difference
			if (ACard::TypeToValue(Card) >= ScoreDifference)
			{
				// If so, its valid to play this card
				ValidCards.Add(Card);
				continue;
			}

			// Check if this card can not overcome the score difference, but...
			if (ACard::TypeToValue(Card) < ScoreDifference)
			{
				// Its a rod card
				if (Card == ECard::ElliotsOrbalStaff)
				{
					// We have a card on the field that can be resurrected
					if (IsBolted(Cards.OpponentField.Last()))
					{
						// The card to resurrect has a high enough score to match or beat the other score
						if (GetBoltedCardRealValue(Cards.OpponentField.Last()) >= ScoreDifference)
						{
							// If so, its valid to play this card
							ValidCards.Add(Card);
							continue;
						}
					}
				}

				// This is a force card
				if (Card == ECard::Force)
				{
					// If we played this force card, the resulting score would be high enough to match or beat the other score
					if (OpponentScore * 2 >= ScoreDifference)
					{
						// If so, its valid to play this card
						ValidCards.Add(Card);
						continue;
					}
				}
			}
		}
	}

	// TODO do this intelligently - for example, preferring to NOT end up in a drawn state, prioritising lower value cards etc

	if (ValidCards.Num() == 0)
	{
		return false;
	}

	ECard CardToPlay = ValidCards[FMath::RandRange(0, ValidCards.Num() - 1)];
	Cards.PlayerHand.RemoveSingle(CardToPlay);

	OutCard = CardToPlay;

	return true;
}

bool B2AIServer::IsBolted(ECard Card) const
{
	return Card > ECard::Force;
}

uint32 B2AIServer::GetBoltedCardRealValue(ECard Card) const
{
	int32 OutValue = 0;

	if (Card > ECard::Force)
	{
		Card = static_cast<ECard>(static_cast<uint32>(Card) - BOLTED_CARD_OFFSET);
	}

	OutValue = ACard::TypeToValue(Card);

	return OutValue;
}

FB2Cards B2AIServer::BoltTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(ECard::Bolt);
		GeneratedCards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return GeneratedCards;
}

FB2Cards B2AIServer::BlastTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(ECard::Blast);
		GeneratedCards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return GeneratedCards;
}

FB2Cards B2AIServer::ForceTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		ECard Type = static_cast<bool>(FMath::RandRange(0, 1)) ? ECard::Force : ECard::GaiusSpear;

		GeneratedCards.PlayerDeck.Add(Type);
		GeneratedCards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return GeneratedCards;
}

FB2Cards B2AIServer::RodTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		ECard Type = static_cast<bool>(FMath::RandRange(0, 1)) ? ECard::ElliotsOrbalStaff : ECard::GaiusSpear;

		GeneratedCards.PlayerDeck.Add(Type);
		GeneratedCards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return GeneratedCards;
}

FB2Cards B2AIServer::MirrorTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		ECard Type = static_cast<bool>(FMath::RandRange(0, 1)) ? ECard::Mirror : static_cast<ECard>(FMath::RandRange(0, 6));

		GeneratedCards.PlayerDeck.Add(Type);
		GeneratedCards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return GeneratedCards;
}

FB2Cards B2AIServer::StandardCardsOnlyTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
		GeneratedCards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return GeneratedCards;
}

FB2Cards B2AIServer::CardOverlapTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(static_cast<ECard>(FMath::Clamp(i, 0, 9)));
		GeneratedCards.OpponentDeck.Add(ECard::Force);
	}

	return GeneratedCards;
}

FB2Cards B2AIServer::DupeTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(ECard::LaurasGreatsword);
		GeneratedCards.OpponentDeck.Add(ECard::LaurasGreatsword);
	}

	return GeneratedCards;
}

FB2Cards B2AIServer::PlayerFirstTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(static_cast<ECard>(FMath::RandRange(0, 4)));
		GeneratedCards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(5, 6)));
	}

	return GeneratedCards;
}
