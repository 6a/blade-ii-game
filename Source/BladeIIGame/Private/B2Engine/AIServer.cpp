#include "B2Engine/AIServer.h"

#include "Algo/Reverse.h"
#include "Misc/DefaultValueHelper.h"

#include "B2Game/Card.h"
#include "B2Predicate/FilterFirstOfType.h"
#include "B2Predicate/IsNotEffectCard.h"
#include "B2Utility/String.h"
#include "B2Utility/Log.h"
#include "B2Utility/Array.h"

const FB2ServerUpdate UB2AIServer::GetNextUpdate()
{
	FB2ServerUpdate Payload = UB2Server::GetNextUpdate();

	if (!bCardsSent)
	{
		FB2Cards OutCards;

		if (AIDifficulty < EAIDifficulty::Tutorial)
		{
			// TODO do something based on the difficulty

			// Generate the cards for this match
			Cards = GenerateCards();

			// Make a copy of the cards to send to the player, so we can freely modify the one we just created and stored interanlly
			OutCards = Cards;

			// Set up the initial internal state based on the cards
			ConfigureInitialState();
		}
		else
		{
			// Generate the cards for this match
			Cards = GenerateTutorialCards();

			// Make a copy of the cards to send to the player, so we can freely modify the one we just created and stored interanlly
			OutCards = Cards;

			TutorialPhase = 0;
		}

		Payload.Code = EServerUpdate::InstructionCards;
		Payload.Payload = OutCards.GetSerialised(0);

		bCardsSent = true;
	}

	return Payload;
}

void UB2AIServer::Tick(float DeltaSeconds)
{
	if (Winner != EPlayer::Undecided)
	{
		return;
	}

	// Early exit and handler for tutorial game
	if (AIDifficulty == EAIDifficulty::Tutorial) 
	{
		HandleTutorial();

		return;
	}

	// Handle messages "to" the server (rather, to us)
	FB2ServerUpdate IncomingUpdate;
	if (OutBoundQueue.Dequeue(IncomingUpdate))
	{
		// Ignore "None" or message types for now
		if (IncomingUpdate.Code == EServerUpdate::None || IncomingUpdate.Code == EServerUpdate::InstructionMessage)
		{
			return;
		}

		// Update the internal card state
		UpdateState(IncomingUpdate);

		// Now we iterate until we have successfully moved into either the Undecided state, or the players turn
		ResolveAITurn();
	}
}

void UB2AIServer::SetDifficulty(EAIDifficulty Difficulty)
{
	AIDifficulty = Difficulty;
}

void UB2AIServer::ConfigureInitialState()
{
	// Note - The AI player is always player 1, or in this case, the "opponent"

	// Place the cards from the decks onto the hands
	for (int32 i = static_cast<int32>(DECK_SLOT_MAX) - 1; i >= static_cast<int32>(DECK_SLOT_MAX) - static_cast<int32>(HAND_SLOT_MAX); i--)
	{
		ECard PlayerCard = RemoveLast(Cards.PlayerDeck);
		Cards.PlayerHand.Add(PlayerCard);

		ECard OpponentCard = RemoveLast(Cards.OpponentDeck);
		Cards.OpponentHand.Add(OpponentCard);
	}

	// Set the initial scores
	PlayerScore = AIScore = 0;

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
		SetAILoss("Could not find an initial move to play");
	}
	else if (Turn == EPlayer::Opponent) // Otherwise, if its our turn, choose a card to play and play it - sending the play to the player afterwards
	{
		// Update the scores
		UpdateScores();

		// Execute the opponents turn - no need to check for success as we exit after this call anyway
		ResolveAITurn();
	}
}

bool UB2AIServer::ExecuteTurn()
{
	// Choose a card to play
	ECard ChosenCard;
	bool bValidMoveFound = GetNextMove(ChosenCard);

	// If a valid move was not found, this means that we lost somehow. This is probably impossible at this stage, unless our entire deck is 
	// full of effect cards or something, but this should be prevented by intelligently generating cards in the first place
	if (!bValidMoveFound)
	{
		SetAILoss("Could not get next move");
	}
	else
	{
		FB2ServerUpdate UpdateToSend = ExecuteMove(ChosenCard);

		InBoundQueue.Enqueue(UpdateToSend);
	}

	UpdateTurn();

	return bValidMoveFound;
}

