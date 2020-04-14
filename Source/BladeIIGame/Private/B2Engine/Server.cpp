#include "B2Engine/Server.h"

B2Server::~B2Server()
{
}

const FB2ServerUpdate B2Server::GetNextUpdate()
{
	return FB2ServerUpdate
	{
		EServerUpdate::None,
	};
}

void B2Server::SendUpdate(EServerUpdate Update, const FString& MetaData) const
{

}
