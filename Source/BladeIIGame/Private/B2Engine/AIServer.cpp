#include "B2Engine/AIServer.h"

#include "Misc/DefaultValueHelper.h"

#include "B2Game/Card.h"
#include "B2Predicate/FilterFirstOfType.h"
#include "B2Predicate/IsNotEffectCard.h"
#include "B2Utility/String.h"

const FB2ServerUpdate B2AIServer::GetNextUpdate()
{
	FB2ServerUpdate Payload = B2Server::GetNextUpdate();

	if (!bCardsSent)
	{
		// Generate the cards for this match
		Cards = PlayerFirstTest();

		// Make a copy of the cards to send to the player, so we can freely modify the one we just created and stored interanlly
		FB2Cards OutCards = Cards;

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
	if (Winner == EPlayer::Opponent)
	{
		return;
	}

	// Handle messages "to" the server (rather, to us)
	FB2ServerUpdate IncomingUpdate;
	while (InboundQueue.Dequeue(IncomingUpdate))
	{
		// Ignore "None" or message types for now
		if (IncomingUpdate.Update == EServerUpdate::None || IncomingUpdate.Update == EServerUpdate::InstructionMessage)
		{
			continue;
		}

		// Update the internal card state
		UpdateState(IncomingUpdate);

		// Now we iterate until we have successfully moved into either the Undecided state, or the players turn
		while (Turn != EPlayer::Player)
		{
			bool bSuccess = ExecuteTurn();
			if (!bSuccess)
			{
				// Game over?
				break;
			}

			if (PlayerScore == OpponentScore)
			{
				bool Success = HandleTie();

				// If we could not break the tie, its a game over - but the player should already be aware. 
				// Just set the state to a player win, as it doesnt matter anyway
				if (!Success)
				{
					Winner = EPlayer::Player;
					break;
				}
			}
		}
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
	bool bValidMoveFound = HandleTie();
		
	// If we couldnt find a valid move to play... We lost!
	// Dont need to do anthing other than set the internal winner, as the player should eventually work this out by themselves
	if (!bValidMoveFound)
	{
		Winner = EPlayer::Player;
	}
	else if (Turn == EPlayer::Opponent) // Otherwise, if its our turn, choose a card to play and play it - sending the play to the player afterwards
	{
		// Execute the opponents turn - no need to check for success as we exit after this call anyway
		ExecuteTurn();
	}

	// Otherwise, its the players turn, or its undecided - either way, we are not waiting for an update from the player
}

bool B2AIServer::ExecuteTurn()
{
	// Choose a card to play
	ECard ChosenCard;
	bool bValidMoveFound = GetNextMove(ChosenCard);

	// If a valid move was not found, this means that we lost somehow. This is probably impossible at this stage, unless our entire deck is 
	// full of effect cards or something, but this should be prevented by intelligently generating cards in the first place
	if (!bValidMoveFound)
	{
		Winner = EPlayer::Player;
	}
	else
	{
		FB2ServerUpdate UpdateToSend = ExecuteNextMove(ChosenCard);

		OutBoundQueue.Enqueue(UpdateToSend);
	}

	return bValidMoveFound;
}

bool B2AIServer::HandleTie()
{
	bool bNoValidMove = false;

	while (true)
	{
		// If there are cards in the decks, we draw from the deck - else from the hand
		// We only need to check one of the decks as at this stage and (always ?) they should be uniform in size
		if (Cards.PlayerDeck.Num() <= 0)
		{
			// Choose a card to play
			ECard ChosenCard;
			bool bValidMoveFound = GetNextMove(ChosenCard);

			if (bValidMoveFound)
			{
				// Add the card to the field
				Cards.OpponentField.Add(ChosenCard);

				// Update the score
				OpponentScore = CalculateScore(Cards.OpponentField);

				// Send the move to the player
				FB2ServerUpdate Update
				{
					CardToServerUpdate(ChosenCard),
				};

				OutBoundQueue.Enqueue(Update);

				bIsWaitingForOpponentsDrawFromHand = true;
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

			if (PlayerCardValue < OpponentCardValue)
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

	return !bNoValidMove;
}

void B2AIServer::UpdateState(const FB2ServerUpdate& Update)
{
	ECard InCard = ServerUpdateToCard(Update.Update);

	// Depending on the type of card and/or the board state, we either place the card on the field, or execute a special card
	// Here we also check for effects that occurred, so we can use them to branch later
	bool bUsedRodEffect = (InCard == ECard::ElliotsOrbalStaff && Cards.PlayerField.Num() > 0 && IsBolted(Cards.PlayerField.Last()));
	bool bUsedBoltEffect = (InCard == ECard::Bolt);
	bool bUsedMirrorEffect = (InCard == ECard::Mirror);
	bool bUsedBlastEffect = (InCard == ECard::Blast);

	// Note - we dont have to check for force, as we can just add it to the field like normal and recalculate the score

	if (Cards.PlayerField.Num() > 0 && (bUsedRodEffect || bUsedBoltEffect || bUsedMirrorEffect || bUsedBlastEffect))
	{
		if (bUsedBlastEffect)
		{
			// If the card is a blast card, we need to also remove the selected card (specified in metadata) from the opponents hand
			int32 OutInt;
			FDefaultValueHelper::ParseInt(Update.Metadata, OutInt);
			ECard BlastedCard = static_cast<ECard>(OutInt);

			Cards.OpponentHand.RemoveSingle(BlastedCard);
			Cards.OpponentDiscard.Add(BlastedCard);
		}
		else if (bUsedRodEffect)
		{
			// If the card is a rod card (that can be used to "heal" a card) then we need to unbolt the last card on the opponents field
			UnBolt(Cards.PlayerField);
		}
		else if (bUsedBoltEffect)
		{
			// If the card is a bolt card, bolt the last card on the players field
			Bolt(Cards.OpponentField);
		}
		else if (bUsedMirrorEffect)
		{
			// If the card is a mirror card, flip the field
			TArray<ECard> CachedOpponentField = Cards.OpponentField;
			Cards.OpponentField = Cards.PlayerField;
			Cards.PlayerField = CachedOpponentField;
		}
	}
	else
	{
		// If its just a normal card, add it to the field
		Cards.PlayerField.Add(InCard);
	}

	UpdateScores();

	if (PlayerScore == OpponentScore)
	{
		Turn = EPlayer::Undecided;
	}
	else if (PlayerScore > OpponentScore)
	{
		Turn = EPlayer::Opponent;
	}
	else
	{
		Turn = EPlayer::Player;
	}
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

bool B2AIServer::GetNextMove(ECard& OutCard)
{
	// TODO make this smart - At the moment we just choose a random card that wont make use auto lose

	// TODO mirror, bolt, blast cards valid?

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

FB2ServerUpdate B2AIServer::ExecuteNextMove(ECard ChosenCard)
{
	FB2ServerUpdate UpateToSendToPlayer
	{
		CardToServerUpdate(ChosenCard),
	};

	// Depending on the type of card and/or the board state, we either place the card on the field, or execute a special card
	// Here we also check for effects that occurred, so we can use them to branch later
	bool bUsedRodEffect = (ChosenCard == ECard::ElliotsOrbalStaff && Cards.PlayerField.Num() > 0 && IsBolted(Cards.PlayerField.Last()));
	bool bUsedBoltEffect = (ChosenCard == ECard::Bolt);
	bool bUsedMirrorEffect = (ChosenCard == ECard::Mirror);
	bool bUsedBlastEffect = (ChosenCard == ECard::Blast);

	// Note - we dont have to check for force, as we can just add it to the field like normal and recalculate the score

	if (Turn == EPlayer::Opponent && (bUsedRodEffect || bUsedBoltEffect || bUsedMirrorEffect || bUsedBlastEffect))
	{
		if (bUsedBlastEffect)
		{
			// If the card is a blast card, we need to also choose a card from the players hand to remove and send it as metadata

			// Randomly choose a card
			ECard CardToBlast = Cards.PlayerHand[FMath::RandRange(0, Cards.PlayerHand.Num() - 1)];
			Cards.PlayerHand.RemoveSingle(CardToBlast);
			Cards.PlayerDiscard.Add(CardToBlast);

			UpateToSendToPlayer.Metadata = FString::FromInt(static_cast<int32>(CardToBlast));
		}
		else if (bUsedRodEffect)
		{
			// If the card is a rod card (that can be used to "heal" a card) then we need to unbolt the last card on the opponents field
			UnBolt(Cards.OpponentField);
		}
		else if (bUsedBoltEffect)
		{
			// If the card is a bolt card, bolt the last card on the players field
			Bolt(Cards.PlayerField);
		}
		else if (bUsedMirrorEffect)
		{
			// If the card is a mirror card, flip the field
			TArray<ECard> CachedOpponentField = Cards.OpponentField;
			Cards.OpponentField = Cards.PlayerField;
			Cards.PlayerField = CachedOpponentField;
		}
	}
	else
	{
		// If its just a normal card, add it to the field
		Cards.OpponentField.Add(ChosenCard);
	}

	// Update the scores
	UpdateScores();

	return UpateToSendToPlayer;
}

void B2AIServer::Bolt(TArray<ECard>& Field)
{
	ECard BoltedCard = Field[Field.Num() - 1];

	ensureMsgf(BoltedCard <= ECard::Force, TEXT("Attempted to bolt a card that is already bolted"));

	Field[Field.Num() - 1] = static_cast<ECard>(static_cast<uint32>(BoltedCard) + BOLTED_CARD_OFFSET);
}

void B2AIServer::UnBolt(TArray<ECard>& Field)
{
	ECard UnboltedCard = Field[Field.Num() - 1];

	ensureMsgf(UnboltedCard > ECard::Force, TEXT("Attempted to unbolt a card that is not bolted"));

	Field[Field.Num() - 1] = static_cast<ECard>(static_cast<uint32>(UnboltedCard) - BOLTED_CARD_OFFSET);
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

uint32 B2AIServer::CalculateScore(TArray<ECard>& Field)
{
	int32 Total = 0;

	for (size_t i = 0; i < Field.Num(); i++)
	{
		ECard Card = Field[i];

		if (!IsBolted(Card))
		{
			if (Card == ECard::Force && i > 0)
			{
				Total *= 2;
			}
			else
			{
				Total += ACard::TypeToValue(Card);
			}
		}
	}

	return Total;
}

EServerUpdate B2AIServer::CardToServerUpdate(ECard Card) const
{
	EServerUpdate OutUpdateType = EServerUpdate::None;

	if (static_cast<uint32>(Card) >= SERVER_MESSAGE_CARD_MIN || static_cast<uint32>(Card) <= SERVER_MESSAGE_CARD_MAX)
	{
		OutUpdateType = static_cast<EServerUpdate>(static_cast<uint32>(Card) + SERVER_MESSAGE_CARD_OFFSET);
	}

	return OutUpdateType;
}

ECard B2AIServer::ServerUpdateToCard(EServerUpdate Update) const
{
	ECard OutCard = ECard::ElliotsOrbalStaff;

	if (static_cast<uint32>(Update) >= SERVER_MESSAGE_CARD_UPDATE_MIN || static_cast<uint32>(Update) <= SERVER_MESSAGE_CARD_UPDATE_MAX)
	{
		OutCard = static_cast<ECard>(static_cast<uint32>(Update) - SERVER_MESSAGE_CARD_OFFSET);
	}

	return OutCard;
}

void B2AIServer::UpdateScores()
{
	PlayerScore = CalculateScore(Cards.PlayerField);
	OpponentScore = CalculateScore(Cards.OpponentField);
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
