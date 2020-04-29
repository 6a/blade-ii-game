#include "B2Engine/PlayerData.h"

#include "Misc/DefaultValueHelper.h"

#include "B2Utility/Log.h"

void B2PlayerData::AddLocalPlayerData(const FString& LocalPlayerRaw)
{
	if (!DeserializeLocalPlayerData(LocalPlayerRaw))
	{
		B2Utility::LogWarning("Unable to deserialise local player data from game server");
	}
}

void B2PlayerData::AddOpponentData(const FString& OpponentRaw)
{
	if (!DeserializeOpponentData(OpponentRaw))
	{
		B2Utility::LogWarning("Unable to deserialise opponent data from game server");
	}
}

bool B2PlayerData::DeserializeLocalPlayerData(const FString& Data)
{
	bool bSuccess = false;
	
	TArray<FString> OutArray;
	int32 Count = Data.ParseIntoArray(OutArray, *DELIMITER);

	if (OutArray.Num() == LOCAL_PLAYER_DATA_SIZE)
	{
		int32 Avatar;
		if (FDefaultValueHelper::ParseInt(OutArray[1], Avatar))
		{
			LocalPlayer.Avatar = static_cast<uint8>(Avatar);
			LocalPlayer.DisplayName = OutArray[0];
			bSuccess = true;
		}
	}

	return bSuccess;
}

bool B2PlayerData::DeserializeOpponentData(const FString& Data)
{
	bool bSuccess = false;

	TArray<FString> OutArray;
	int32 Count = Data.ParseIntoArray(OutArray, *DELIMITER);

	if (OutArray.Num() == OPPONENT_DATA_SIZE)
	{
		int32 Avatar;
		if (FDefaultValueHelper::ParseInt(OutArray[2], Avatar))
		{
			Opponent.Avatar = static_cast<uint8>(Avatar);
			Opponent.DisplayName = OutArray[0];
			Opponent.PublicID = OutArray[1];
			bSuccess = true;
		}
	}

	return bSuccess;
}
