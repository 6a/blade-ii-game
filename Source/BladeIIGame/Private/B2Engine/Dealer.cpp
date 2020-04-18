#include "B2Engine/Dealer.h"

#include "B2Game/CardSelector.h"
#include "B2Utility/Log.h"

// Class scope values
const size_t DECK_CAPACITY = 15;
const size_t HAND_CAPACITY = 10;
const size_t FIELD_CAPACITY = 15;
const float MIN_MOVE_TRANSITION_DURATION = 0.3f;
const float MAX_MOVE_TRANSITION_DURATION = 1.4f;
const float MAX_MOVE_SQ_DISTANCE = 3267.789551f;
const float CARD_POP_OUT_DISTANCE = 2.f;
const FVector CARD_STACKING_OFFSET = FVector(0, 0, 0.03f);
const FVector CARD_FLIP_ARC = FVector(0, 0, 6);
const FVector MIRROR_ARC = FVector(0, 0, 4);
const FVector RAISED_CARD_OFFSET = FVector(0, 0, 6.f);
const FVector FORCE_MAX_OFFSET_PLAYER = FVector(-2, -0.4f, 122);
const FVector FORCE_MAX_OFFSET_OPPONENT = FVector(2, 0.4f, 122);

UB2Dealer::UB2Dealer()
{
	bCardsDealt = false;

	// Set all event-use waitgroups to none;
	WaitGroupDealFinished = B2WaitGroupNone;
	WaitGroupCardMoveFinished = B2WaitGroupNone;
	WaitGroupClearFinished = B2WaitGroupNone;
	WaitGroupEffectReady = B2WaitGroupNone;
	WaitGroupBlastFinished = B2WaitGroupNone;
	WaitGroupHandPositionUpdateFinished = B2WaitGroupNone;

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
	for (size_t i = 0; i < Arena->PlayerHand->Num(); i++)
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
	for (size_t i = 0; i < Arena->PlayerHand->Num(); i++)
	{
		float Delay = DelayPreShuffleGatherUp;
		FVector CenterPosition = FMath::Lerp(Arena->PlayerHand->GetTransformForIndex(3).Position, Arena->PlayerHand->GetTransformForIndex(4).Position, 0.5f) + (i * CARD_STACKING_OFFSET);
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
	for (size_t i = 0; i < Arena->OpponentHand->Num(); i++)
	{
		float Delay = DelayPreShuffleGatherUp;
		FVector CenterPosition = FMath::Lerp(Arena->OpponentHand->GetTransformForIndex(3).Position, Arena->OpponentHand->GetTransformForIndex(4).Position, 0.5f) + (i * CARD_STACKING_OFFSET);
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
	for (size_t i = 0; i < Arena->PlayerHand->Num(); i++)
	{
		float Delay = DelayShuffleSteps;

		// Get a pointer to the target card 
		ACard* Card = Arena->PlayerHand->GetCardByIndex(i);

		FVector CenterPosition = FMath::Lerp(Arena->PlayerHand->GetTransformForIndex(3).Position, Arena->PlayerHand->GetTransformForIndex(4).Position, 0.5f) + (i * CARD_STACKING_OFFSET);
		FRotator CenterRotationUpwards = FMath::Lerp(Arena->PlayerHand->GetTransformForIndex(4).Rotation, Arena->PlayerHand->GetTransformForIndex(5).Rotation, 0.5f);

		// Transition 1
		B2TPosition Position
		{
			CenterPosition,
			CenterPosition + RAISED_CARD_OFFSET,
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
		FVector VerticalOffset = i > 4 ? CARD_STACKING_OFFSET + ((i - 5) * CARD_STACKING_OFFSET * 2) : -(CARD_STACKING_OFFSET + ((4 - i) * CARD_STACKING_OFFSET * 2));

		Position = B2TPosition
		{
			Position.EndPosition,
			Position.EndPosition - VerticalOffset,
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
		CenterPosition += RAISED_CARD_OFFSET + (IndexAfterSort * CARD_STACKING_OFFSET);

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
			CenterPosition + VerticalOffset,
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
		FVector SpreadStart = Arena->PlayerHand->GetTransformForIndex(0).Position + IndexAfterSort * CARD_STACKING_OFFSET;

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

		Delay = (Arena->PlayerHand->Num() * OffsetShuffleSpread) - IndexAfterSort * OffsetShuffleSpread;

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

	// Opponents cards (no shuffle, just place)
	for (size_t i = 0; i < Arena->OpponentHand->Num(); i++)
	{
		float Delay = DelayShuffleSteps;

		// Get a pointer to the target card 
		ACard* Card = Arena->OpponentHand->GetCardByIndex(i);

		FVector CenterPosition = FMath::Lerp(Arena->OpponentHand->GetTransformForIndex(3).Position, Arena->OpponentHand->GetTransformForIndex(4).Position, 0.5f) + i * CARD_STACKING_OFFSET;
		FRotator CenterRotationUpwards = FMath::Lerp(Arena->OpponentHand->GetTransformForIndex(4).Rotation, Arena->OpponentHand->GetTransformForIndex(5).Rotation, 0.5f);

		// Transition 1
		B2TPosition Position
		{
			CenterPosition,
			CenterPosition + RAISED_CARD_OFFSET,
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

		// Transition 2
		FVector SpreadStart = Arena->OpponentHand->GetTransformForIndex(0).Position + i * CARD_STACKING_OFFSET;

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

		Delay = (Arena->OpponentHand->Num() * OffsetShuffleSpread) - i * OffsetShuffleSpread;

		// Transition 3
		Position = B2TPosition
		{
			Position.EndPosition,
			Arena->OpponentHand->GetTransformForIndex(i).Position,
			FVector(0, 0, 0),
			EEase::EaseInOut,
		};

		Rotation = B2TRotation
		{
			Rotation.EndRotation,
			Arena->OpponentHand->GetTransformForIndex(i).Rotation,
			EEase::EaseOut,
		};

		// Add the transition to the transition queue
		Transition = B2Transition(WG_PostShuffleSpread, Position, Rotation, DurationShuffleSpread, Delay);
		Card->QueueTransition(Transition);
	}
}

void UB2Dealer::FastDeal() 
{
	if (bCardsDealt)
	{
		B2Utility::LogWarning("Cards have already been dealt - cannot deal again");
		return;
	}

	check(Arena);

	bCardsDealt = true;

	B2WaitGroup WG_Final = B2Transition::GetNextWaitGroup();

	// Set the waitgroup for dealing
	WaitGroupDealFinished = WG_Final + 1;

	const float DelayOnStart = 0.5f;

	// Player cards - into deck
	for (int i = DECK_CAPACITY - 1; i >= 0; i--)
	{
		float Delay = DelayOnStart;

		// Grab the card and slap it onto correlating hand slot position
		ACard* Card = Arena->PlayerDeck->GetCardByIndex(i);

		FVector TargetPosition;
		FRotator TargetRotation;

		if (i > 4)
		{
			TargetPosition = Arena->PlayerHand->GetTransformForIndex(i - 5).Position;
			TargetRotation = Arena->PlayerHand->GetTransformForIndex(i - 5).Rotation;
			Arena->PlayerDeck->RemoveByIndex(i);
			Arena->PlayerHand->Add(Card);
		}
		else
		{
			TargetPosition = Arena->PlayerDeck->GetTransformForIndex(i).Position;
			TargetRotation = Arena->PlayerDeck->GetTransformForIndex(i).Rotation;
		}

		// Transition 1
		B2TPosition Position
		{
			Card->GetActorLocation(),
			TargetPosition,
			FVector(0, 0, 0),
			EEase::Linear,
		};

		B2TRotation Rotation
		{
			Card->GetActorRotation(),
			TargetRotation,
			EEase::Linear,
		};

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(WG_Final, Position, Rotation, 0.1f, Delay);
		Card->QueueTransition(Transition);
	}

	// Opponent cards - into deck
	for (int i = DECK_CAPACITY - 1; i >= 0; i--)
	{
		float Delay = DelayOnStart;

		// Grab the card and slap it onto correlating hand slot position
		ACard* Card = Arena->OpponentDeck->GetCardByIndex(i);

		FVector TargetPosition;
		FRotator TargetRotation;

		if (i > 4)
		{
			TargetPosition = Arena->OpponentHand->GetTransformForIndex(i - 5).Position;
			TargetRotation = Arena->OpponentHand->GetTransformForIndex(i - 5).Rotation;
			Arena->OpponentDeck->RemoveByIndex(i);
			Arena->OpponentHand->Add(Card);
		}
		else
		{
			TargetPosition = Arena->OpponentDeck->GetTransformForIndex(i).Position;
			TargetRotation = Arena->OpponentDeck->GetTransformForIndex(i).Rotation;
		}

		// Transition 1
		B2TPosition Position
		{
			Card->GetActorLocation(),
			TargetPosition,
			FVector(0, 0, 0),
			EEase::Linear,
		};

		B2TRotation Rotation
		{
			Card->GetActorRotation(),
			TargetRotation,
			EEase::Linear,
		};

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(WG_Final, Position, Rotation, 0.1f, Delay);
		Card->QueueTransition(Transition);
	}
}

void UB2Dealer::Move(UCardSlot* SourceSlot, uint32 SourceIndex, UCardSlot* TargetSlot, const FVector& Arc, bool bUseWaitGroup)
{
	const float DelayOnStart = 0.2f;

	// Get the target card
	ACard* Card = SourceSlot->RemoveByIndex(SourceIndex);

	// Calculate how long the transition should be
	const FVector SourceCardPosition = Card->GetActorLocation();
	const FB2Transform TargetTransform = TargetSlot->GetTransformForIndex(TargetSlot->Num());
	const float RatioOfMaxMoveDuration = FVector::DistSquared(SourceCardPosition, TargetTransform.Position) / MAX_MOVE_SQ_DISTANCE;
	const float TransitionDuration = FMath::Clamp(MAX_MOVE_TRANSITION_DURATION * RatioOfMaxMoveDuration, MIN_MOVE_TRANSITION_DURATION, MAX_MOVE_TRANSITION_DURATION);

	// Determine the wait group to use, and increment the finished wait group if required
	B2WaitGroup MoveWaitGroup = bUseWaitGroup ? B2Transition::GetNextWaitGroup() : B2WaitGroupNone;
	if (bUseWaitGroup) WaitGroupCardMoveFinished = MoveWaitGroup + 1;

	// Add it to the target slot
	TargetSlot->Add(Card);

	// Transition 1
	B2TPosition Position
	{
		Card->GetActorLocation(),
		TargetTransform.Position,
		Arc,
		EEase::EaseInOut,
	};

	B2TRotation Rotation
	{
		Card->GetActorRotation(),
		TargetTransform.Rotation,
		EEase::EaseInOut,
	};

	// Add the transition to the transition queue
	B2Transition Transition = B2Transition(MoveWaitGroup, Position, Rotation, TransitionDuration, DelayOnStart);
	Card->QueueTransition(Transition);
}

void UB2Dealer::PlayerEffectCard(ACard* Card)
{
	FVector Offset = GetDirectionNormalized(Card, EPlayer::Player) * CARD_POP_OUT_DISTANCE;
	EffectCard(Card, Offset);
}

void UB2Dealer::OpponentEffectCard(ACard* Card)
{
	FVector SelectorOffset = ACardSelector::OFFSET_WHEN_SELECTED * FVector(1, -1, 1);

	FVector Offset = GetDirectionNormalized(Card, EPlayer::Opponent) * CARD_POP_OUT_DISTANCE + SelectorOffset;
	EffectCard(Card, Offset);
}

void UB2Dealer::FlipFieldCard(ACard* Card, bool bNewActive, float Delay)
{
	Card->SetActive(bNewActive);

	const float DelayOnStart = Delay;
	const float TransitionDuration = 0.4f;

	B2WaitGroup WaitGroup = B2Transition::GetNextWaitGroup();

	FRotator TargetRotation = Card->GetActorRotation() + FRotator(180, 0, 0);

	// Transition 1
	B2TPosition Position
	{
		Card->GetActorLocation(),
		Card->GetActorLocation(),
		CARD_FLIP_ARC,
		EEase::EaseInOut,
	};

	B2TRotation Rotation
	{
		Card->GetActorRotation(),
		TargetRotation,
		EEase::EaseInOut,
	};

	// Add the transition to the transition queue
	B2Transition Transition = B2Transition(WaitGroup, Position, Rotation, TransitionDuration, DelayOnStart);
	Card->QueueTransition(Transition);
}

void UB2Dealer::Mirror()
{
	const float DelayOnStart = 0;
	const float MirrorDuration = 0.7f;

	B2WaitGroup WaitGroup = B2Transition::GetNextWaitGroup();

	// Perform all the swaps in advance

	TArray<ACard*> PlayerField = Arena->PlayerField->RemoveAll();
	TArray<ACard*> OpponentField = Arena->OpponentField->RemoveAll();

	Arena->PlayerField->Add(OpponentField);
	Arena->OpponentField->Add(PlayerField);

	for (size_t i = 0; i < PlayerField.Num(); i++)
	{
		ACard* Card = PlayerField[i];
		FB2Transform TargetTransform = Arena->OpponentField->GetTransformForIndex(i);

		B2TPosition Position
		{
			Card->GetActorLocation(),
			TargetTransform.Position,
			MIRROR_ARC,
			EEase::EaseInOut,
		};

		B2TRotation Rotation
		{
			Card->GetActorRotation(),
			TargetTransform.Rotation,
			EEase::EaseInOut,
		};

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(WaitGroup, Position, Rotation, MirrorDuration, DelayOnStart);
		Card->QueueTransition(Transition);
	}

	for (size_t i = 0; i < OpponentField.Num(); i++)
	{
		ACard* Card = OpponentField[i];
		FB2Transform TargetTransform = Arena->PlayerField->GetTransformForIndex(i);

		B2TPosition Position
		{
			Card->GetActorLocation(),
			TargetTransform.Position,
			MIRROR_ARC,
			EEase::EaseInOut,
		};

		B2TRotation Rotation
		{
			Card->GetActorRotation(),
			TargetTransform.Rotation,
			EEase::EaseInOut,
		};

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(WaitGroup, Position, Rotation, MirrorDuration, DelayOnStart);
		Card->QueueTransition(Transition);
	}
}

void UB2Dealer::PreBlastSelect(EPlayer Target)
{
	// Early exit with noop if the target is not specified
	if (Target == EPlayer::Undecided) return;

	UCardSlot* TargetSlot = Target == EPlayer::Player ? Arena->PlayerHand : Arena->OpponentHand;

	// Gather to center
	const float DelayOnStart = 0.45f;
	const float DelayPrePlayerHandFlip = 0.3f;
	const float TransitionGatherDuration = 0.45f;
	const float DurationPlayerHandFlip = 0.3f;

	FVector TargetPosition = FMath::Lerp(TargetSlot->GetTransformForIndex(3).Position, TargetSlot->GetTransformForIndex(4).Position, 0.5f);
	FRotator TargetRotation = FMath::Lerp(TargetSlot->GetTransformForIndex(4).Rotation, TargetSlot->GetTransformForIndex(5).Rotation, 0.5f);

	for (size_t i = 0; i < TargetSlot->Num(); i++)
	{
		ACard* Card = TargetSlot->GetCardByIndex(i);
		FVector Offset = RAISED_CARD_OFFSET + i * CARD_STACKING_OFFSET;

		B2TPosition Position
		{
			Card->GetActorLocation(),
			TargetPosition + Offset,
			FVector::ZeroVector,
			EEase::EaseInOut,
		};

		B2TRotation Rotation
		{
			Card->GetActorRotation(),
			TargetRotation,
			EEase::EaseInOut,
		};

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(B2WaitGroupNone, Position, Rotation, TransitionGatherDuration, DelayOnStart);
		Card->QueueTransition(Transition);

		if (Target == EPlayer::Player)
		{
			TargetRotation = FMath::Lerp(Arena->PlayerHandReversed->GetTransformForIndex(4).Rotation, Arena->PlayerHandReversed->GetTransformForIndex(5).Rotation, 0.5f);

			FVector VerticalOffset = (TargetSlot->Num() - 1 - i) * CARD_STACKING_OFFSET;

			Position = B2TPosition
			{
				Position.EndPosition,
				Position.EndPosition - VerticalOffset,
				FVector(0, 0, 0),
				EEase::EaseInOut,
			};

			Rotation = B2TRotation
			{
				Rotation.StartRotation,
				TargetRotation,
				EEase::EaseInOut,
			};

			// Add the transition to the transition queue
			Transition = B2Transition(B2WaitGroupNone, Position, Rotation, DurationPlayerHandFlip, DelayPrePlayerHandFlip);
			Card->QueueTransition(Transition);
		}
	}
}

void UB2Dealer::BlastSelect(EPlayer Target)
{
	// Early exit with noop if the target is not specified
	if (Target == EPlayer::Undecided) return;

	UCardSlot* TargetSlot = Target == EPlayer::Player ? Arena->PlayerHand : Arena->OpponentHand;

	const float DelayOnStart = 1;
	const float DelayPlayerHandFlip = 0.3f;
	const float DurationPlayerHandFlip = 0.45f;
	const float DurationSpread = 0.4f;

	// rotaion before spread declared here so it can be reused later
	FRotator RotationBeforeSpread;
	if (Target == EPlayer::Player)
	{
		RotationBeforeSpread = FMath::Lerp(Arena->PlayerHand->GetTransformForIndex(4).Rotation, Arena->PlayerHand->GetTransformForIndex(5).Rotation, 0.5f);
	}
	else
	{
		RotationBeforeSpread = FMath::Lerp(Arena->OpponentHand->GetTransformForIndex(4).Rotation, Arena->OpponentHand->GetTransformForIndex(5).Rotation, 0.5f);
	}

	B2WaitGroup WaitGroup = B2Transition::GetNextWaitGroup();
	WaitGroupEffectReady = WaitGroup + 1;

	for (size_t i = 0; i < TargetSlot->Num(); i++)
	{
		ACard* Card = TargetSlot->GetCardByIndex(i);

		// If the target is the players hand, flip the cards
		if (Target == EPlayer::Player)
		{
			B2TPosition Position
			{
				Card->GetActorLocation(),
				Card->GetActorLocation() + CARD_STACKING_OFFSET,
				FVector(0, 0, 0),
				EEase::EaseInOut,
			};

			B2TRotation Rotation
			{
				Card->GetActorRotation(),
				RotationBeforeSpread,
				EEase::EaseInOut,
			};

			// Add the transition to the transition queue
			B2Transition Transition = B2Transition(B2WaitGroupNone, Position, Rotation, DurationPlayerHandFlip, DelayPlayerHandFlip);
			Card->QueueTransition(Transition);
		}

		FVector Offset = GetDirectionNormalized(Card, Target) * CARD_POP_OUT_DISTANCE;

		FB2Transform TargetTransform = TargetSlot->GetTransformForIndex(i);
		TargetTransform.Position += RAISED_CARD_OFFSET + Offset;

		B2TPosition Position
		{
			Card->GetActorLocation(),
			TargetTransform.Position,
			FVector::ZeroVector,
			EEase::EaseInOut,
		};

		B2TRotation Rotation
		{
			RotationBeforeSpread,
			TargetSlot->GetTransformForIndex(i).Rotation,
			EEase::EaseInOut,
		};

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(WaitGroup, Position, Rotation, DurationSpread, DelayOnStart);
		Card->QueueTransition(Transition);
	}
}

void UB2Dealer::BlastCleanup(EPlayer Target)
{
	// Early exit with noop if the target is not specified
	if (Target == EPlayer::Undecided) return;

	B2WaitGroup WaitGroup = B2Transition::GetNextWaitGroup();
	WaitGroupBlastFinished = WaitGroup + 1;

	const float DelayOnStart = 0.0f;
	const float SpreadDelay = 0.25f;
	const float StackTransitionDuration = 0.5f;
	const float SpreadTransitionDuration = 0.5f;

	UCardSlot* TargetSlot = Target == EPlayer::Player ? Arena->PlayerHand : Arena->OpponentHand;

	FVector TargetPosition = FMath::Lerp(TargetSlot->GetTransformForIndex(3).Position, TargetSlot->GetTransformForIndex(4).Position, 0.5f);
	FRotator TargetRotation = FMath::Lerp(TargetSlot->GetTransformForIndex(4).Rotation, TargetSlot->GetTransformForIndex(5).Rotation, 0.5f);

	for (size_t i = 0; i < TargetSlot->Num(); i++)
	{
		if (Target == EPlayer::Player)
		{

		}

		ACard* Card = TargetSlot->GetCardByIndex(i);
		
		// First transition (stack)
		FVector StackPosition = TargetPosition + i * CARD_STACKING_OFFSET;

		B2TPosition Position
		{
			Card->GetActorLocation(),
			StackPosition,
			FVector::ZeroVector,
			EEase::EaseInOut,
		};

		B2TRotation Rotation
		{
			Card->GetActorRotation(),
			TargetRotation,
			EEase::EaseInOut,
		};

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(B2WaitGroupNone, Position, Rotation, StackTransitionDuration, DelayOnStart);
		Card->QueueTransition(Transition);

		// Last transition (spread)

		FB2Transform EndTransform = TargetSlot->GetTransformForIndex(i);

		Position = B2TPosition
		{
			Position.EndPosition,
			EndTransform.Position,
			FVector::ZeroVector,
			EEase::EaseInOut,
		};

		Rotation = B2TRotation
		{
			Rotation.EndRotation,
			EndTransform.Rotation,
			EEase::EaseInOut,
		};

		// Add the transition to the transition queue
		Transition = B2Transition(WaitGroup, Position, Rotation, SpreadTransitionDuration, SpreadDelay);
		Card->QueueTransition(Transition);
	}
}

void UB2Dealer::ForceOut(ACard* Card)
{
	const float DelayOnStart = 0.4f;
	const float TransitionDuration = 0.2f;

	FVector Target = CardIsFromPlayerField(Card) ? FORCE_MAX_OFFSET_PLAYER : FORCE_MAX_OFFSET_OPPONENT;

	// Transition 1
	B2TPosition Position
	{
		Card->GetActorLocation(),
		Target,
		FVector::ZeroVector,
		EEase::EaseIn,
	};

	B2TRotation Rotation
	{
		Card->GetActorRotation(),
		Card->GetActorRotation(),
		EEase::Linear,
	};

	// Add the transition to the transition queue
	B2Transition Transition = B2Transition(B2WaitGroupNone, Position, Rotation, TransitionDuration, DelayOnStart);
	Card->QueueTransition(Transition);
}

void UB2Dealer::ForceIn(ACard* Card)
{
	const float DelayOnStart = 0.0f;
	const float TransitionDuration = 0.5f;

	FB2Transform TargetTransform = CardIsFromPlayerField(Card) ? Arena->PlayerField->GetNextTransform() : Arena->OpponentField->GetNextTransform();

	// Transition 1
	B2TPosition Position
	{
		Card->GetActorLocation(),
		TargetTransform.Position,
		FVector::ZeroVector,
		EEase::EaseIn,
	};

	B2TRotation Rotation
	{
		Card->GetActorRotation(),
		TargetTransform.Rotation,
		EEase::EaseOut,
	};

	// Add the transition to the transition queue
	B2Transition Transition = B2Transition(B2WaitGroupNone, Position, Rotation, TransitionDuration, DelayOnStart);
	Card->QueueTransition(Transition);
}

void UB2Dealer::UpdateHandPositions(EPlayer Target)
{
	const float DelayOnStart = 0.1f;
	const float TransitionDuration = 0.35f;

	UCardSlot* TargetSlot = Target == EPlayer::Player ? Arena->PlayerHand : Arena->OpponentHand;

	// Early exit if the slot is empty
	if (TargetSlot->Num() <= 0) return;

	B2WaitGroup WaitGroup = B2Transition::GetNextWaitGroup();
	WaitGroupHandPositionUpdateFinished = WaitGroup + 1;

	for (size_t i = 0; i < TargetSlot->Num(); i++)
	{
		FB2Transform TargetTransform = TargetSlot->GetTransformForIndex(i);
		ACard* TargetCard = TargetSlot->GetCardByIndex(i);

		// Transition 1
		B2TPosition Position
		{
			TargetCard->GetActorLocation(),
			TargetTransform.Position,
			FVector::ZeroVector,
			EEase::EaseInOut,
		};

		B2TRotation Rotation
		{
			TargetCard->GetActorRotation(),
			TargetTransform.Rotation,
			EEase::EaseInOut,
		};

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(WaitGroup, Position, Rotation, TransitionDuration, DelayOnStart);
		TargetCard->QueueTransition(Transition);
	}
}

void UB2Dealer::ClearSingleFromField(ACard* Card) const
{
	const float DelayOnStart = 0.0f;
	const float ClearTransitionDuration = 0.5f;

	UCardSlot* TargetSlot = CardIsFromPlayerField(Card) ? Arena->PlayerDiscard : Arena->OpponentDiscard;
	UCardSlot* SourceSlot = CardIsFromPlayerField(Card) ? Arena->PlayerField : Arena->OpponentField;
	FVector TargetPosition = TargetSlot->GetTransformForIndex(0).Position;

	TargetSlot->Add(SourceSlot->RemoveByID(Card->GetID()));

	// Transition 1
	B2TPosition Position
	{
		Card->GetActorLocation(),
		TargetPosition,
		FVector::ZeroVector,
		EEase::EaseIn,
	};

	B2TRotation Rotation
	{
		Card->GetActorRotation(),
		Card->GetActorRotation(),
		EEase::EaseInOut,
	};

	// Add the transition to the transition queue
	B2Transition Transition = B2Transition(B2WaitGroupNone, Position, Rotation, ClearTransitionDuration, DelayOnStart);
	Card->QueueTransition(Transition);
}

void UB2Dealer::ClearField()
{
	const float DelayOnStart = 0.5f;
	const float ClearTransitionDuration = 0.5f;

	// Determine the wait group to use, and increment the finished wait group
	
	B2WaitGroup ClearWaitGroup = B2Transition::GetNextWaitGroup();
	B2WaitGroup PostDelayWaitGroup = B2Transition::GetNextWaitGroup();
	WaitGroupClearFinished = PostDelayWaitGroup + 1;

	FVector PlayerCardTargetPosition = Arena->PlayerDiscard->GetTransformForIndex(0).Position;
	for (int32 i = Arena->PlayerField->Num() - 1; i >= 0; i--)
	{
		ACard* Card = Arena->PlayerField->RemoveByIndex(i);
		Arena->PlayerDiscard->Add(Card);

		// Transition 1
		B2TPosition Position
		{
			Card->GetActorLocation(),
			PlayerCardTargetPosition,
			FVector::ZeroVector,
			EEase::EaseIn,
		};

		B2TRotation Rotation
		{
			Card->GetActorRotation(),
			Card->GetActorRotation(),
			EEase::EaseInOut,
		};

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(ClearWaitGroup, Position, Rotation, ClearTransitionDuration, DelayOnStart);
		Card->QueueTransition(Transition);
	}

	FVector OpponentCardTargetPosition = Arena->OpponentDiscard->GetTransformForIndex(0).Position;

	for (int32 i = Arena->OpponentField->Num() - 1; i >= 0; i--)
	{
		ACard* Card = Arena->OpponentField->RemoveByIndex(i);
		Arena->OpponentDiscard->Add(Card);

		// Transition 1
		B2TPosition Position
		{
			Card->GetActorLocation(),
			OpponentCardTargetPosition,
			FVector::ZeroVector,
			EEase::EaseIn,
		};

		B2TRotation Rotation
		{
			Card->GetActorRotation(),
			Card->GetActorRotation(),
			EEase::EaseInOut,
		};

		// Add the transition to the transition queue
		B2Transition Transition = B2Transition(ClearWaitGroup, Position, Rotation, ClearTransitionDuration, DelayOnStart);
		Card->QueueTransition(Transition);
	}

	// Add a delay in so that there is some time between the cards being cleared and the next round starting
	const float AddedDelay = 1.f;

	ACard* PCard = Arena->PlayerDiscard->GetFirst();
	ACard* OCard = Arena->OpponentDiscard->GetFirst();

	ACard* DelayCard = PCard ? PCard : OCard;

	// Transition 2
	B2TPosition Position
	{
		OpponentCardTargetPosition,
		OpponentCardTargetPosition,
		FVector::ZeroVector,
		EEase::EaseIn,
	};

	B2TRotation Rotation
	{
		DelayCard->GetActorRotation(),
		DelayCard->GetActorRotation(),
		EEase::EaseInOut,
	};

	// Add the transition to the transition queue
	B2Transition Transition = B2Transition(PostDelayWaitGroup, Position, Rotation, AddedDelay, 0);
	DelayCard->QueueTransition(Transition);
}

void UB2Dealer::Tick(float DeltaSeconds)
{
	// Start processing callbacks, but only the cards were dealt
	if (bCardsDealt)
	{
		B2WaitGroup CurrentWaitGroup = B2Transition::GetCurrentWaitGroup();

		// Various wait groups checked with if else - wait groups are not constant so cant use a switch statement

		// Fire a callback when the dealing transition has finished
		if (CurrentWaitGroup == WaitGroupDealFinished)
		{
			// Also re-sort the hands as their orders were messed up when shuffling
			Arena->PlayerHand->UpdateCardOrder();
			Arena->OpponentHand->UpdateCardOrder();

			// Fire the event and reset this wait group so we dont keep entering this part
			if (OnDealerEvent.IsBound()) OnDealerEvent.Broadcast(EDealerEvent::CardsDealt);
			WaitGroupDealFinished = B2WaitGroupNone;
		}
		else if (CurrentWaitGroup == WaitGroupCardMoveFinished)
		{
			// Fire the event and reset this wait group so we dont keep entering this part
			if (OnDealerEvent.IsBound()) OnDealerEvent.Broadcast(EDealerEvent::CardPlaced);
			WaitGroupCardMoveFinished = B2WaitGroupNone;
		}
		else if (CurrentWaitGroup == WaitGroupClearFinished)
		{
			// Fire the event and reset this wait group so we dont keep entering this part
			if (OnDealerEvent.IsBound()) OnDealerEvent.Broadcast(EDealerEvent::CardPlaced);
			WaitGroupClearFinished = B2WaitGroupNone;
		}
		else if (CurrentWaitGroup == WaitGroupEffectReady)
		{
			// Fire the event and reset this wait group so we dont keep entering this part
			if (OnDealerEvent.IsBound()) OnDealerEvent.Broadcast(EDealerEvent::EffectReady);
			WaitGroupEffectReady = B2WaitGroupNone;
		}
		else if (CurrentWaitGroup == WaitGroupBlastFinished)
		{
			// Fire the event and reset this wait group so we dont keep entering this part
			if (OnDealerEvent.IsBound()) OnDealerEvent.Broadcast(EDealerEvent::BlastFinished);
			WaitGroupBlastFinished = B2WaitGroupNone;
		}
		else if (CurrentWaitGroup == WaitGroupHandPositionUpdateFinished)
		{
			// Fire the event and reset this wait group so we dont keep entering this part
			if (OnDealerEvent.IsBound()) OnDealerEvent.Broadcast(EDealerEvent::CardPositionUpdateFinished);
			WaitGroupHandPositionUpdateFinished = B2WaitGroupNone;
		}
	}
}

void UB2Dealer::EffectCard(ACard* Card, FVector Offset)
{
	const float DelayOnStart = 0.2f;
	const float TransitionDuration = 0.2f;

	B2WaitGroup WaitGroup = B2Transition::GetNextWaitGroup();
	WaitGroupEffectReady = WaitGroup + 1;

	FVector TargetPosition = Card->GetActorLocation() + Offset;
	FRotator TargetRotation = Card->IsFaceDown() ? Card->GetActorRotation() + FRotator(180, 0, 0) : Card->GetActorRotation();

	// Transition 1
	B2TPosition Position
	{
		Card->GetActorLocation(),
		TargetPosition,
		FVector::ZeroVector,
		EEase::EaseInOut,
	};

	B2TRotation Rotation
	{
		Card->GetActorRotation(),
		TargetRotation,
		EEase::EaseInOut,
	};

	// Add the transition to the transition queue
	B2Transition Transition = B2Transition(WaitGroup, Position, Rotation, TransitionDuration, DelayOnStart);
	Card->QueueTransition(Transition);
}

FVector UB2Dealer::GetDirectionNormalized(const ACard* Card, const EPlayer Target) const
{
	// Early exit with a zero vector if the player is not specified
	if (Target == EPlayer::Undecided) return FVector::ZeroVector;

	float Rotation = Target == EPlayer::Player ? 90 : -90;

	return Card->GetActorRotation().Vector().RotateAngleAxis(Rotation, FVector::UpVector).GetSafeNormal();
}

bool UB2Dealer::CardIsFromPlayerField(const ACard* Card) const
{
	return Card->GetActorLocation().Y < 0;
}
