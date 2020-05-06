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

	// Early exit check for draw condition
	if (GameDrawn(Arena, PlayerScore, OpponentScore))
	{
		GI->EndGame(EPlayer::Undecided, EWinCondition());
		return;
	}

	// Check if either local player or the opponent won after the most recent move
	bool bIsOtherPlayersTurn = GI->GetGameState()->Turn == EPlayer::Opponent;
	EWinCondition LocalPlayerOutcome = CheckIfTargetWon(PlayerScore, OpponentScore, PlayerField, OpponentHand, OpponentField, OpponentDeckCount, bIsOtherPlayersTurn);
	if (LocalPlayerOutcome != EWinCondition::None)
	{
		GI->EndGame(EPlayer::Player, LocalPlayerOutcome);
		return;
	}

	bIsOtherPlayersTurn = GI->GetGameState()->Turn == EPlayer::Player;
	EWinCondition OpponentOutcome = CheckIfTargetWon(OpponentScore, PlayerScore, OpponentField, PlayerHand, PlayerField, PlayerDeckCount, bIsOtherPlayersTurn);
	if (OpponentOutcome != EWinCondition::None)
	{
		GI->EndGame(EPlayer::Opponent, OpponentOutcome);
		return;
	}

	// If its the blast state (and we didnt find a winner/loser) just enter the same players turn again
	if (GameState->bHandleBlastEdgeCase)
	{
		GameState->bHandleBlastEdgeCase = false;

		GI->ContinueTurn();
		return;
	}

	// Handle tied scores
	if (PlayerScore == OpponentScore)
	{
		float Delay = 0;

		// Set the opponent message to matched if the field has cards on it (if it doesnt, it means we have already set it)
		if (GI->GetArena()->PlayerField->Num() + GI->GetArena()->OpponentField->Num() > 0)
		{
			GI->GetUIAvatarLayer()->SetOpponentMessage(EOpponentMessage::MatchedScores, GI->GetOpponentAvatar()->GetCurrentCharacterName());
			GI->GetOpponentAvatar()->AnimateMouth();

			Delay = DELAY_ON_REPORT_TIED_SCORE;
		}

		GI->ClearAndDraw(Delay);
		return;
	}

	// If the turn is undecided, reaching this point means that we need to set the turn to the opposite player,
	// so that the following changeturn call switches over to the correct player
	if (GI->GetGameState()->Turn == EPlayer::Undecided)
	{
		GI->GetGameState()->Turn = PlayerScore > OpponentScore ? EPlayer::Player : EPlayer::Opponent;

		if (GI->GetGameState()->Turn == EPlayer::Player)
		{
			GI->GetUIAvatarLayer()->SetOpponentMessage(EOpponentMessage::OpponentGoesFirst, GI->GetOpponentAvatar()->GetCurrentCharacterName());
		}
		else
		{
			GI->GetUIAvatarLayer()->SetOpponentMessage(EOpponentMessage::PlayerGoesFirst, GI->GetOpponentAvatar()->GetCurrentCharacterName());
		}

		GI->GetOpponentAvatar()->AnimateMouth();
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

EWinCondition GSM_State_ProcessBoardState::CheckIfTargetWon(uint32 TargetScore, uint32 OppositePlayerScore, const TArray<ECard>& TargetField, const TArray<ECard>& OppositePlayerHand, const TArray<ECard>& OppositePlayerField, uint32 OppositePlayerDeckCount, bool bIsOppositePlayersTurn) const
{
	ABladeIIGameMode* GI = GameModeInstance;

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

	// Exit early if a blast card was used, which ended in the opposite players hand becoming empty
	if (GI->GetGameState()->bHandleBlastEdgeCase)
	{
		if (OppositePlayerHand.Num() == 0)
		{
			return EWinCondition::ScoreVictory;
		}
	}

	if (TargetScore > OppositePlayerScore)
	{
		uint32 ScoreGap = TargetScore - OppositePlayerScore;

		// If its the other players turn and they failed to match or beat the targets score
		if (bIsOppositePlayersTurn && !GI->GetGameState()->bHandleBlastEdgeCase)
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
			if (OppositePlayerField.Num() > 0 && OppositePlayerField.Last() > ECard::Force)
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
			if (TargetField.Num() > 0 && TargetField.Last() <= ECard::Force)
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

bool GSM_State_ProcessBoardState::GameDrawn(AArena* Arena, uint32 LocalPlayerScore, uint32 OpponentScore) const
{
	ABladeIIGameMode* GI = GameModeInstance;

	// All cards expired, with equal scores
	if (Arena->PlayerDeck->Num() + Arena->OpponentDeck->Num() == 0)
	{
		if (Arena->PlayerHand->Num() + Arena->OpponentHand->Num() == 0)
		{
			if (LocalPlayerScore == OpponentScore)
			{
				return true;
			}
		}
	}

	return false;
}
