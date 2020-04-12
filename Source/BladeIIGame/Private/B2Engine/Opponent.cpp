#include "B2Engine/Opponent.h"

void UB2Opponent::Tick(float DeltaSeconds)
{
	B2ServerUpdate ServerUpdate = BackEnd->GetNextUpdate();

	if (ServerUpdate.Update == EServerUpdate::InstructionCards)
	{
		if (OnCardsReceived.IsBound()) OnCardsReceived.Broadcast(FB2Cards(ServerUpdate.Metadata));
	}
	else
	{
		if (OnServerUpdateReceived.IsBound()) OnServerUpdateReceived.Broadcast(ServerUpdate.Update, ServerUpdate.Metadata);
	}
}

void UB2Opponent::SendUpdate(EServerUpdate Update, const FString& MetaData) const
{
	UE_LOG(LogTemp, Warning, TEXT("SENDING UPDATE TO SERVER"));
}
