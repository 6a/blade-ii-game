#include "B2Engine/GameStateMachine/GSM_State_ProcessBoardState.h"

#include "B2Utility/Log.h"
#include "B2Predicate/IsNotEffectCard.h"
#include "B2Predicate/HasCardOfHighEnoughValue.h"
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

	uint32 PlayerScore = GameState->PlayerScore;
	uint32 OpponentScore = GameState->OpponentScore;
	uint32 PlayerDeckCount = GameState->Cards.PlayerDeck.Num();
	uint32 OpponentDeckCount = GameState->Cards.OpponentDeck.Num();
	TArray<ECard> PlayerHand = GameState->Cards.PlayerHand;
	TArray<ECard> PlayerField = GameState->Cards.PlayerField;
	TArray<ECard> OpponentHand = GameState->Cards.OpponentHand;
	TArray<ECard> OpponentField = GameState->Cards.OpponentField;

	// Check for all the potential branches.
	// Early exit when a condition is met

	// Handle tied scores
	if (PlayerScore == OpponentScore)
	{
		GI->ClearAndDraw();
		return;
	}

	// Check if either local player or the opponent won after the most recent move
	bool bIsOtherPlayersTurn = GI->GetGameState()->Turn == EPlayer::Opponent;
	EWinCondition LocalPlayerOutcome = CheckIfTargetWon(PlayerScore, OpponentScore, OpponentHand, OpponentField, OpponentDeckCount, bIsOtherPlayersTurn);
	if (LocalPlayerOutcome != EWinCondition::None)
	{
		GI->VictoryAchieved(EPlayer::Player, LocalPlayerOutcome);
		return;
	}

	bIsOtherPlayersTurn = GI->GetGameState()->Turn == EPlayer::Player;
	EWinCondition OpponentOutcome = CheckIfTargetWon(OpponentScore, PlayerScore, PlayerHand, PlayerField, PlayerDeckCount, bIsOtherPlayersTurn);
	if (OpponentOutcome != EWinCondition::None)
	{
		GI->VictoryAchieved(EPlayer::Opponent, OpponentOutcome);
		return;
	}

	if (GI->GetGameState()->Turn == EPlayer::Undecided)
	{
		// If the turn is undecided, reaching this point means that we need to set the turn to the opposite player,
		// so that the following changeturn call switches over to the correct player
		if (GI->GetGameState()->Turn == EPlayer::Undecided)
		{
			GI->GetGameState()->Turn = PlayerScore > OpponentScore ? EPlayer::Player : EPlayer::Opponent;
		}
	}

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

EWinCondition GSM_State_ProcessBoardState::CheckIfTargetWon(uint32 TargetScore, uint32 OppositePlayerScore, const TArray<ECard>& OppositePlayerHand, const TArray<ECard>& OppositePlayerField, uint32 OppositePlayerDeckCount, bool bIsOppositePlayersTurn) const
{
	// Logically this is kind of backwards but it works in my head

	// TODO perhaps we need to handle the case where a player ends up in an unwinnable position after the first draw. 
	// If this happens, we should probably just redraw instead.

	// Early exit if the opponent only has effect cards left, as this is an auto win regardless
	if (!OppositePlayerHand.ContainsByPredicate<B2Predicate_IsNotEffectCard>(B2Predicate_IsNotEffectCard()))
	{
		return EWinCondition::OpponentOnlyHasEffectCards;
	}

	// Early exit if the score is tied and the opponent has no cards left in the deck or hand
	if (TargetScore == OppositePlayerScore)
	{
		if (OppositePlayerDeckCount + OppositePlayerHand.Num() == 0)
		{
			return EWinCondition::ScoreVictory;
		}
	}

	if (TargetScore > OppositePlayerScore)
	{
		// If its the other players turn and they failed to match or beat the targets score
		if (bIsOppositePlayersTurn)
		{
			return EWinCondition::ScoreVictory;
		}

		// If the opponents hand is empty
		if (OppositePlayerHand.Num() == 0)
		{
			return EWinCondition::ScoreVictory;
		}

		// If the difference is bigger than any card could cover...
		uint32 ScoreGap = TargetScore - OppositePlayerScore;
		if (ScoreGap > ACard::MAX_CARD_SCORE)
		{
			// And there is no force card that can be played
			if (!OppositePlayerHand.FindByPredicate<B2Predicate_IsForce>(B2Predicate_IsForce()))
			{
				return EWinCondition::ScoreVictory;
			}
			// Else if there is a force card, but the score would still not be high enough if the force card was used
			else if (TargetScore > OppositePlayerScore * 2)
			{
				return EWinCondition::ScoreVictory;
			}
		}

		// If the difference is not too big to cover by a normal card, but the opponents highest value card is not high valued enough
		// This works because rods are worth 1, and flipped cards are worth 0
		if (!OppositePlayerHand.FindByPredicate<B2Predicate_HasCardOfHighEnoughValue>(B2Predicate_HasCardOfHighEnoughValue(ScoreGap)))
		{
			// And they have no rods to play
			if (!OppositePlayerHand.FindByPredicate<B2Predicate_IsRod>(B2Predicate_IsRod()))
			{
				return EWinCondition::ScoreVictory;
			}
			// else they have some rod cards, but the last card on their field is not inactive
			else if (OppositePlayerField.Last() <= ECard::Force)
			{
				return EWinCondition::ScoreVictory;
			}
			// else they have rod cards, and can play them, but the opponent is not able to bump their score up past the required value by using it
			else if (ACard::TypeToValue(OppositePlayerField.Last()) < ScoreGap)
			{
				return EWinCondition::ScoreVictory;
			}
		}
	}

	return EWinCondition::None;
}
