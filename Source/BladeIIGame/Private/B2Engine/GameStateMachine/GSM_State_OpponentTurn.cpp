#include "B2Engine/GameStateMachine/GSM_State_OpponentTurn.h"

#include "B2Utility/Log.h"

#include "B2GameMode/BladeIIGameMode.h"

GSM_State_OpponentTurn::GSM_State_OpponentTurn()
{
	GSM_State::GSM_State();
}

void GSM_State_OpponentTurn::Init(ABladeIIGameMode* GameMode)
{
	GSM_State::Init(GameMode);

	bStale = false;
}

void GSM_State_OpponentTurn::Tick(float DeltaSeconds)
{
	GSM_State::Tick(DeltaSeconds);

	if (bStale) return;

	ABladeIIGameMode* GI = GameModeInstance;

	FB2ServerUpdate MoveUpdate;
	if (GI->GetOpponent()->MoveUpdateQueue.Dequeue(MoveUpdate))
	{
		ECard Card = ServerUpdateToCard(MoveUpdate.Update);

		// Depending on the type of card and/or the board state, we either place the card on the field, or execute a special card
		// Here we also check for effects that occurred, so we can use them to branch later
		bool bUsedRodEffect = (Card == ECard::ElliotsOrbalStaff && GI->GetArena()->OpponentField->Num() > 0 && !GI->GetArena()->OpponentField->GetLast()->IsActive());
		bool bUsedBoltEffect = (Card == ECard::Bolt);
		bool bUsedMirrorEffect = (Card == ECard::Mirror);
		bool bUsedBlastEffect = (Card == ECard::Blast);
		bool bUsedForceEffect = (Card == ECard::Force);
		bool bUsedNormalCard = !bUsedRodEffect && !bUsedBoltEffect && !bUsedMirrorEffect && !bUsedBlastEffect && !bUsedForceEffect;

		// If the selected card was a normal card or a force card, and the opponents lastest field card is flipped, remove it
		ACard* LatestFieldCard = GI->GetArena()->OpponentField->GetLast();
		if (!LatestFieldCard->IsActive() && (bUsedForceEffect || bUsedNormalCard))
		{
			GI->GetDealer()->ClearSingleFromField(LatestFieldCard);
		}

		if (!bUsedNormalCard)
		{
			GI->GetDealer()->OpponentEffectCard(GI->GetArena()->OpponentHand->GetFirstOfType(Card));
		}
		else
		{
			// From opponent hand to opponent field
			UCardSlot* CurrentSlot = GI->GetArena()->OpponentHand;
			UCardSlot* TargetSlot = GI->GetArena()->OpponentField;

			int32 SourceSlotIndex = CurrentSlot->GetFirstIndexOfType(Card);
			if (SourceSlotIndex != -1)
			{
				GI->GetDealer()->Move(CurrentSlot, SourceSlotIndex, TargetSlot, ARC_ON_MOVE);
			}
			else
			{
				B2Utility::LogWarning(FString::Printf(TEXT("Received an invalid card from the opponent: [ %d ]"), Card));

				// Error recovery? Check the next one? End the game?
			}

			// Update the card positions in the hand as we have just removed one
			GI->GetDealer()->UpdateHandPositions(EPlayer::Opponent);
		}

		bStale = true;
	}
}

void GSM_State_OpponentTurn::End()
{
	GSM_State::End();


}
