#include "B2Engine/Opponent.h"

void UB2Opponent::Tick(float DeltaSeconds)
{
	FB2ServerUpdate ServerUpdate = BackEnd->GetNextUpdate();

	while (ServerUpdate.Update != EServerUpdate::None)
	{
		if (ServerUpdate.Update == EServerUpdate::InstructionCards)
		{
			if (OnCardsReceived.IsBound()) OnCardsReceived.Broadcast(FB2Cards(ServerUpdate.Metadata));
		}
		else if (ServerUpdate.Update >= EServerUpdate::InstructionQuit)
		{
			if (OnInstructionReceived.IsBound()) OnInstructionReceived.Broadcast(ServerUpdate);
		}
		else
		{
			MoveUpdateQueue.Enqueue(ServerUpdate);
		}

		ServerUpdate = BackEnd->GetNextUpdate();
	}
}

void UB2Opponent::SendUpdate(EServerUpdate Update, const FString& MetaData) const
{
	UE_LOG(LogTemp, Warning, TEXT("SENDING UPDATE TO SERVER"));
}
