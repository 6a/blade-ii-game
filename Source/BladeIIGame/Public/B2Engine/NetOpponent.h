#pragma once

#include "CoreMinimal.h"

#include "Opponent.h"

#include "NetOpponent.generated.h"

UCLASS()
class UB2NetOpponent : public UB2Opponent
{
	GENERATED_BODY()

public:

	/**
	 * Configure this net opponent.
	 * @param PublicID - Public ID of the local user.
	 * @param AuthToken - Auth token for the local user.
	 * @param MatchID - Match ID.
	 * @param World - A pointer to the current world.
	 */
	void Configure(const FString& PublicID, const FString& AuthToken, uint64 MatchID, UWorld* World);
};