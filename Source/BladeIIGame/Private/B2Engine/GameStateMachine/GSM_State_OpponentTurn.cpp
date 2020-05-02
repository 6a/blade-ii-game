#include "B2Engine/GameStateMachine/GSM_State_OpponentTurn.h"

#include "Misc/DefaultValueHelper.h"

#include "B2Utility/Log.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_OpponentTurn::GSM_State_OpponentTurn()
{
	GSM_State::GSM_State();
}

void GSM_State_OpponentTurn::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	ABladeIIGameMode* GI = GameModeInstance;

	bMoveHandled = false;
	bMovedReceived = false;

	CachedMove = FB2ServerUpdate{ EServerUpdate::None };

	MoveExecutionTime = TNumericLimits<float>::Max();

	GI->GetUIStatusIndicatorLayer()->SetState(UStatusIndicator::State::OpponentTurn);
}

void GSM_State_OpponentTurn::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	ABladeIIGameMode* GI = GameModeInstance;

	if (!bMovedReceived)
	{
		FB2ServerUpdate MoveUpdate;
		if (GI->GetOpponent()->MoveUpdateQueue.Dequeue(MoveUpdate))
		{
			CachedMove = MoveUpdate;

			bMovedReceived = true;

			// If the opponent is an AI opponent, we add an artificial delay
			if (GI->GetSettings()->IsVersusAI())
			{
				MoveExecutionTime = GI->GetWorld()->GetTimeSeconds() + FMath::FRandRange(AI_DELAY_MIN, AI_DELAY_MAX);
			}
			else
			{
				MoveExecutionTime = 0;
			}
		}
	}
	else if (!bMoveHandled && GI->GetWorld()->GetTimeSeconds() > MoveExecutionTime)
	{
		ECard Card = ServerUpdateToCard(CachedMove.Code);

		// Reset eyes
		GI->GetOpponentAvatar()->RevertEyes();

		// Depending on the type of card and/or the board state, we either place the card on the field, or execute a special card
		// Here we also check for effects that occurred, so we can use them to branch later
		bool bUsedRodEffect = (Card == ECard::ElliotsOrbalStaff && GI->GetArena()->OpponentField->Num() > 0 && !GI->GetArena()->OpponentField->GetLast()->IsActive());
		bool bUsedBoltEffect = (Card == ECard::Bolt && GI->GetArena()->PlayerField->Num() > 0 && GI->GetArena()->PlayerField->GetLast()->IsActive());
		bool bUsedMirrorEffect = (Card == ECard::Mirror && GI->GetArena()->PlayerField->Num() > 0 && GI->GetArena()->OpponentField->Num() > 0);
		bool bUsedBlastEffect = (Card == ECard::Blast && GI->GetArena()->PlayerHand->Num() > 0);
		bool bUsedForceEffect = (Card == ECard::Force);
		bool bUsedNormalCard = !bUsedRodEffect && !bUsedBoltEffect && !bUsedMirrorEffect && !bUsedBlastEffect && !bUsedForceEffect;

		// If the selected card was a normal card or a force card, and the opponents lastest field card is flipped, remove it
		ACard* LatestFieldCard = GI->GetArena()->OpponentField->GetLast();
		if (LatestFieldCard && !LatestFieldCard->IsActive() && (bUsedForceEffect || bUsedNormalCard))
		{
			GI->GetDealer()->ClearSingleFromField(LatestFieldCard);
		}

		int32 SourceSlotIndex = GI->GetArena()->OpponentHand->GetFirstIndexOfType(Card);
		if (SourceSlotIndex != -1)
		{
			GI->GetGameState()->CursorPosition = ECardSlot::OpponentHand;
			GI->GetGameState()->CursorSlotIndex = SourceSlotIndex;

			if (!bUsedNormalCard)
			{
				// Edge case to capture the blasted card from the opponent
				if (bUsedBlastEffect)
				{
					int32 OutInt;
					FDefaultValueHelper::ParseInt(CachedMove.Payload, OutInt);

					if (OutInt != -1)
					{
						GI->GetGameState()->MostRecentBlastedCard = static_cast<ECard>(OutInt);
					}
					else
					{
						B2Utility::LogWarning(FString::Printf(TEXT("Unable to parse the blast metadata from the opponent: [ %s ]"), *CachedMove.Payload));
					}
				}

				GI->GetDealer()->OpponentEffectCard(GI->GetArena()->OpponentHand->GetFirstOfType(Card));
			}
			else
			{
				// From opponent hand to opponent field
				GI->GetDealer()->Move(GI->GetArena()->OpponentHand, SourceSlotIndex, GI->GetArena()->OpponentField, ARC_ON_MOVE);

				// Update the card positions in the hand as we have just removed one
				GI->GetDealer()->UpdateHandPositions(EPlayer::Opponent);
			}
		}
		else
		{
			B2Utility::LogWarning(FString::Printf(TEXT("Received an invalid card from the opponent: [ %d ]"), Card));

			// Error recovery? Check the next one? End the game?
		}

		bMoveHandled = true;

		// Update the status indicator if this wasnt a blast card
		if (!bUsedBlastEffect)
		{
			GI->GetUIStatusIndicatorLayer()->SetState(UStatusIndicator::State::Waiting);
		}
	}
}

void GSM_State_OpponentTurn::End()
{
	GSM_State::End();


}
