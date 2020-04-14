#include "B2Engine/GameStateMachine/GSM_State_ProcessBoardState.h"

#include "B2Utility/Log.h"
#include "B2Predicate/IsNotEffectCard.h"
#include "B2Predicate/HasCardOfHighEnoughValue.h"
#include "B2Predicate/HasInactiveCard.h"
#include "B2Predicate/IsForce.h"
#include "B2Predicate/IsRod.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_ProcessBoardState::GSM_State_ProcessBoardState()
{
	GSM_State::GSM_State();
}

void GSM_State_ProcessBoardState::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	ABladeIIGameMode* GI = GameModeInstance;
	AArena* Arena = GI->GetArena();
	B2GameState* GameState = GI->GetGameState();

	// Check for all the potential branches.
	// Early exit when a condition is met

	uint32 PlayerScore = GameState->PlayerScore;
	uint32 OpponentScore = GameState->OpponentScore;
	TArray<ACard*> PlayerHand = Arena->PlayerHand->GetAll();
	TArray<ACard*> PlayerField = Arena->PlayerField->GetAll();
	TArray<ACard*> OpponentHand = Arena->OpponentHand->GetAll();
	TArray<ACard*> OpponentField = Arena->OpponentField->GetAll();

	// Check if either local player or the opponent won after the most recent move
	if (CheckIfTargetWon(PlayerScore, OpponentScore, OpponentHand, OpponentField))
	{
		// The player won
	}
	else if (CheckIfTargetWon(OpponentScore, PlayerScore, PlayerHand, PlayerField))
	{
		// The opponent won
	}

	// Handle tied scores

	// Otherwise, the board is still in a playable state so just switch the turn
	GI->ChangeTurn();
}

void GSM_State_ProcessBoardState::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);
}

void GSM_State_ProcessBoardState::End()
{
	GSM_State::End();


}

bool GSM_State_ProcessBoardState::CheckIfTargetWon(uint32 TargetScore, uint32 OppositePlayerScore, const TArray<ACard*> OppositePlayerHand, const TArray<ACard*> OppositePlayerField) const
{
	// Logically this is kind of backwards but it works in my head

	// Early exit if the opponent only has effect cards left, as this is an auto loss regardless
	if (!OppositePlayerHand.ContainsByPredicate<B2Predicate_IsNotEffectCard>(B2Predicate_IsNotEffectCard()))
	{
		return true;
	}

	if (TargetScore > OppositePlayerScore)
	{
		// If the opponents hand is empty
		if (OppositePlayerHand.Num() == 0)
		{
			return true;
		}

		// If the difference is bigger than any card could cover...
		uint32 ScoreGap = TargetScore - OppositePlayerScore;
		if (ScoreGap > 7)
		{
			// And there is no force card that can be played
			if (!OppositePlayerHand.FindByPredicate<B2Predicate_IsForce>(B2Predicate_IsForce()))
			{
				return true;
			}
			// Else if there is a force card, but the score would still not be high enough if the force card was used
			else if (TargetScore > OppositePlayerScore * 2)
			{
				return true;
			}
		}

		// If the difference is not too big to cover by a normal card, but the opponents highest value card is not high valued enough
		// This works because rods are worth 1 as well
		if (!OppositePlayerHand.FindByPredicate<B2Predicate_HasCardOfHighEnoughValue>(B2Predicate_HasCardOfHighEnoughValue(ScoreGap)))
		{
			// And they have no rods to play
			if (!OppositePlayerHand.FindByPredicate<B2Predicate_IsRod>(B2Predicate_IsRod()))
			{
				return true;
			}
			// else they have some rod cards, but they do not have any face down cards on their field to resurrect
			else if (!OppositePlayerField.FindByPredicate<B2Predicate_HasInactiveCard>(B2Predicate_HasInactiveCard()))
			{
				return true;
			}
			// else they have rod cards, and can play them, but the opponent is not able to bump their score up past the required value by using it
			else if (ACard::TypeToValue(OppositePlayerField.Last()->Type) < ScoreGap)
			{
				return true;
			}
		}
	}

	return false;
}
