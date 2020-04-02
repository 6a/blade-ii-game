#include "B2Engine/Dealer.h"

#include "B2Misc/Utility.h"

const size_t DECK_CAPACITY = 15;
const size_t HAND_CAPACITY = 10;
const size_t FIELD_CAPACITY = 15;

void B2Dealer::Deal()
{
	if (bCardsDealt)
	{
		B2Utility::LogWarning("Cards have already been dealt - cannot deal again");
		return;
	}

	check(Arena);

	// Player cards
	for (size_t i = 0; i < 10; i++)
	{
		size_t DeckIndex = DECK_CAPACITY - 1 - i;
		size_t HandIndex = i;
		float Delay = 1.5f + i * 0.5f;

		FVector StartPosition = Arena->PlayerDeck->GetTransformForIndex(DeckIndex).Position;
		FRotator StartRotation = Arena->PlayerDeck->GetTransformForIndex(DeckIndex).Rotation;
		FVector EndPosition = Arena->PlayerDeck->GetTransformForIndex(DECK_CAPACITY - 1).Position + FVector(0, 0, 4.0);
		FRotator EndRotation = StartRotation;

		ACard* Card = Arena->PlayerDeck->RemoveByIndex(DeckIndex);

		B2Transition Transition = B2Transition(StartPosition, EndPosition, StartRotation, EndRotation, FVector(0, 0, 0), EEase::EaseInOut, 0.4f, Delay);
		Card->QueueTransition(Transition);

		StartPosition = EndPosition;
		StartRotation = EndRotation;
		EndPosition = Arena->PlayerHand->GetTransformForIndex(HandIndex).Position;
		EndRotation = Arena->PlayerHand->GetTransformForIndex(HandIndex).Rotation;

		Transition = B2Transition(StartPosition, EndPosition, StartRotation, EndRotation, FVector(0, -2.0, 12), EEase::EaseOut, 0.4f, 0.0f);
		Card->QueueTransition(Transition);

		Arena->PlayerHand->Add(Card);
	}

	// Opponent cards
	for (size_t i = 0; i < 10; i++)
	{
		size_t DeckIndex = DECK_CAPACITY - 1 - i;
		size_t HandIndex = i;
		float Delay = 1.5f + i * 0.5f;

		FVector StartPosition = Arena->OpponentDeck->GetTransformForIndex(DeckIndex).Position;
		FRotator StartRotation = Arena->OpponentDeck->GetTransformForIndex(DeckIndex).Rotation;
		FVector EndPosition = Arena->OpponentDeck->GetTransformForIndex(DECK_CAPACITY - 1).Position + FVector(0, 0, 4.0);
		FRotator EndRotation = StartRotation;

		ACard* Card = Arena->OpponentDeck->RemoveByIndex(DeckIndex);

		B2Transition Transition = B2Transition(StartPosition, EndPosition, StartRotation, EndRotation, FVector(0, 0, 0), EEase::EaseInOut, 0.4f, Delay);
		Card->QueueTransition(Transition);

		StartPosition = EndPosition;
		StartRotation = EndRotation;
		EndPosition = Arena->OpponentHand->GetTransformForIndex(HandIndex).Position;
		EndRotation = Arena->OpponentHand->GetTransformForIndex(HandIndex).Rotation;

		Transition = B2Transition(StartPosition, EndPosition, StartRotation, EndRotation, FVector(0, 2.0, 12), EEase::EaseOut, 0.4f, 0.0f);
		Card->QueueTransition(Transition);

		Arena->OpponentHand->Add(Card);
	}

	bCardsDealt = true;
}
