#include "B2Engine/GameStateMachine/GSM_State_ProcessBoardState.h"

#include "B2Utility/Log.h"
#include "B2Predicate/IsNotEffectCard.h"
#include "B2Predicate/HasCardOfHighEnoughValue.h"
#include "B2Predicate/MatchCardType.h"

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

	// Check if either local player or the opponent won after the most recent move
	bool bIsOtherPlayersTurn = GI->GetGameState()->Turn == EPlayer::Opponent;
	EWinCondition LocalPlayerOutcome = CheckIfTargetWon(PlayerScore, OpponentScore, PlayerHand, PlayerField, OpponentHand, OpponentField, OpponentDeckCount, bIsOtherPlayersTurn);
	if (LocalPlayerOutcome != EWinCondition::None)
	{
		GI->VictoryAchieved(EPlayer::Player, LocalPlayerOutcome);
		return;
	}

	bIsOtherPlayersTurn = GI->GetGameState()->Turn == EPlayer::Player;
	EWinCondition OpponentOutcome = CheckIfTargetWon(OpponentScore, PlayerScore, OpponentHand, OpponentField, PlayerHand, PlayerField, PlayerDeckCount, bIsOtherPlayersTurn);
	if (OpponentOutcome != EWinCondition::None)
	{
		GI->VictoryAchieved(EPlayer::Opponent, OpponentOutcome);
		return;
	}

	// Handle tied scores
	if (PlayerScore == OpponentScore)
	{
		GI->ClearAndDraw();
		return;
	}

	// If the turn is undecided, reaching this point means that we need to set the turn to the opposite player,
	// so that the following changeturn call switches over to the correct player
	if (GI->GetGameState()->Turn == EPlayer::Undecided)
	{
		GI->GetGameState()->Turn = PlayerScore > OpponentScore ? EPlayer::Player : EPlayer::Opponent;
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

EWinCondition GSM_State_ProcessBoardState::CheckIfTargetWon(uint32 TargetScore, uint32 OppositePlayerScore, const TArray<ECard>& TargetHand, const TArray<ECard>& TargetField, const TArray<ECard>& OppositePlayerHand, const TArray<ECard>& OppositePlayerField, uint32 OppositePlayerDeckCount, bool bIsOppositePlayersTurn) const
{
	// Logically this is kind of backwards but it works in my head

	// TODO perhaps we need to handle the case where a player ends up in an unwinnable position after the first draw. 
	// If this happens, we should probably just redraw instead.

	// Early exit if the opponent only has effect cards left, as this is an auto win regardless
	// Skips check if the opposite players hand is empty (as it means that the target probably has 1 more card to play
	if (OppositePlayerHand.Num() > 0 && !OppositePlayerHand.ContainsByPredicate<B2Predicate_IsNotEffectCard>(B2Predicate_IsNotEffectCard()))
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
		uint32 ScoreGap = TargetScore - OppositePlayerScore;

		// If its the other players turn and they failed to match or beat the targets score
		if (bIsOppositePlayersTurn)
		{
			return EWinCondition::ScoreVictory;
		}

		// If the other players hand is empty
		if (OppositePlayerHand.Num() == 0)
		{
			return EWinCondition::ScoreVictory;
		}

		// If there is no way for the other player to overcome the score difference, they lose

		// Do they have a regular card that would cause their score to match or beat the other score
		if (OppositePlayerHand.ContainsByPredicate<B2Predicate_HasCardOfHighEnoughValue>(B2Predicate_HasCardOfHighEnoughValue(ScoreGap)))
		{
			return EWinCondition::None;
		}

		// Does the other player have a rod card, which can be played, and would playing it save them?
		if (OppositePlayerHand.ContainsByPredicate<B2Predicate_MatchCardEnumType>(B2Predicate_MatchCardEnumType(ECard::ElliotsOrbalStaff)))
		{
			if (OppositePlayerField.Last() > ECard::Force)
			{
				if (OppositePlayerField.Last() == ECard::InactiveForce)
				{
					if (OppositePlayerScore * 2 >= TargetScore)
					{
						return EWinCondition::None;
					}
				}
				else if (GetBoltedCardRealValue(OppositePlayerField.Last()) >= ScoreGap)
				{
					return EWinCondition::None;
				}
			}
		}

		// Does the other player have a bolt card, which can be played, and would playing it save them?
		if (OppositePlayerHand.ContainsByPredicate<B2Predicate_MatchCardEnumType>(B2Predicate_MatchCardEnumType(ECard::Bolt)))
		{
			if (TargetField.Last() <= ECard::Force)
			{
				// Note - bolts are always valid to play as long as there is a card that can be bolted on the opposite field
				return EWinCondition::None;
			}
		}

		// Does the other player have a mirror card, which can be played, and would playing it save them?
		if (OppositePlayerHand.ContainsByPredicate<B2Predicate_MatchCardEnumType>(B2Predicate_MatchCardEnumType(ECard::Mirror)))
		{
			// Note - mirrors are always valid to play from a lower score position
			return EWinCondition::None;
		}

		// Does the other player have a blast card, which can be played, and would playing it save them?
		if (OppositePlayerHand.ContainsByPredicate<B2Predicate_MatchCardEnumType>(B2Predicate_MatchCardEnumType(ECard::Blast)))
		{
			// Note - blast cards are always valid to play from any position as the turn does not change, as long as the opposite
			// player has cards in their hand, which should always be the case if the game has not been ended on the previous turn
			return EWinCondition::None;
		}

		// Does the other player have a force card, and would playing it save them?
		if (OppositePlayerHand.ContainsByPredicate<B2Predicate_MatchCardEnumType>(B2Predicate_MatchCardEnumType(ECard::Force)))
		{
			if (OppositePlayerScore * 2 >= TargetScore)
			{
				return EWinCondition::None;
			}
		}

		return EWinCondition::ScoreVictory;
	}

	return EWinCondition::None;
}
