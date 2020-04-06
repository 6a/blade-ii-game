#include "B2Engine/Dealer.h"

#include "Kismet/KismetMathLibrary.h"

#include "B2Misc/Utility.h"

// Class scope values
const size_t DECK_CAPACITY = 15;
const size_t HAND_CAPACITY = 10;
const size_t FIELD_CAPACITY = 15;
const float CARD_STACKING_OFFSET = 0.09f;

UB2Dealer::UB2Dealer()
{
	bCardsDealt = false;

	// Set all event-use waitgroups to none;
	WaitGroupDealFinished = B2WaitGroupNone;

	B2Transition::ResetStatic();
}

void UB2Dealer::Deal()
{
	if (bCardsDealt)
	{
		B2Utility::LogWarning("Cards have already been dealt - cannot deal again");
		return;
	}

	check(Arena);

	bCardsDealt = true;

	// Animation Segment 1
	// FROM: Out of bounds
	// TO: Deck

	const float DelayOnStart = 1.5f;
	const float DurationIntoDeck = 0.4f;
	const float OffsetOnStart = 0.2f;
	B2WaitGroup WG_IntoDeck = B2Transition::GetNextWaitGroup();

	// Player cards - into deck
	for (size_t i = 0; i < DECK_CAPACITY; i++)
	{
		float Delay = DelayOnStart + i * OffsetOnStart;

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
		B2Transition Transition = B2Transition(WG_IntoDeck, Position, Rotation, DurationIntoDeck, Delay);
		Card->QueueTransition(Transition);
	}

	// Opponent cards - into deck
	for (size_t i = 0; i < DECK_CAPACITY; i++)
	{
		float Delay = DelayOnStart + i * OffsetOnStart;

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
		B2Transition Transition = B2Transition(WG_IntoDeck, Position, Rotation, DurationIntoDeck, Delay);
		Card->QueueTransition(Transition);
	}

	// Animation Segment 2
	// FROM: Deck
	// TO: Hand

	const float DurationDeckToAir = 0.4f;
	const float DurationAirToHand = 0.3f;
	const float OffsetDeckToHand = 0.4f; 
	B2WaitGroup WG_DeckToHand = B2Transition::GetNextWaitGroup();

	// Player cards - deck to hand
	for (size_t i = 0; i < HAND_CAPACITY; i++)
	{
		size_t DeckIndex = DECK_CAPACITY - 1 - i;
		size_t HandIndex = i;
		float Delay = i * OffsetDeckToHand;

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
		Transition = B2Transition(WG_DeckToHand, Position, Rotation, DurationDeckToAir, 0.0f);
		Card->QueueTransition(Transition);

		// Add the card that we popped from the players deck, to the players hand
		Arena->PlayerHand->Add(Card);
	}

	// Opponent cards - deck to hand
	for (size_t i = 0; i < HAND_CAPACITY; i++)
	{
		size_t DeckIndex = DECK_CAPACITY - 1 - i;
		size_t HandIndex = i;
		float Delay = i * OffsetDeckToHand;

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

	// Animation Segment 3
	// FROM: Player Hand (Hidden)
	// TO: Player Hand (Revealed)

	const float DurationPlayerHandReveal = 0.3f;
	const float OffsetPlayerHandReveal = 0.3f;
	B2WaitGroup WG_PlayerHandReveal = B2Transition::GetNextWaitGroup();

	// Player cards reveal
	for (size_t i = 0; i < Arena->PlayerHand->Count(); i++)
	{
		float Delay = i * OffsetPlayerHandReveal;

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
		B2Transition Transition = B2Transition(WG_PlayerHandReveal, Position, Rotation, DurationPlayerHandReveal, Delay);
		Card->QueueTransition(Transition);
	}

	// Animation Segment 4
	// FROM: Hand (Spread)
	// TO: Hand (Gathered)

	const float DelayPreShuffleGatherUp = 1.f;
	const float DurationPreShuffleGatherUp = 0.5f;
	B2WaitGroup WG_GatherUp = B2Transition::GetNextWaitGroup();

	// Player cards gather up
	for (size_t i = 0; i < Arena->PlayerHand->Count(); i++)
	{
		float Delay = DelayPreShuffleGatherUp;
		FVector CenterPosition = FMath::Lerp(Arena->PlayerHand->GetTransformForIndex(3).Position, Arena->PlayerHand->GetTransformForIndex(4).Position, 0.5f) + (i * FVector(0, 0, CARD_STACKING_OFFSET));
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
		B2Transition Transition = B2Transition(WG_GatherUp, Position, Rotation, DurationPreShuffleGatherUp, Delay);
		Card->QueueTransition(Transition);
	}

	// Opponents cards gather up
	for (size_t i = 0; i < Arena->OpponentHand->Count(); i++)
	{
		float Delay = DelayPreShuffleGatherUp;
		FVector CenterPosition = FMath::Lerp(Arena->OpponentHand->GetTransformForIndex(3).Position, Arena->OpponentHand->GetTransformForIndex(4).Position, 0.5f) + (i * FVector(0, 0, CARD_STACKING_OFFSET));
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
		B2Transition Transition = B2Transition(WG_GatherUp, Position, Rotation, DurationPreShuffleGatherUp, Delay);
		Card->QueueTransition(Transition);
	}

	// Animation Segment 5
	// FROM: Player Hand (Unsorted)
	// TO: Player Hand (Visually Sorted)

	const float DelayShuffleSteps = 1.f;
	const float OffsetShuffleSpread = 0.06f;
	const float DurationShuffleSteps = 0.4f;
	const float DurationShuffleSpread = OffsetShuffleSpread * HAND_CAPACITY;

	B2WaitGroup WG_Shuffle = B2Transition::GetNextWaitGroup();
	B2WaitGroup WG_PostShuffleSpread = B2Transition::GetNextWaitGroup();

	// Set the waitgroup for dealing
	WaitGroupDealFinished = WG_PostShuffleSpread + 1;

	// Correct order for players hand
	TArray<FString> SortedPlayerHand = Arena->PlayerHand->GetSortedIDsDescending();

	// Player cards shuffle
	for (size_t i = 0; i < Arena->PlayerHand->Count(); i++)
	{
		float Delay = DelayShuffleSteps;

		// Get a pointer to the target card 
		ACard* Card = Arena->PlayerHand->GetCardByIndex(i);

		FVector CenterPosition = FMath::Lerp(Arena->PlayerHand->GetTransformForIndex(3).Position, Arena->PlayerHand->GetTransformForIndex(4).Position, 0.5f);
		CenterPosition += (i * FVector(0, 0, CARD_STACKING_OFFSET));
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
		B2Transition Transition = B2Transition(B2WaitGroupNone, Position, Rotation, DurationShuffleSteps, Delay);
		Card->QueueTransition(Transition);

		// Transition 2
		FRotator CenterRotationDownwards = FMath::Lerp(Arena->PlayerHandReversed->GetTransformForIndex(4).Rotation, Arena->PlayerHandReversed->GetTransformForIndex(5).Rotation, 0.5f);

		FVector ArcOffset;
		float VerticalOffset = i > 4 ? CARD_STACKING_OFFSET + ((i - 5) * CARD_STACKING_OFFSET * 2) : -(CARD_STACKING_OFFSET + ((4 - i) * CARD_STACKING_OFFSET * 2));

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
		Transition = B2Transition(B2WaitGroupNone, Position, Rotation, DurationShuffleSteps, Delay);
		Card->QueueTransition(Transition);

		// Transition 3 (Sort)
		int IndexAfterSort = SortedPlayerHand.IndexOfByPredicate([Card](const FString& s) { return s == Card->GetID(); });

		CenterPosition = FMath::Lerp(Arena->PlayerHand->GetTransformForIndex(3).Position, Arena->PlayerHand->GetTransformForIndex(4).Position, 0.5f);
		CenterPosition += FVector(0, 0, 6.f) + (IndexAfterSort * FVector(0, 0, CARD_STACKING_OFFSET));

		Position = B2TPosition
		{
			Position.EndPosition,
			CenterPosition,
			FVector(0, 0, 0),
			EEase::EaseInOut,
		};

		// Rotation remains unchanged

		// Add the transition to the transition queue
		Transition = B2Transition(B2WaitGroupNone, Position, Rotation, 0.f, 0.f);
		Card->QueueTransition(Transition);

		// Transition 4
		CenterPosition = Position.EndPosition;

		VerticalOffset = IndexAfterSort > 4 ? CARD_STACKING_OFFSET + ((IndexAfterSort - 5) * CARD_STACKING_OFFSET * 2) : -(CARD_STACKING_OFFSET + ((4 - IndexAfterSort) * CARD_STACKING_OFFSET * 2));

		Position = B2TPosition
		{
			Position.EndPosition,
			CenterPosition + FVector(0, 0, VerticalOffset),
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
		Transition = B2Transition(WG_Shuffle, Position, Rotation, DurationShuffleSteps, Delay);
		Card->QueueTransition(Transition);

		// Post shuffle spread - included in this for loop so we can reuse the sorted cards

		// Transition 6
		FVector SpreadStart = Arena->PlayerHand->GetTransformForIndex(0).Position;
		SpreadStart.Z += IndexAfterSort * CARD_STACKING_OFFSET;

		Position = B2TPosition
		{
			Position.EndPosition,
			SpreadStart,
			FVector(0, 0, 0),
			EEase::EaseIn,
		};

		Rotation = B2TRotation
		{
			Rotation.EndRotation,
			Arena->PlayerHand->GetTransformForIndex(0).Rotation,
			EEase::EaseIn,
		};

		// Add the transition to the transition queue
		Transition = B2Transition(B2WaitGroupNone, Position, Rotation, DurationShuffleSteps, Delay);
		Card->QueueTransition(Transition);

		Delay = (Arena->PlayerHand->Count() * OffsetShuffleSpread) - IndexAfterSort * OffsetShuffleSpread;

		// Transition 7
		Position = B2TPosition
		{
			Position.EndPosition,
			Arena->PlayerHand->GetTransformForIndex(IndexAfterSort).Position,
			FVector(0, 0, 0),
			EEase::EaseInOut,
		};

		Rotation = B2TRotation
		{
			Rotation.EndRotation,
			Arena->PlayerHand->GetTransformForIndex(IndexAfterSort).Rotation,
			EEase::EaseOut,
		};

		// Add the transition to the transition queue
		Transition = B2Transition(WG_PostShuffleSpread, Position, Rotation, DurationShuffleSpread, Delay);
		Card->QueueTransition(Transition);
	}

	// Correct order for opponents hand
	TArray<FString> SortedOpponentHand = Arena->OpponentHand->GetSortedIDsDescending();

	// Opponents cards shuffle
	for (size_t i = 0; i < Arena->OpponentHand->Count(); i++)
	{
		float Delay = DelayShuffleSteps;

		// Get a pointer to the target card 
		ACard* Card = Arena->OpponentHand->GetCardByIndex(i);

		FVector CenterPosition = FMath::Lerp(Arena->OpponentHand->GetTransformForIndex(3).Position, Arena->OpponentHand->GetTransformForIndex(4).Position, 0.5f);
		CenterPosition += (i * FVector(0, 0, CARD_STACKING_OFFSET));
		FRotator CenterRotationUpwards = FMath::Lerp(Arena->OpponentHand->GetTransformForIndex(4).Rotation, Arena->OpponentHand->GetTransformForIndex(5).Rotation, 0.5f);

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
		B2Transition Transition = B2Transition(WG_Shuffle, Position, Rotation, DurationShuffleSteps, Delay);
		Card->QueueTransition(Transition);

		// Transition 2 (Sort)
		int IndexAfterSort = SortedOpponentHand.IndexOfByPredicate([Card](const FString& s) { return s == Card->GetID(); });

		CenterPosition = FMath::Lerp(Arena->OpponentHand->GetTransformForIndex(3).Position, Arena->OpponentHand->GetTransformForIndex(4).Position, 0.5f);
		CenterPosition += FVector(0, 0, 6.f) + (IndexAfterSort * FVector(0, 0, CARD_STACKING_OFFSET));

		Position = B2TPosition
		{
			Position.EndPosition,
			CenterPosition,
			FVector(0, 0, 0),
			EEase::EaseInOut,
		};

		// Rotation remains unchanged

		// Add the transition to the transition queue
		Transition = B2Transition(B2WaitGroupNone, Position, Rotation, 0.f, 0.f);
		Card->QueueTransition(Transition);

		// Post shuffle spread - included so we can reuse the sorted cards

		// Transition 3
		FVector SpreadStart = Arena->OpponentHand->GetTransformForIndex(0).Position;
		SpreadStart.Z += IndexAfterSort * CARD_STACKING_OFFSET;

		Position = B2TPosition
		{
			Position.EndPosition,
			SpreadStart,
			FVector(0, 0, 0),
			EEase::EaseIn,
		};

		Rotation = B2TRotation
		{
			Rotation.EndRotation,
			Arena->OpponentHand->GetTransformForIndex(0).Rotation,
			EEase::EaseIn,
		};

		// Add the transition to the transition queue
		Transition = B2Transition(B2WaitGroupNone, Position, Rotation, DurationShuffleSteps, Delay);
		Card->QueueTransition(Transition);

		Delay = (Arena->OpponentHand->Count() * OffsetShuffleSpread) - IndexAfterSort * OffsetShuffleSpread;

		// Transition 4
		Position = B2TPosition
		{
			Position.EndPosition,
			Arena->OpponentHand->GetTransformForIndex(IndexAfterSort).Position,
			FVector(0, 0, 0),
			EEase::EaseInOut,
		};

		Rotation = B2TRotation
		{
			Rotation.EndRotation,
			Arena->OpponentHand->GetTransformForIndex(IndexAfterSort).Rotation,
			EEase::EaseOut,
		};

		// Add the transition to the transition queue
		Transition = B2Transition(WG_PostShuffleSpread, Position, Rotation, DurationShuffleSpread, Delay);
		Card->QueueTransition(Transition);
	}
}

void UB2Dealer::MoveFromDeck(UCardSlot* SourceSlot, uint32 SourceIndex, UCardSlot* TargetSlot, bool bUseWaitGroup)
{
	const float DelayOnStart = 0.2f;
	const float DurationIntoDeck = 0.4f;

	B2WaitGroup MoveWaitGroup = bUseWaitGroup ? B2Transition::GetNextWaitGroup() : B2WaitGroupNone;
	if (bUseWaitGroup) WaitGroupCardMoveFinished = MoveWaitGroup + 1;

	float Delay = DelayOnStart;

	// Get a pointer to the target card
	ACard* Card = SourceSlot->GetCardByIndex(SourceIndex);

	// Remove it from the source slot
	SourceSlot->RemoveByIndex(SourceIndex);

	// Add it to the target slot
	TargetSlot->Add(Card);

	// Get target transform
	FB2Transform TargetTransform = TargetSlot->GetTransformForIndex(TargetSlot->Count() - 1);

	B2TPosition Position
	{
		Card->GetActorLocation(),
		TargetTransform.Position,
		FVector(0, 0, 12.f),
		EEase::EaseInOut,
	};

	B2TRotation Rotation
	{
		Card->GetActorRotation(),
		TargetTransform.Rotation,
		EEase::EaseInOut,
	};

	// Add the transition to the transition queue
	B2Transition Transition = B2Transition(MoveWaitGroup, Position, Rotation, DurationIntoDeck, Delay);
	Card->QueueTransition(Transition);
}

void UB2Dealer::Tick(float DeltaSeconds)
{
	// Start processing callbacks, but only the cards were dealt
	if (bCardsDealt)
	{
		B2WaitGroup CurrentWaitGroup = B2Transition::GetCurrentWaitGroup();

		// Various wait groups checked with if else - wait groups are not constant so cant use a switch statement

		// Fire a callback when the dealing transition has finished
		if (WaitGroupDealFinished == CurrentWaitGroup)
		{
			// Also re-sort the hands as their orders were messed up when shuffling
			Arena->PlayerHand->UpdateCardOrder();
			Arena->OpponentHand->UpdateCardOrder();

			// Fire the event and reset this wait group so we dont keep entering this part
			if (OnDealerEvent.IsBound()) OnDealerEvent.Broadcast(EDealerEvent::CardsDealt);
			WaitGroupDealFinished = B2WaitGroupNone;
		}

		if (WaitGroupCardMoveFinished == CurrentWaitGroup)
		{
			// Fire the event and reset this wait group so we dont keep entering this part
			if (OnDealerEvent.IsBound()) OnDealerEvent.Broadcast(EDealerEvent::CardPlaced);
			WaitGroupCardMoveFinished = B2WaitGroupNone;
		}
	}
}
