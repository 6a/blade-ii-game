#include "B2Engine/AIOpponent.h"

#include "B2Utility/Log.h"
#include "B2Engine/AIServer.h"

void UB2AIOpponent::Configure(EAIDifficulty Difficulty)
{
	BackEnd = new B2AIServer();

	B2Utility::LogInfo("AI Opponent Initialised");
}
