#include "B2Engine/AIOpponent.h"

#include "B2Engine/AIServer.h"

void UB2AIOpponent::Configure(EAIDifficulty Difficulty)
{
	BackEnd = new B2AIServer();
}