void UB2AIServer::UpdateTurn()
{
	if (PlayerScore == AIScore)
	{
		Turn = EPlayer::Undecided;
	}
	else if (PlayerScore > AIScore)
	{
		Turn = EPlayer::Opponent;
	}
	else
	{
		Turn = EPlayer::Player;
	}
}

bool UB2AIServer::HandleTie()
{
	bool bNoValidMove = false;

	while (true)
	{
		// Clear the field
		Cards.PlayerDiscard.Append(Cards.PlayerField);
		Cards.PlayerField.Empty();

		Cards.OpponentDiscard.Append(Cards.OpponentField);
		Cards.OpponentField.Empty();

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
				AIScore = CalculateScore(Cards.OpponentField);

				// Send the move to the player
				FB2ServerUpdate Update
				{
					CardToServerUpdate(ChosenCard),
				};

				InBoundQueue.Enqueue(Update);
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

	UpdateScores();

	return !bNoValidMove;
}

void UB2AIServer::UpdateState(const FB2ServerUpdate& Update)
{
	ECard InCard = ServerUpdateToCard(Update.Code);
	Cards.PlayerHand.RemoveSingle(InCard);

	// Depending on the type of card and/or the board state, we either place the card on the field, or execute a special card
	// Here we also check for effects that occurred, so we can use them to branch later
	// Note - we dont have to check for force, as we can just add it to the field like normal and recalculate the score
	bool bUsedRodEffect = (InCard == ECard::ElliotsOrbalStaff && Cards.PlayerField.Num() > 0 && IsBolted(Cards.PlayerField.Last()));
	bool bUsedBoltEffect = (InCard == ECard::Bolt) && Cards.OpponentField.Num() > 0 && !IsBolted(Cards.OpponentField.Last());
	bool bUsedMirrorEffect = (InCard == ECard::Mirror) && Cards.PlayerField.Num() > 0 && Cards.OpponentField.Num() > 0;
	bool bUsedBlastEffect = (InCard == ECard::Blast) && Cards.OpponentHand.Num() > 0;
	bool bUsedForceEffect = (InCard == ECard::Force && PlayerScore > 0);
	bool bUsedNormalOrForceCard = (!bUsedRodEffect && !bUsedBoltEffect && !bUsedMirrorEffect && !bUsedBlastEffect) || bUsedForceEffect;

	// If the selected card was a normal card or a force card, and the players lastest field card is flipped, remove it
	if (Cards.PlayerField.Num() > 0 && IsBolted(Cards.PlayerField.Last()) && bUsedNormalOrForceCard)
	{
		Cards.PlayerDiscard.Add(RemoveLast(Cards.PlayerField));
	}

	if (Cards.PlayerField.Num() > 0 && !bUsedNormalOrForceCard)
	{
		if (bUsedBlastEffect)
		{
			// If the card is a blast card, we need to also remove the selected card (specified in metadata) from the opponents hand
			int32 OutInt;
			FDefaultValueHelper::ParseInt(Update.Payload, OutInt);
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

		// Discard the card
		Cards.PlayerDiscard.Add(InCard);
	}
	else
	{
		// Add the card to the field
		Cards.PlayerField.Add(InCard);
	}

	UpdateScores();

	UpdateTurn();
}

ECard UB2AIServer::RemoveLast(TArray<ECard>& FromArray)
{
	ECard Card = ECard::InactiveForce;

	if (FromArray.Num() > 0)
	{
		Card = FromArray.Last();
		FromArray.RemoveAt(FromArray.Num() - 1);
	}
		
	return Card;
}

bool UB2AIServer::GetNextMove(ECard& OutCard)
{
	// TODO make this smart - At the moment we just choose a random card that wont make use auto lose

	TArray<ECard> ValidCards;
	uint32 ScoreDifference = PlayerScore - AIScore;

	// Edge case - if there is only 1 card left in the hand, treat it as valid (as long as its not an effect card, as 
	// playing an effect card as your last is illegal)
	if (Cards.OpponentHand.Num() == 1 && Cards.OpponentHand.Last() < ECard::Force)
	{
		ValidCards.Add(Cards.OpponentHand.Last());
	}
	else
	{
		for (ECard Card : Cards.OpponentHand)
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

				// This is a bolt card
				if (Card == ECard::Bolt)
				{
					// The player has at least one card on their field, which is not already bolted/disabled
					if (Cards.PlayerField.Num() > 0 && !IsBolted(Cards.PlayerField.Last()))
					{
						// If the players score will be lower or equal to our score after bolting
						TArray<ECard> PlayerFieldBolted = Cards.PlayerField;
						Bolt(PlayerFieldBolted);
						uint32 PostBoltPlayerScore = CalculateScore(PlayerFieldBolted);
						if (AIScore >= PostBoltPlayerScore)
						{
							// If so, its valid to play this card
							ValidCards.Add(Card);
							continue;
						}
					}
				}

				// This is a mirror card
				if (Card == ECard::Mirror)
				{
					// If fliping the field will either improve or match the opponents current score
					if (PlayerScore >= AIScore)
					{
						// If so, its valid to play this card
						ValidCards.Add(Card);
						continue;
					}
				}

				// This is a blast card
				if (Card == ECard::Blast)
				{
					// If the player has at least one card in their hand
					if (Cards.PlayerHand.Num() > 0)
					{
						// If so, its valid to play this card
						ValidCards.Add(Card);
						continue;
					}
				}

				// This is a force card
				if (Card == ECard::Force)
				{
					// If we played this force card, the resulting score would be high enough to match or beat the other score
					if (AIScore * 2 >= PlayerScore)
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
		B2Utility::LogWarning(FString("Could not find any valid cards!"));

		return false;
	}

	ECard CardToPlay = ValidCards[FMath::RandRange(0, ValidCards.Num() - 1)];
	Cards.OpponentHand.RemoveSingle(CardToPlay);

	OutCard = CardToPlay;

	return true;
}

FB2ServerUpdate UB2AIServer::ExecuteMove(ECard ChosenCard)
{
	FB2ServerUpdate UpateToSendToPlayer
	{
		CardToServerUpdate(ChosenCard),
	};

	// Depending on the type of card and/or the board state, we either place the card on the field, or execute a special card
	// Here we also check for effects that occurred, so we can use them to branch later
	bool bUsedRodEffect = (ChosenCard == ECard::ElliotsOrbalStaff && Cards.OpponentField.Num() > 0 && IsBolted(Cards.OpponentField.Last()));
	bool bUsedBoltEffect = (ChosenCard == ECard::Bolt) && Cards.PlayerField.Num() > 0 && !IsBolted(Cards.PlayerField.Last());
	bool bUsedMirrorEffect = (ChosenCard == ECard::Mirror) && Cards.PlayerField.Num() > 0 && Cards.OpponentField.Num() > 0;
	bool bUsedBlastEffect = (ChosenCard == ECard::Blast) && Cards.PlayerHand.Num() > 0;
	bool bUsedForceEffect = (ChosenCard == ECard::Force && AIScore > 0);
	bool bUsedNormalOrForceCard = (!bUsedRodEffect && !bUsedBoltEffect && !bUsedMirrorEffect && !bUsedBlastEffect) || bUsedForceEffect;

	// If the selected card was a normal card or a force card, and the opponents lastest field card is flipped, remove it
	if (Cards.OpponentField.Num() > 0 && IsBolted(Cards.OpponentField.Last()) && bUsedNormalOrForceCard)
	{
		Cards.OpponentDiscard.Add(RemoveLast(Cards.OpponentField));
	}

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

			UpateToSendToPlayer.Payload = FString::FromInt(static_cast<int32>(CardToBlast));
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

void UB2AIServer::Bolt(TArray<ECard>& Field)
{
	ECard BoltedCard = Field[Field.Num() - 1];

	ensureMsgf(BoltedCard <= ECard::Force, TEXT("Attempted to bolt a card that is already bolted"));

	Field[Field.Num() - 1] = static_cast<ECard>(static_cast<uint32>(BoltedCard) + BOLTED_CARD_OFFSET);
}

void UB2AIServer::UnBolt(TArray<ECard>& Field)
{
	ECard UnboltedCard = Field[Field.Num() - 1];

	ensureMsgf(UnboltedCard > ECard::Force, TEXT("Attempted to unbolt a card that is not bolted"));

	Field[Field.Num() - 1] = static_cast<ECard>(static_cast<uint32>(UnboltedCard) - BOLTED_CARD_OFFSET);
}

bool UB2AIServer::IsBolted(ECard Card) const
{
	return Card > ECard::Force;
}

uint32 UB2AIServer::GetBoltedCardRealValue(ECard Card) const
{
	int32 OutValue = 0;

	if (Card > ECard::Force)
	{
		Card = static_cast<ECard>(static_cast<uint32>(Card) - BOLTED_CARD_OFFSET);
	}

	OutValue = ACard::TypeToValue(Card);

	return OutValue;
}

uint32 UB2AIServer::CalculateScore(TArray<ECard>& Field) const
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

EServerUpdate UB2AIServer::CardToServerUpdate(ECard Card) const
{
	EServerUpdate OutUpdateType = EServerUpdate::None;

	if (static_cast<uint32>(Card) >= SERVER_MESSAGE_CARD_MIN || static_cast<uint32>(Card) <= SERVER_MESSAGE_CARD_MAX)
	{
		OutUpdateType = static_cast<EServerUpdate>(static_cast<uint32>(Card) + SERVER_MESSAGE_CARD_OFFSET);
	}

	return OutUpdateType;
}

ECard UB2AIServer::ServerUpdateToCard(EServerUpdate Update) const
{
	ECard OutCard = ECard::ElliotsOrbalStaff;

	if (static_cast<uint32>(Update) >= SERVER_MESSAGE_CARD_UPDATE_MIN && static_cast<uint32>(Update) <= SERVER_MESSAGE_CARD_UPDATE_MAX)
	{
		OutCard = static_cast<ECard>(static_cast<uint32>(Update) - SERVER_MESSAGE_CARD_OFFSET);
	}

	return OutCard;
}

void UB2AIServer::UpdateScores()
{
	PlayerScore = CalculateScore(Cards.PlayerField);
	AIScore = CalculateScore(Cards.OpponentField);
}

void UB2AIServer::ResolveAITurn()
{
	while (Turn != EPlayer::Player)
	{
		if (PlayerScore == AIScore)
		{
			bool bSuccess = HandleTie();

			// If we could not break the tie, its a game over - but the player should already be aware. 
			// Just set the state to a player win, as it doesnt matter anyway
			if (!bSuccess)
			{
				SetAILoss("Could not break tie");
				break;
			}

			if (Turn == EPlayer::Undecided)
			{
				break;
			}
		}
		else
		{
			bool bSuccess = ExecuteTurn();
			if (!bSuccess)
			{
				// Game over?
				break;
			}
		}
	}
}

void UB2AIServer::SetAILoss(const FString& Reason)
{
	B2Utility::LogWarning(FString("AI entered loss state: ").Append(Reason));

	Winner = EPlayer::Player;

	BoltTest();
}

FB2Cards UB2AIServer::GenerateCards() const
{
	FB2Cards GeneratedCards;

	// Add all the cards (ref: https://www.reddit.com/r/Falcom/comments/fxt5nq/can_i_buy_the_card_game_blade_anywhere/fmxo8qo/)
	TArray<ECard> Pool = {
		ECard::ElliotsOrbalStaff, ECard::ElliotsOrbalStaff,
		ECard::FiesTwinGunswords, ECard::FiesTwinGunswords, ECard::FiesTwinGunswords, ECard::FiesTwinGunswords, ECard::FiesTwinGunswords,
		ECard::AlisasOrbalBow, ECard::AlisasOrbalBow, ECard::AlisasOrbalBow, ECard::AlisasOrbalBow, ECard::AlisasOrbalBow,
		ECard::JusisSword, ECard::JusisSword, ECard::JusisSword, ECard::JusisSword, ECard::JusisSword,
		ECard::MachiasOrbalShotgun, ECard::MachiasOrbalShotgun, ECard::MachiasOrbalShotgun, ECard::MachiasOrbalShotgun,
		ECard::GaiusSpear, ECard::GaiusSpear, ECard::GaiusSpear,
		ECard::LaurasGreatsword, ECard::LaurasGreatsword,
		ECard::Bolt, ECard::Bolt, ECard::Bolt, ECard::Bolt,
		ECard::Mirror, ECard::Mirror, ECard::Mirror, ECard::Mirror,
		ECard::Blast, ECard::Blast, ECard::Blast, ECard::Blast,
		ECard::Force, ECard::Force,
	};

	bool bSuccess = false;
	while (!bSuccess)
	{
		B2Utility::ShuffleArray(Pool);

		GeneratedCards.PlayerDeck = TArray<ECard>(Pool.GetData(), 15);
		GeneratedCards.OpponentDeck = TArray<ECard>(Pool.GetData() + (Pool.GetTypeSize() * 15), 15);

		bSuccess = ValidateCards(GeneratedCards);
	}

	return GeneratedCards;
}

FB2Cards UB2AIServer::GenerateTutorialCards() const
{
	FB2Cards GeneratedCards;

	GeneratedCards.PlayerDeck = TArray<ECard>{
		ECard::Bolt,
		ECard::AlisasOrbalBow,
		ECard::Force,
		ECard::GaiusSpear,
		ECard::FiesTwinGunswords,
		ECard::GaiusSpear,
		ECard::Blast,
		ECard::Mirror,
		ECard::LaurasGreatsword,
		ECard::MachiasOrbalShotgun,
		ECard::JusisSword,
		ECard::ElliotsOrbalStaff,
		ECard::Bolt,
		ECard::AlisasOrbalBow,
		ECard::Force,
	};

	GeneratedCards.OpponentDeck = TArray<ECard>{
		ECard::ElliotsOrbalStaff, 
		ECard::AlisasOrbalBow, 
		ECard::FiesTwinGunswords,
		ECard::AlisasOrbalBow,
		ECard::MachiasOrbalShotgun, 
		ECard::FiesTwinGunswords, 
		ECard::FiesTwinGunswords,
		ECard::ElliotsOrbalStaff,
		ECard::Bolt,
		ECard::Mirror,
		ECard::FiesTwinGunswords, 
		ECard::JusisSword, 
		ECard::MachiasOrbalShotgun,
		ECard::JusisSword,
		ECard::GaiusSpear,
	};

	return GeneratedCards;
}

bool UB2AIServer::ValidateCards(const FB2Cards& InCards) const
{
	for (size_t i = 0; i < MAX_DRAW_ON_START; i++)
	{
		uint32 CardIndex = POST_INIT_DECK_SIZE - 1 - i;

		// break - just in case
		if (CardIndex < 0)
		{
			break;
		}

		int32 LocalPlayerScore = ACard::TypeToValue(InCards.PlayerDeck[CardIndex]);
		int32 OpponentScore = ACard::TypeToValue(InCards.OpponentDeck[CardIndex]);
		int32 ScoreDifference = FMath::Abs(LocalPlayerScore - OpponentScore);
		
		// If the scores are not equal, we then go on to check if there is a valid move that can be made by the player who goes first
		if (ScoreDifference != 0)
		{
			TArray<ECard> HandToCheck;
			ECard CardToBeatOrMatch;
			uint32 CurrentScore;

			// Determine the data set to work on
			if (LocalPlayerScore < OpponentScore)
			{
				HandToCheck = TArray<ECard>(InCards.PlayerDeck.GetData(), 15);
				CardToBeatOrMatch = InCards.OpponentDeck[CardIndex];
				CurrentScore = LocalPlayerScore;
			}
			else
			{
				HandToCheck = TArray<ECard>(InCards.OpponentDeck.GetData(), 15);
				CardToBeatOrMatch = InCards.PlayerDeck[CardIndex];
				CurrentScore = OpponentScore;
			}

			// Reverse the hand to check (as it will get drawn from the top of the deck, rather than the middle)
			Algo::Reverse(HandToCheck);

			return ValidFirstMoveAvailable(HandToCheck, CardToBeatOrMatch, CurrentScore);
		}
	}


	return false;
}

bool UB2AIServer::ValidFirstMoveAvailable(const TArray<ECard>& CardSet, ECard CardToBeatOrMatch, uint32 CurrentScore) const
{
	// Early exit if there is only one card, though this should never happen on the local client
	if (CardSet.Num() == 1)
	{
		return true;
	}

	// This implementation is greedy but it should be fine - TODO see if this is causing a spike
	for (size_t i = 0; i < CardSet.Num(); i++)
	{
		// Blast are always invalid as they dont change the score
		if (CardSet[i] != ECard::Blast)
		{
			// If the new score beats or matches the target score, its valid
			if (CurrentScore + ACard::TypeToValue(CardSet[i]) >= ACard::TypeToValue(CardToBeatOrMatch))
			{
				return true;
			}

			// If the card is a force card and playing it would beat or match the target score, its valid
			if (CardSet[i] == ECard::Force)
			{
				if (CurrentScore * 2 >= ACard::TypeToValue(CardToBeatOrMatch))
				{
					return true;
				}
			}

			// Bolts and mirrors are always valid to play from this position
			if (CardSet[i] == ECard::Bolt || CardSet[i] == ECard::Mirror)
			{
				return true;
			}
		}
	}

	return false;
}

void UB2AIServer::HandleTutorial()
{
	// Handle messages from the player
	FB2ServerUpdate IncomingUpdate;
	if (OutBoundQueue.Dequeue(IncomingUpdate))
	{
		// Ignore "None" or message types for now
		if (IncomingUpdate.Code == EServerUpdate::None)
		{
			return;
		}

		// Handle the trigger that tells us to switch back to normal mode
		if (IncomingUpdate.Code == EServerUpdate::InstructionTutorialFinished)
		{
			AIDifficulty = EAIDifficulty::Intermediate;

			ConfigureInitialState();
		}
	}
}

FB2Cards UB2AIServer::BoltTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(FMath::RandBool() ? ECard::Bolt : static_cast<ECard>(FMath::RandRange(1, 6)));
		GeneratedCards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return GeneratedCards;
}

FB2Cards UB2AIServer::OpponentBoltTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
		GeneratedCards.OpponentDeck.Add(FMath::RandBool() ? ECard::Bolt : static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return GeneratedCards;
}

FB2Cards UB2AIServer::BlastTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(FMath::RandBool() ? ECard::Blast : static_cast<ECard>(FMath::RandRange(1, 6)));
		GeneratedCards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(0, 10)));
	}

	return GeneratedCards;
}

