#include "B2Engine/Server.h"

B2Server::~B2Server()
{
	InboundQueue.Empty();
	OutBoundQueue.Empty();
}

const FB2ServerUpdate B2Server::GetNextUpdate()
{
	FB2ServerUpdate OutUpdate
	{
		EServerUpdate::None,
	};

	OutBoundQueue.Dequeue(OutUpdate);

	return OutUpdate;
}

void B2Server::SendUpdate(EServerUpdate Update, const FString& MetaData)
{
	InboundQueue.Enqueue(FB2ServerUpdate{Update, MetaData});
}

void B2Server::Tick(float DeltaSeconds)
{

}
