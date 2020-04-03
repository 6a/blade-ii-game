#include "B2Engine/Dealer.h"

#include "Kismet/KismetMathLibrary.h"

#include "B2Misc/Utility.h"

const size_t DECK_CAPACITY = 15;
const size_t HAND_CAPACITY = 10;
const size_t FIELD_CAPACITY = 15;
const float CARD_HEIGHT = 0.03f;

void B2Dealer::Deal()
{
	if (bCardsDealt)
	{
		B2Utility::LogWarning("Cards have already been dealt - cannot deal again");
		return;
	}

	check(Arena);

	bCardsDealt = true;

	// Const settings
	float DelayOnStart = 1.5f;
	float DelayOffsetOnStart = 0.2f;
	float DelayOOBToDeck = 0.4f;
	float DelayPreDeal = 1.f;
	float DelayPostDeal = 7.5f;
	float DelayPostReveal = 3.f;
	float DelayPlayerPerCardShuffle = 0.25f;
	float DelayOpponentPerCardShuffle = 0.1f;
	float DelayPreOpponentShuffle = 0.7f;
	float SubtractDelayShowPlayerCards = 0.4f;
	float DurationDeckToAir = 0.4f;
	float DurationAirToHand = 0.3f;
	float DurationRevealCards = 0.3f;
	float DurationGroupCards = 0.5f;

	// Player cards - into deck
	for (size_t i = 0; i < DECK_CAPACITY; i++)
	{
		float Delay = DelayOnStart + i * DelayOffsetOnStart;

		// Get a pointer to the topmost card from the players deck
		ACard* Card = Arena->PlayerDeck->GetCardByIndex(i);

		FVector StartPosition = Card->GetActorLocation();
		FRotator StartRotation = Card->GetActorRotation();
		FVector EndPosition = Arena->PlayerDeck->GetTransformForIndex(i).Position;
		FRotator EndRotation = StartRotation;

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(StartPosition, EndPosition, StartRotation, EndRotation, FVector(0, -2, 0), EEase::EaseInOut, DelayOOBToDeck, Delay);
		Card->QueueTransition(Transition);
	}

	// Opponent cards - into deck
	for (size_t i = 0; i < DECK_CAPACITY; i++)
	{
		float Delay = DelayOnStart + i * DelayOffsetOnStart;

		// Get a pointer to the topmost card from the players deck
		ACard* Card = Arena->OpponentDeck->GetCardByIndex(i);

		FVector StartPosition = Card->GetActorLocation();
		FRotator StartRotation = Card->GetActorRotation();
		FVector EndPosition = Arena->OpponentDeck->GetTransformForIndex(i).Position;
		FRotator EndRotation = StartRotation;

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(StartPosition, EndPosition, StartRotation, EndRotation, FVector(0, 2, 0), EEase::EaseInOut, DelayOOBToDeck, Delay);
		Card->QueueTransition(Transition);
	}

	// Player cards - deck to hand
	for (size_t i = 0; i < HAND_CAPACITY; i++)
	{
		size_t DeckIndex = DECK_CAPACITY - 1 - i;
		size_t HandIndex = i;
		float Delay = DelayPreDeal + i * (DurationDeckToAir + DelayOOBToDeck);

		// Transition 1
		FVector StartPosition = Arena->PlayerDeck->GetTransformForIndex(DeckIndex).Position;
		FRotator StartRotation = Arena->PlayerDeck->GetTransformForIndex(DeckIndex).Rotation;
		FVector EndPosition = Arena->PlayerDeck->GetTransformForIndex(DECK_CAPACITY - 1).Position + FVector(0, 0, 4.0);
		FRotator EndRotation = StartRotation;

		// Pop the topmost card from the players deck
		ACard* Card = Arena->PlayerDeck->RemoveByIndex(DeckIndex);

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(StartPosition, EndPosition, StartRotation, EndRotation, FVector(0, 0, 0), EEase::EaseInOut, DurationDeckToAir, Delay);
		Card->QueueTransition(Transition);

		// Transition 2
		StartPosition = EndPosition;
		StartRotation = EndRotation;
		EndPosition = Arena->PlayerHand->GetTransformForIndex(HandIndex).Position;
		EndRotation = Arena->PlayerHandReversed->GetTransformForIndex(HandIndex).Rotation;

		// Add the transition to the transition queue
		Transition = B2Transition(StartPosition, EndPosition, StartRotation, EndRotation, FVector(0, -2.0, 12), EEase::EaseOut, DurationAirToHand, 0.0f);
		Card->QueueTransition(Transition);

		// Add the card that we popped from the players deck, to the players hand
		Arena->PlayerHand->Add(Card);
	}

	// Opponent cards - deck to hand
	for (size_t i = 0; i < HAND_CAPACITY; i++)
	{
		size_t DeckIndex = DECK_CAPACITY - 1 - i;
		size_t HandIndex = i;
		float Delay = DelayPreDeal + i * (DurationDeckToAir + DelayOOBToDeck);

		// Transition 1
		FVector StartPosition = Arena->OpponentDeck->GetTransformForIndex(DeckIndex).Position;
		FRotator StartRotation = Arena->OpponentDeck->GetTransformForIndex(DeckIndex).Rotation;
		FVector EndPosition = Arena->OpponentDeck->GetTransformForIndex(DECK_CAPACITY - 1).Position + FVector(0, 0, 4.0);
		FRotator EndRotation = StartRotation;

		// Pop the topmost card from the opponents deck
		ACard* Card = Arena->OpponentDeck->RemoveByIndex(DeckIndex);

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(StartPosition, EndPosition, StartRotation, EndRotation, FVector(0, 0, 0), EEase::EaseInOut, DurationDeckToAir, Delay);
		Card->QueueTransition(Transition);

		// Transition 2
		StartPosition = EndPosition;
		StartRotation = EndRotation;
		EndPosition = Arena->OpponentHand->GetTransformForIndex(HandIndex).Position;
		EndRotation = Arena->OpponentHand->GetTransformForIndex(HandIndex).Rotation;

		// Add the transition to the transition queue
		Transition = B2Transition(StartPosition, EndPosition, StartRotation, EndRotation, FVector(0, 2.0, 12), EEase::EaseOut, DurationAirToHand, 0.0f);
		Card->QueueTransition(Transition);

		// Add the card that we popped from the opponents deck, to the opponents hand
		Arena->OpponentHand->Add(Card);
	}

	// Player cards reveal
	for (size_t i = 0; i < Arena->PlayerHand->Size(); i++)
	{
		float Delay = DelayPostDeal - (i * SubtractDelayShowPlayerCards);

		// Transition 1
		FVector StartPosition = Arena->PlayerHandReversed->GetTransformForIndex(i).Position;
		FRotator StartRotation = Arena->PlayerHandReversed->GetTransformForIndex(i).Rotation;
		FVector EndPosition = StartPosition;
		FRotator EndRotation = Arena->PlayerHand->GetTransformForIndex(i).Rotation;

		// Get a pointer to the first card card from the players hand
		ACard* Card = Arena->PlayerHand->GetCardByIndex(i);

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(StartPosition, EndPosition, StartRotation, EndRotation, FVector(4.5f, 2.f, 5.f), EEase::EaseIn, DurationRevealCards, Delay);
		Card->QueueTransition(Transition);
	}

	// Player cards shuffle
	for (size_t i = 0; i < Arena->PlayerHand->Size(); i++)
	{
		float Delay = DelayPostReveal - (i * (DelayPlayerPerCardShuffle));
		FVector CenterPosition = FMath::Lerp(Arena->PlayerHand->GetTransformForIndex(4).Position, Arena->PlayerHand->GetTransformForIndex(5).Position, 0.5f) + (i * FVector(0, 0, CARD_HEIGHT));
		FRotator CenterRotation = FMath::Lerp(Arena->PlayerHand->GetTransformForIndex(4).Rotation, Arena->PlayerHand->GetTransformForIndex(5).Rotation, 0.5f);

		// Transition 1
		FVector StartPosition = Arena->PlayerHand->GetTransformForIndex(i).Position;
		FRotator StartRotation = Arena->PlayerHand->GetTransformForIndex(i).Rotation;
		FVector EndPosition = CenterPosition;
		FRotator EndRotation = CenterRotation;

		// Get a pointer to the target card 
		ACard* Card = Arena->PlayerHand->GetCardByIndex(i);

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(StartPosition, EndPosition, StartRotation, EndRotation, FVector(0, 0, 0), EEase::EaseInOut, DurationGroupCards, Delay);
		Card->QueueTransition(Transition);
	}

	// Opponents cards shuffle
	for (size_t i = 0; i < Arena->PlayerHand->Size(); i++)
	{
		float Delay = DelayPostDeal + DelayPreOpponentShuffle - (i * (SubtractDelayShowPlayerCards + DelayOpponentPerCardShuffle));
		FVector CenterPosition = FMath::Lerp(Arena->OpponentHand->GetTransformForIndex(4).Position, Arena->OpponentHand->GetTransformForIndex(5).Position, 0.5f);
		CenterPosition += (i * FVector(0, 0, CARD_HEIGHT));
		FRotator CenterRotation = FMath::Lerp(Arena->OpponentHand->GetTransformForIndex(4).Rotation, Arena->OpponentHand->GetTransformForIndex(5).Rotation, 0.5f);

		// Transition 1
		FVector StartPosition = Arena->OpponentHand->GetTransformForIndex(i).Position;
		FRotator StartRotation = Arena->OpponentHand->GetTransformForIndex(i).Rotation;
		FVector EndPosition = CenterPosition;
		FRotator EndRotation = CenterRotation;

		// Get a pointer to the target card 
		ACard* Card = Arena->OpponentHand->GetCardByIndex(i);

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(StartPosition, EndPosition, StartRotation, EndRotation, FVector(0, 0, 0), EEase::EaseInOut, DurationGroupCards, Delay);
		Card->QueueTransition(Transition);
	}

	// At this point we can assume that all the cards are relatively in sync


}