FB2Cards UB2AIServer::OpponentBlastTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(static_cast<ECard>(FMath::RandRange(0, 10)));
		GeneratedCards.OpponentDeck.Add(FMath::RandBool() ? ECard::Blast : static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return GeneratedCards;
}

FB2Cards UB2AIServer::ForceTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(FMath::RandBool() ? ECard::Force : static_cast<ECard>(FMath::RandRange(1, 6)));
		GeneratedCards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return GeneratedCards;
}

FB2Cards UB2AIServer::OpponentForceTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
		GeneratedCards.OpponentDeck.Add(FMath::RandBool() ? ECard::Force : static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return GeneratedCards;
}

FB2Cards UB2AIServer::RodTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(FMath::RandBool() ? ECard::ElliotsOrbalStaff : static_cast<ECard>(FMath::RandRange(1, 6)));
		GeneratedCards.OpponentDeck.Add(FMath::RandBool() ? ECard::Bolt : static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return GeneratedCards;
}

FB2Cards UB2AIServer::OpponentRodTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(FMath::RandBool() ? ECard::Bolt : static_cast<ECard>(FMath::RandRange(1, 6)));
		GeneratedCards.OpponentDeck.Add(FMath::RandBool() ? ECard::ElliotsOrbalStaff : static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return GeneratedCards;
}

