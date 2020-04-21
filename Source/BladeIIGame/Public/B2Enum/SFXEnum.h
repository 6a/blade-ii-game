#pragma once

#include "CoreMinimal.h"

/* EPlayer represents different dealer events (such as on deck dealt, on card placed) */
UENUM(BlueprintType)
enum class ESFX : uint8
{
	TextPulse UMETA(DisplayName = "Text Pulse"),
	CardDeal UMETA(DisplayName = "Card Deal"),
	CursorNavigate UMETA(DisplayName = "Cursor Navigate"),
	CursorSelect UMETA(DisplayName = "Cursor Select"),
	EffectBase UMETA(DisplayName = "Base Effect SFX"),
	EffectRod UMETA(DisplayName = "Rod Effect SFX"),
	EffectBolt UMETA(DisplayName = "Bolt SFX"),
	EffectBlastStart UMETA(DisplayName = "Blast Start SFX"),
	EffectBlastSelect UMETA(DisplayName = "Blast Select SFX"),
	EffectForce UMETA(DisplayName = "Force SFX"),
};