#pragma once

#include "CoreMinimal.h"

/* EDealerEvent represents different dealer events (such as on deck dealt, on card placed) */
UENUM(BlueprintType)
enum class EDealerEvent : uint8
{
	None UMETA(DisplayName = "Default value"),
	CardsDealt UMETA(DisplayName = "Cards Dealt"),
	CardPlaced UMETA(DisplayName = "Card Placed"),
	EffectReady UMETA(DisplayName = "Card pre-effect transition finished"),
	BlastFinished UMETA(DisplayName = "Blast sequence finished"),
	CardPositionUpdateFinished UMETA(DisplayName = "Blast sequence finished"),
};