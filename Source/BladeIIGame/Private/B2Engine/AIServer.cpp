#include "B2Engine/AIServer.h"

const uint32 MAX_ECARD_VALUE = 10;

const B2ServerUpdate B2AIServer::GetNextUpdate()
{
	B2ServerUpdate Payload = B2Server::GetNextUpdate();

	// Current has a test implementation that just sends random cards
	if (!bCardsSent)
	{
		bCardsSent = true;
		Payload.Type = EPayload::Cards;
		Payload.Cards = FB2Cards();

		// Player Deck
		for (int i = 14; i >= 0; --i)
		{
			ECard Card = static_cast<ECard>(FMath::RandRange(7, 8));
			Payload.Cards.PlayerDeck.Add(Card);
		}

		// Opponent Deck
		for (int i = 14; i >= 0; --i)
		{
			ECard Card = static_cast<ECard>(FMath::RandRange(7, 8));
			Payload.Cards.OpponentDeck.Add(Card);
		}
	}

	return Payload;
}
