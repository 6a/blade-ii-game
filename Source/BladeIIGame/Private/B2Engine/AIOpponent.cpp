#include "B2Engine/AIOpponent.h"

#include "UObject/UObjectGlobals.h"

#include "B2Utility/Log.h"
#include "B2Engine/AIServer.h"

void UB2AIOpponent::Configure(EAIDifficulty Difficulty)
{
	UB2AIServer* AIServer = NewObject<UB2AIServer>();
	AIServer->SetDifficulty(Difficulty);

	BackEnd = AIServer;


	B2Utility::LogInfo("AI Opponent Initialised");
}
