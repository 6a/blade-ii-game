#pragma once

#include "CoreMinimal.h"

#include "Opponent.h"
#include "B2Enum/AIDifficulty.h"

#include "AIOpponent.generated.h"

UCLASS()
class UB2AIOpponent : public UB2Opponent
{
	GENERATED_BODY()

public:

	/**
	 * Configure the AI opponent.
	 * @param EAIDifficulty - The difficulty of the AI opponent.
	 */
	virtual void Configure(EAIDifficulty Difficulty) override;
};