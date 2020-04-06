#include "B2Engine/Opponent.h"

#include "B2Misc/Utility.h"

void UB2Opponent::Tick(float DeltaSeconds)
{
	B2ServerUpdate ServerUpdate = BackEnd->GetNextUpdate();

	switch (ServerUpdate.Type)
	{
		case EPayload::WaitingForInitialDeal:

			break;
		case EPayload::Cards:
			if (OnCardsReceived.IsBound()) OnCardsReceived.Broadcast(ServerUpdate.Cards);
			break;
		case EPayload::Move:

			break;
		case EPayload::Instruction:

			break;
	}
}

void UB2Opponent::SendMove(const FB2Move& Move)
{

}

void UB2Opponent::SendInstruction(const EInstruction& Instruction)
{

}
