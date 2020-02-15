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
	 */
	virtual void Configure(FString PublicID, FString AuthToken, uint64 MatchID) override;

};