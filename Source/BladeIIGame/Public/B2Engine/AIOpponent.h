#pragma once

#include "CoreMinimal.h"

#include "Opponent.h"
#include "B2Misc/Enum/AIDifficultyEnum.h"

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
	void Configure(EAIDifficulty Difficulty);
};