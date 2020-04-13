#include "B2Engine/AIServer.h"

const uint32 MAX_ECARD_VALUE = 10;

const B2ServerUpdate B2AIServer::GetNextUpdate()
{
	B2ServerUpdate Payload = B2Server::GetNextUpdate();

	if (!bCardsSent)
	{
		Payload.Update = EServerUpdate::InstructionCards;
		Payload.Metadata = MirrorTest().GetSerialised(0);

		bCardsSent = true;
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

FB2Cards B2AIServer::BlastTest() const
{
	FB2Cards Cards;

	for (int i = 14; i >= 0; i--)
	{
		Cards.PlayerDeck.Add(ECard::Blast);
		Cards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return Cards;
}

FB2Cards B2AIServer::ForceTest() const
{
	FB2Cards Cards;

	for (int i = 14; i >= 0; i--)
	{
		ECard Type = static_cast<bool>(FMath::RandRange(0, 1)) ? ECard::Force : ECard::GaiusSpear;

		Cards.PlayerDeck.Add(Type);
		Cards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return Cards;
}

FB2Cards B2AIServer::RodTest() const
{
	FB2Cards Cards;

	for (int i = 14; i >= 0; i--)
	{
		ECard Type = static_cast<bool>(FMath::RandRange(0, 1)) ? ECard::ElliotsOrbalStaff : ECard::GaiusSpear;

		Cards.PlayerDeck.Add(Type);
		Cards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return Cards;
}

FB2Cards B2AIServer::MirrorTest() const
{
	FB2Cards Cards;

	for (int i = 14; i >= 0; i--)
	{
		ECard Type = static_cast<bool>(FMath::RandRange(0, 1)) ? ECard::Mirror : static_cast<ECard>(FMath::RandRange(0, 6));

		Cards.PlayerDeck.Add(Type);
		Cards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return Cards;
}

FB2Cards B2AIServer::StandardCardsOnlyTest() const
{
	FB2Cards Cards;

	for (int i = 14; i >= 0; i--)
	{
		Cards.PlayerDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
		Cards.OpponentDeck.Add(static_cast<ECard>(FMath::RandRange(1, 6)));
	}

	return Cards;
}

FB2Cards B2AIServer::CardOverlapTest() const
{
	FB2Cards Cards;

	for (int i = 14; i >= 0; i--)
	{
		Cards.PlayerDeck.Add(static_cast<ECard>(FMath::Clamp(i, 0, 9)));
		Cards.OpponentDeck.Add(ECard::Force);
	}

	return Cards;
}
