#include "B2Engine/Server.h"

UB2Server::~UB2Server()
{
	OutBoundQueue.Empty();
	InBoundQueue.Empty();
}

const FB2ServerUpdate UB2Server::GetNextUpdate()
{
	FB2ServerUpdate InUpdate
	{
		EServerUpdate::None,
	};

	InBoundQueue.Dequeue(InUpdate);

	return InUpdate;
}

void UB2Server::SendUpdate(EServerUpdate Update, const FString& MetaData)
{
	OutBoundQueue.Enqueue(FB2ServerUpdate{Update, MetaData});
}

void UB2Server::Tick(float DeltaSeconds)
{

}