FB2Cards UB2AIServer::MirrorTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(FMath::RandBool() ? ECard::Mirror : static_cast<ECard>(FMath::RandRange(1, 6)));
		GeneratedCards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return GeneratedCards;
}

FB2Cards UB2AIServer::OpponentMirrorTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
		GeneratedCards.OpponentDeck.Add(FMath::RandBool() ? ECard::Mirror : static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return GeneratedCards;
}

FB2Cards UB2AIServer::StandardCardsOnlyTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
		GeneratedCards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return GeneratedCards;
}

FB2Cards UB2AIServer::CardOverlapTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(static_cast<ECard>(FMath::Clamp(i, 0, 9)));
		GeneratedCards.OpponentDeck.Add(ECard::Force);
	}

	return GeneratedCards;
}

FB2Cards UB2AIServer::DupeTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(ECard::LaurasGreatsword);
		GeneratedCards.OpponentDeck.Add(ECard::LaurasGreatsword);
	}

	return GeneratedCards;
}

FB2Cards UB2AIServer::PlayerFirstTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(static_cast<ECard>(FMath::RandRange(0, 4)));
		GeneratedCards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(5, 6)));
	}

	return GeneratedCards;
}

FB2Cards UB2AIServer::AIFirstTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(static_cast<ECard>(FMath::RandRange(5, 6)));
		GeneratedCards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(0, 4)));
	}

	return GeneratedCards;
}

FB2Cards UB2AIServer::AllRandomTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(static_cast<ECard>(FMath::RandRange(0, 10)));
		GeneratedCards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(0, 10)));
	}

	return GeneratedCards;
}

FB2Cards UB2AIServer::MirrorBoltTest() const
{
	FB2Cards GeneratedCards;

	for (int i = 14; i >= 0; i--)
	{
		GeneratedCards.PlayerDeck.Add(FMath::RandBool() ? ECard::Mirror : static_cast<ECard>(FMath::RandRange(0, 6)));
		GeneratedCards.OpponentDeck.Add(FMath::RandBool() ? ECard::Bolt : static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return GeneratedCards;	
}
