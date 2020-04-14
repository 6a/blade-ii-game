#include "B2Engine/NetOpponent.h"

#include "B2Utility/Log.h"
#include "B2Engine/NetServer.h"

void UB2NetOpponent::Configure(FString PublicID, FString AuthToken, uint64 MatchID)
{
	BackEnd = new B2NetServer();

	B2Utility::LogInfo("Net Opponent Initialised");
}