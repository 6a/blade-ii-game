#pragma once

#include "CoreMinimal.h"

#include "B2Enum/WinConditionEnum.h"

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

	/* The amount of time to wait, when the score is tied, before reporting back to the game mode - gives time for the opponent callout + animation */
	const float DELAY_ON_REPORT_TIED_SCORE = 3.f;

	/**
	 * Check if the specified score grants a win under the current board state
	 * @param TargetScore - The score of the player for which we are checking whether they have won
	 * @param OppositePlayerScore - The score of the opposing player
	 * @param TargetHand - The cards currently in the hand slot of the target player
	 * @param TargetField - The cards currently in the field slot of the target player
	 * @param OppositePlayerHand - The cards currently in the hand slot of the opposing player
	 * @param OppositePlayerField - The cards currently in the field slot of the opposing player
	 * @param OppositePlayerDeckCount - The number of cards in the deck slot of the opposing player
	 * @returns Either the win condition achieved, or EWinCondition::None if the target did not win
	 */
	EWinCondition CheckIfTargetWon(uint32 TargetScore, uint32 OppositePlayerScore, const TArray<ECard>& TargetHand, const TArray<ECard>& TargetField, const TArray<ECard>& OppositePlayerHand, const TArray<ECard>& OppositePlayerField, uint32 OppositePlayerDeckCount, bool bIsOppositePlayersTurn) const;
};

