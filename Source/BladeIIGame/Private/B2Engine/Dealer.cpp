#include "B2Engine/Dealer.h"

#include "Kismet/KismetMathLibrary.h"

#include "B2Misc/Utility.h"

const size_t DECK_CAPACITY = 15;
const size_t HAND_CAPACITY = 10;
const size_t FIELD_CAPACITY = 15;
const float CARD_HEIGHT = 0.03f;

B2Dealer::B2Dealer()
{
	bCardsDealt = false;

	B2Transition::ResetStatic();
}

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

	B2WaitGroup WG_IntoDeck = B2Transition::GetNextWaitGroup();

	// Player cards - into deck
	for (size_t i = 0; i < DECK_CAPACITY; i++)
	{
		float Delay = DelayOnStart + i * DelayOffsetOnStart;

		// Get a pointer to the topmost card from the players deck
		ACard* Card = Arena->PlayerDeck->GetCardByIndex(i);

		// Transition 1
		B2TPosition Position
		{
			Card->GetActorLocation(),
			Arena->PlayerDeck->GetTransformForIndex(i).Position,
			FVector(0, -2, 0),
			EEase::EaseInOut,
		};

		B2TRotation Rotation
		{
			Card->GetActorRotation(),
			Card->GetActorRotation(),
			EEase::EaseInOut,
		};

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(WG_IntoDeck, Position, Rotation, DelayOOBToDeck, Delay);
		Card->QueueTransition(Transition);
	}

	// Opponent cards - into deck
	for (size_t i = 0; i < DECK_CAPACITY; i++)
	{
		float Delay = DelayOnStart + i * DelayOffsetOnStart;

		// Get a pointer to the topmost card from the players deck
		ACard* Card = Arena->OpponentDeck->GetCardByIndex(i);

		// Transition 1
		B2TPosition Position
		{
			Card->GetActorLocation(),
			Arena->OpponentDeck->GetTransformForIndex(i).Position,
			FVector(0, 2, 0),
			EEase::EaseInOut,
		};

		B2TRotation Rotation
		{
			Card->GetActorRotation(),
			Card->GetActorRotation(),
			EEase::EaseInOut,
		};

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(WG_IntoDeck, Position, Rotation, DelayOOBToDeck, Delay);
		Card->QueueTransition(Transition);
	}

	B2WaitGroup WG_DeckToHand = B2Transition::GetNextWaitGroup();

	// Player cards - deck to hand
	for (size_t i = 0; i < HAND_CAPACITY; i++)
	{
		size_t DeckIndex = DECK_CAPACITY - 1 - i;
		size_t HandIndex = i;
		float Delay = i * (0.4f);

		// Pop the topmost card from the players deck
		ACard* Card = Arena->PlayerDeck->RemoveByIndex(DeckIndex);

		// Transition 1
		B2TPosition Position
		{
			Arena->PlayerDeck->GetTransformForIndex(DeckIndex).Position,
			Arena->PlayerDeck->GetTransformForIndex(DECK_CAPACITY - 1).Position + FVector(0, 0, 4.0),
			FVector(0, 0, 0),
			EEase::EaseInOut,
		};

		B2TRotation Rotation
		{
			Arena->PlayerDeck->GetTransformForIndex(DeckIndex).Rotation,
			Arena->PlayerDeck->GetTransformForIndex(DeckIndex).Rotation,
			EEase::EaseInOut,
		};

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(B2WaitGroupNone, Position, Rotation, DurationDeckToAir, Delay);
		Card->QueueTransition(Transition);

		// Transition 2
		Position = B2TPosition
		{
			Position.EndPosition,
			Arena->PlayerHand->GetTransformForIndex(HandIndex).Position,
			FVector(0, -2.0f, 12.f),
			EEase::EaseOut,
		};

		Rotation = B2TRotation
		{
			Rotation.EndRotation,
			Arena->PlayerHandReversed->GetTransformForIndex(HandIndex).Rotation,
			EEase::EaseOut,
		};

		// Add the transition to the transition queue
		Transition = B2Transition(WG_DeckToHand, Position, Rotation, DurationAirToHand, 0.0f);
		Card->QueueTransition(Transition);

		// Add the card that we popped from the players deck, to the players hand
		Arena->PlayerHand->Add(Card);
	}

	// Opponent cards - deck to hand
	for (size_t i = 0; i < HAND_CAPACITY; i++)
	{
		size_t DeckIndex = DECK_CAPACITY - 1 - i;
		size_t HandIndex = i;
		float Delay = i * (0.4f);

		// Pop the topmost card from the opponents deck
		ACard* Card = Arena->OpponentDeck->RemoveByIndex(DeckIndex);

		// Transition 1
		B2TPosition Position
		{
			Arena->OpponentDeck->GetTransformForIndex(DeckIndex).Position,
			Arena->OpponentDeck->GetTransformForIndex(DECK_CAPACITY - 1).Position + FVector(0, 0, 4.0),
			FVector(0, 0, 0),
			EEase::EaseInOut,
		};

		B2TRotation Rotation
		{
			Arena->OpponentDeck->GetTransformForIndex(DeckIndex).Rotation,
			Arena->OpponentDeck->GetTransformForIndex(DeckIndex).Rotation,
			EEase::EaseInOut,
		};

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(B2WaitGroupNone, Position, Rotation, DurationDeckToAir, Delay);
		Card->QueueTransition(Transition);

		// Transition 2
		Position = B2TPosition
		{
			Position.EndPosition,
			Arena->OpponentHand->GetTransformForIndex(HandIndex).Position,
			FVector(0, 2.0f, 12.f),
			EEase::EaseOut,
		};

		Rotation = B2TRotation
		{
			Rotation.EndRotation,
			Arena->OpponentHand->GetTransformForIndex(HandIndex).Rotation,
			EEase::EaseOut,
		};

		// Add the transition to the transition queue
		Transition = B2Transition(WG_DeckToHand, Position, Rotation, DurationAirToHand, 0.0f);
		Card->QueueTransition(Transition);

		// Add the card that we popped from the players deck, to the players hand
		Arena->OpponentHand->Add(Card);
	}

	B2WaitGroup WG_PlayerHandReveal = B2Transition::GetNextWaitGroup();

	// Player cards reveal
	for (size_t i = 0; i < Arena->PlayerHand->Size(); i++)
	{
		float Delay = i * 0.3f;

		// Get a pointer to the first card card from the players hand
		ACard* Card = Arena->PlayerHand->GetCardByIndex(i);

		// Transition 1
		B2TPosition Position
		{
			Arena->PlayerHandReversed->GetTransformForIndex(i).Position,
			Arena->PlayerHandReversed->GetTransformForIndex(i).Position,
			FVector(6.5f, 2.f, 5.f),
			EEase::EaseIn,
		};

		B2TRotation Rotation
		{
			Arena->PlayerHandReversed->GetTransformForIndex(i).Rotation,
			Arena->PlayerHand->GetTransformForIndex(i).Rotation,
			EEase::EaseIn,
		};

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(WG_PlayerHandReveal, Position, Rotation, DurationRevealCards, Delay);
		Card->QueueTransition(Transition);
	}

	B2WaitGroup WG_GatherUp = B2Transition::GetNextWaitGroup();

	// Player cards gather up
	for (size_t i = 0; i < Arena->PlayerHand->Size(); i++)
	{
		float Delay = 1.f;
		FVector CenterPosition = FMath::Lerp(Arena->PlayerHand->GetTransformForIndex(3).Position, Arena->PlayerHand->GetTransformForIndex(4).Position, 0.5f) + (i * FVector(0, 0, CARD_HEIGHT));
		FRotator CenterRotation = FMath::Lerp(Arena->PlayerHand->GetTransformForIndex(4).Rotation, Arena->PlayerHand->GetTransformForIndex(5).Rotation, 0.5f);
		
		// Get a pointer to the target card 
		ACard* Card = Arena->PlayerHand->GetCardByIndex(i);

		// Transition 1
		B2TPosition Position
		{
			Arena->PlayerHand->GetTransformForIndex(i).Position,
			CenterPosition,
			FVector(0, 0, 0),
			EEase::EaseInOut,
		};

		B2TRotation Rotation
		{
			Arena->PlayerHand->GetTransformForIndex(i).Rotation,
			CenterRotation,
			EEase::EaseInOut,
		};

		FVector StartPosition = Arena->PlayerHand->GetTransformForIndex(i).Position;
		FRotator StartRotation = Arena->PlayerHand->GetTransformForIndex(i).Rotation;
		FVector EndPosition = CenterPosition;
		FRotator EndRotation = CenterRotation;

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(WG_GatherUp, Position, Rotation, DurationGroupCards, Delay);
		Card->QueueTransition(Transition);
	}

	// Opponents cards gather up
	for (size_t i = 0; i < Arena->OpponentHand->Size(); i++)
	{
		float Delay = 1.f;
		FVector CenterPosition = FMath::Lerp(Arena->OpponentHand->GetTransformForIndex(3).Position, Arena->OpponentHand->GetTransformForIndex(4).Position, 0.5f) + (i * FVector(0, 0, CARD_HEIGHT));
		FRotator CenterRotation = FMath::Lerp(Arena->OpponentHand->GetTransformForIndex(4).Rotation, Arena->OpponentHand->GetTransformForIndex(5).Rotation, 0.5f);

		// Get a pointer to the target card 
		ACard* Card = Arena->OpponentHand->GetCardByIndex(i);

		// Transition 1
		B2TPosition Position
		{
			Arena->OpponentHand->GetTransformForIndex(i).Position,
			CenterPosition,
			FVector(0, 0, 0),
			EEase::EaseInOut,
		};

		B2TRotation Rotation
		{
			Arena->OpponentHand->GetTransformForIndex(i).Rotation,
			CenterRotation,
			EEase::EaseInOut,
		};

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(WG_GatherUp, Position, Rotation, DurationGroupCards, Delay);
		Card->QueueTransition(Transition);
	}

	B2WaitGroup WG_PlayerShuffle = B2Transition::GetNextWaitGroup();

	// Player cards shuffle
	for (size_t i = 0; i < Arena->PlayerHand->Size(); i++)
	{
		float Delay = 0.5f;

		// Get a pointer to the target card 
		ACard* Card = Arena->PlayerHand->GetCardByIndex(i);

		FVector CenterPosition = FMath::Lerp(Arena->PlayerHand->GetTransformForIndex(3).Position, Arena->PlayerHand->GetTransformForIndex(4).Position, 0.5f);
		CenterPosition += (i * FVector(0, 0, CARD_HEIGHT));
		FRotator CenterRotationUpwards = FMath::Lerp(Arena->PlayerHand->GetTransformForIndex(4).Rotation, Arena->PlayerHand->GetTransformForIndex(5).Rotation, 0.5f);

		// Transition 1
		B2TPosition Position
		{
			CenterPosition,
			CenterPosition + FVector(0, 0, 6.f),
			FVector(0, 0, 0),
			EEase::EaseInOut,
		};

		B2TRotation Rotation
		{
			CenterRotationUpwards,
			CenterRotationUpwards,
			EEase::EaseInOut,
		};

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(B2WaitGroupNone, Position, Rotation, 0.4f, Delay);
		Card->QueueTransition(Transition);

		// Transition 2
		FRotator CenterRotationDownwards = FMath::Lerp(Arena->PlayerHandReversed->GetTransformForIndex(4).Rotation, Arena->PlayerHandReversed->GetTransformForIndex(5).Rotation, 0.5f);

		FVector ArcOffset;
		float VerticalOffset = i > 4 ? CARD_HEIGHT + ((i - 5) * CARD_HEIGHT * 2) : -(CARD_HEIGHT + ((4 - i) * CARD_HEIGHT * 2));

		Position = B2TPosition
		{
			Position.EndPosition,
			Position.EndPosition - FVector(0, 0, VerticalOffset),
			FVector(0, 0, 0),
			EEase::EaseInOut,
		};

		Rotation = B2TRotation
		{
			CenterRotationUpwards,
			CenterRotationDownwards,
			EEase::EaseInOut,
		};

		// Add the transition to the transition queue
		Transition = B2Transition(B2WaitGroupNone, Position, Rotation, 0.4f, Delay);
		Card->QueueTransition(Transition);

		// Transition 3
		Position = B2TPosition
		{
			Position.EndPosition,
			CenterPosition + FVector(0, 0, 6.f),
			FVector(0, 0, 0),
			EEase::EaseInOut,
		};

		Rotation = B2TRotation
		{
			CenterRotationDownwards,
			CenterRotationUpwards,
			EEase::EaseInOut,
		};

		// Add the transition to the transition queue
		Transition = B2Transition(WG_PlayerShuffle, Position, Rotation, 0.4f, Delay);
		Card->QueueTransition(Transition);


		// TODO sort and rearrange for both hands
	}

	B2WaitGroup WG_PostShuffleSpread = B2Transition::GetNextWaitGroup();

	// Player cards spread
	for (size_t i = 0; i < Arena->PlayerHand->Size(); i++)
	{
		float Delay = 1.f;
		FVector CenterPosition = FMath::Lerp(Arena->PlayerHand->GetTransformForIndex(3).Position, Arena->PlayerHand->GetTransformForIndex(4).Position, 0.5f) + (i * FVector(0, 0, CARD_HEIGHT));
		FRotator CenterRotation = FMath::Lerp(Arena->PlayerHand->GetTransformForIndex(4).Rotation, Arena->PlayerHand->GetTransformForIndex(5).Rotation, 0.5f);

		// Get a pointer to the target card 
		ACard* Card = Arena->PlayerHand->GetCardByIndex(i);

		// Transition 1
		B2TPosition Position
		{
			CenterPosition + FVector(0, 0, 6.f),
			Arena->PlayerHand->GetTransformForIndex(i).Position,
			FVector(0, 0, 0),
			EEase::EaseInOut,
		};

		B2TRotation Rotation
		{
			CenterRotation,
			Arena->PlayerHand->GetTransformForIndex(i).Rotation,
			EEase::EaseInOut,
		};

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(WG_PostShuffleSpread, Position, Rotation, DurationGroupCards, Delay);
		Card->QueueTransition(Transition);
	}

	// Opponent cards spread
	for (size_t i = 0; i < Arena->OpponentHand->Size(); i++)
	{
		float Delay = 1.f;
		FVector CenterPosition = FMath::Lerp(Arena->OpponentHand->GetTransformForIndex(3).Position, Arena->OpponentHand->GetTransformForIndex(4).Position, 0.5f) + (i * FVector(0, 0, CARD_HEIGHT));
		FRotator CenterRotation = FMath::Lerp(Arena->OpponentHand->GetTransformForIndex(4).Rotation, Arena->OpponentHand->GetTransformForIndex(5).Rotation, 0.5f);

		// Get a pointer to the target card 
		ACard* Card = Arena->OpponentHand->GetCardByIndex(i);

		// Transition 1
		B2TPosition Position
		{
			CenterPosition + FVector(0, 0, 6.f),
			Arena->OpponentHand->GetTransformForIndex(i).Position,
			FVector(0, 0, 0),
			EEase::EaseInOut,
		};

		B2TRotation Rotation
		{
			CenterRotation,
			Arena->OpponentHand->GetTransformForIndex(i).Rotation,
			EEase::EaseInOut,
		};

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(WG_PostShuffleSpread, Position, Rotation, DurationGroupCards, Delay);
		Card->QueueTransition(Transition);
	}
}