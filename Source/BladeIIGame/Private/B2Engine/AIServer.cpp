#include "B2Engine/AIServer.h"

const uint32 MAX_ECARD_VALUE = 10;

const B2ServerUpdate B2AIServer::GetNextUpdate()
{
	B2ServerUpdate Payload = B2Server::GetNextUpdate();

	// Currently has a test implementation that just sends random cards
	if (!bCardsSent)
	{
		bCardsSent = true;
		Payload.Type = EPayload::Cards;
		Payload.Cards = BoltTest();
	}

	return Payload;
}

FB2Cards B2AIServer::BoltTest() const
{
	FB2Cards Cards;

	for (int i = 14; i >= 0; i--)
	{
		Cards.PlayerDeck.Add(ECard::Bolt);
		Cards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return Cards;
}
