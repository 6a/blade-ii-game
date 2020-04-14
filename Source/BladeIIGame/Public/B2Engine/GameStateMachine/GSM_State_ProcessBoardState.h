#pragma once

#include "CoreMinimal.h"

#include "GSM_State.h"

class GSM_State_ProcessBoardState : public GSM_State
{
public:
	GSM_State_ProcessBoardState();

	virtual void Init(class ABladeIIGameMode* GameMode) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void End() override;

	virtual EGameState Type() const override { return EGameState::ProcessingBoardState; }

private:
	/**
	 * Check if the specified score grants a win under the current board state
	 * @param TargetScore - The score of the player for which we are checking whether they have won
	 * @param OppositePlayerScore - The score of the opposing player
	 * @param OppositePlayerHand - The cards currently in the hand slot of the opposing player
	 * @param OppositePlayerField - The cards currently in the field slot of the opposing player
	 * @returns true if won
	 */
	bool CheckIfTargetWon(uint32 TargetScore, uint32 OppositePlayerScore, TArray<ECard> OppositePlayerHand, TArray<ECard> OppositePlayerField) const;
};

