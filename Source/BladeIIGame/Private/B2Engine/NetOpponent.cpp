#include "B2Engine/NetOpponent.h"

#include "UObject/UObjectGlobals.h"

#include "B2Utility/Log.h"
#include "B2Engine/NetServer.h"

void UB2NetOpponent::Configure(const FString& PublicID, const FString& AuthToken, uint64 MatchID, UWorld* World)
{
	UB2NetServer* NetServer = NewObject<UB2NetServer>();

	NetServer->Initialise(PublicID, AuthToken, MatchID, World);
	BackEnd = NetServer;

	B2Utility::LogInfo("Net Opponent Initialised");
}