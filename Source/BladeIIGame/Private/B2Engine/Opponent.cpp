#include "B2Engine/Opponent.h"

#include "B2Utility/Log.h"

void UB2Opponent::Tick(float DeltaSeconds)
{
	BackEnd->Tick(DeltaSeconds);

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
			B2Utility::LogInfo(FString::Printf(TEXT("Opponent received card instruction from server; [ %d ]"), ServerUpdate.Update));

			MoveUpdateQueue.Enqueue(ServerUpdate);
		}

		ServerUpdate = BackEnd->GetNextUpdate();
	}
}

void UB2Opponent::SendUpdate(EServerUpdate Update, const FString& MetaData) const
{
	UE_LOG(LogTemp, Warning, TEXT("SENDING UPDATE TO SERVER"));

	BackEnd->SendUpdate(Update, MetaData);
}
