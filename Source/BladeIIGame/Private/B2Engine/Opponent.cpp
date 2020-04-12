#include "B2Engine/Opponent.h"

#include "B2Utility/Log.h"

void UB2Opponent::Tick(float DeltaSeconds)
{
	B2ServerUpdate ServerUpdate = BackEnd->GetNextUpdate();

	if (ServerUpdate.Update == EServerUpdate::InstructionCards)
	{
		if (OnCardsReceived.IsBound()) OnCardsReceived.Broadcast(FB2Cards(ServerUpdate.Metadata));
	}
}

void UB2Opponent::SendMove(const FB2Move& Move)
{

}

void UB2Opponent::SendInstruction(const EInstruction& Instruction)
{

}
