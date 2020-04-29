#pragma once

#include "CoreMinimal.h"

#include "Cards.h"
#include "B2Enum/CardSlotEnum.h"
#include "B2Enum/PlayerEnum.h"

struct B2Player
{
	FString DisplayName;
	FString PublicID;
	uint8 Avatar;
};

struct B2PlayerData
{

public:

	B2Player LocalPlayer;
	B2Player Opponent;

	/**
	 * Add data for the local player
	 * @param LocalPlayerRaw - The local player data string received from the game server
	 */
	void AddLocalPlayerData(const FString& LocalPlayerRaw);
	
	/**
	 * Add data for the opponent
	 * @param OpponentRaw - The opponent data string received from the game server
	 */
	void AddOpponentData(const FString& OpponentRaw);

private:

	/* Decodes a local player data string into the local player member variable */
	bool DeserializeLocalPlayerData(const FString& Data);

	/* Decodes a opponent data string into the opponent member variable */
	bool DeserializeOpponentData(const FString& Data);

};

