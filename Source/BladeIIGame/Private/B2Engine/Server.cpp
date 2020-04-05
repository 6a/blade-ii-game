#include "B2Engine/Server.h"

B2Server::~B2Server()
{
}

const B2ServerUpdate B2Server::GetNextUpdate()
{
	B2ServerUpdate Payload
	{
		EPayload::None,
	};

	return Payload;
}
