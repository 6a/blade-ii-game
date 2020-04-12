#include "B2Engine/Server.h"

B2Server::~B2Server()
{
}

const B2ServerUpdate B2Server::GetNextUpdate()
{
	return B2ServerUpdate();
}

void B2Server::SendUpdate(EServerUpdate Update, const FString& MetaData) const
{

}
