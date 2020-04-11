#include "B2Game/ScoreDisplay.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"

const uint32 DIGIT_MAX = 9;
const uint32 DIGIT_POOL_SIZE = 4;
const FString DIGIT_PATH_ROOT = "StaticMesh'/Game/BladeIIGame/Meshes/Digits/SM_Digit__";
const FString DIGIT_HIGHLIGHT_PATH = "StaticMesh'/Game/BladeIIGame/Meshes/Digits/SM_Digit_Highlight'";
const FString MATERIAL_STANDARD_PATH = "MaterialInstanceConstant'/Game/BladeIIGame/Materials/MI_Digit'";
const FString MATERIAL_HIGHLIGHT_PATH = "MaterialInstanceConstant'/Game/BladeIIGame/Materials/MI_Digits_Highlight'";
const FVector HIDDEN_LOCATION = FVector(0, 0, -20);
const float SPACING = 0.5f;

UScoreDisplay::UScoreDisplay()
{
	DigitPoolRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Digit Pool Root"));
	DigitPoolRoot->SetupAttachment(this);

	for (size_t i = 0; i <= DIGIT_MAX; i++)
	{
		DigitPool.Add(i, FDigitPuddle());

		for (size_t j = 0; j < DIGIT_POOL_SIZE; j++)
		{
			UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("Digit Pool [ %d ] #%d"), i, j));
			MeshComponent->SetupAttachment(DigitPoolRoot);
			MeshComponent->SetVisibility(false);
			
			ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(*FString::Printf(TEXT("%s%d'"), *DIGIT_PATH_ROOT, i));
			if (MeshFinder.Succeeded())
			{
				UStaticMesh* StaticMesh = MeshFinder.Object;
				MeshComponent->SetStaticMesh(StaticMesh);

				DigitPool[i].Source.Add(MeshComponent);
			}
		}
	}

	// Set up digit highliter
	DigitHighlighter = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Digit Highlighter"));
	DigitHighlighter->SetupAttachment(this);
	DigitHighlighter->SetVisibility(false);

	ConstructorHelpers::FObjectFinder<UStaticMesh> HighlighterMeshFinder(*DIGIT_HIGHLIGHT_PATH);
	if (HighlighterMeshFinder.Succeeded())
	{
		UStaticMesh* StaticMesh = HighlighterMeshFinder.Object;
		DigitHighlighter->SetStaticMesh(StaticMesh);
		DigitHighlighter->bCastDynamicShadow = false;
	}

	// Find digit materials
	ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialStandardFinder(*MATERIAL_STANDARD_PATH);
	if (MaterialStandardFinder.Succeeded())
	{
		DigitMaterialStandard = MaterialStandardFinder.Object;
	}

	// Find digit materials
	ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialHighlightFinder(*MATERIAL_HIGHLIGHT_PATH);
	if (MaterialHighlightFinder.Succeeded())
	{
		DigitMaterialHighlight = MaterialHighlightFinder.Object;
	}

	CurrentPlayerScore = CurrentOpponentScore = UINT32_MAX;
}

void UScoreDisplay::Update(uint32 PlayerScore, uint32 OpponentScore)
{
	// Clamp both scores between 0 and 99 to avoid any problems (and allows us to hardcode just two significant figures)
	PlayerScore = FMath::Clamp(PlayerScore, uint32(0), uint32(99));
	OpponentScore = FMath::Clamp(OpponentScore, uint32(0), uint32(99));

	// Player score
	if (CurrentPlayerScore != PlayerScore)
	{
		CurrentPlayerScore = PlayerScore;

		// Clear
		for (size_t i = 0; i < PlayerDigits.Num(); i++)
		{
			PlayerDigits[i]->SetWorldLocation(HIDDEN_LOCATION);
			PlayerDigits[i]->SetVisibility(false);
		}

		PlayerDigits.Empty(PlayerScore > 9 ? 2 : 1);

		// Get a pointer to each digit (mesh component) that we will be displaying
		// We take from the pool for each digit, but to avoid having to cache the state of the pools, we simply 
		// Grab the zeroth and first index for the players score - the opponents score will take the second and third
		// This way we wont overlap

		TArray<UStaticMeshComponent*> Digits;

		// Least significant digit
		Digits.Add(DigitPool[PlayerScore % 10].Source[0]);

		// Most significant (if the score is > 9)
		if (PlayerScore > 9)
		{
			Digits.Add(DigitPool[PlayerScore / 10].Source[1]);
		}

		// Move the digits out to their respective positions

		if (Digits.Num() == 1)
		{
			FVector Position = PlayerDigitAnchor;
			Digits[0]->SetWorldLocation(Position);
			Digits[0]->SetVisibility(true);

			PlayerDigits.Add(Digits[0]);
		}
		else
		{
			float TotalDigitWidth = Digits[0]->Bounds.GetBox().GetSize().X / 2 + Digits[1]->Bounds.GetBox().GetSize().X / 2;
			TotalDigitWidth += SPACING;

			FVector Offset = FVector(TotalDigitWidth * 0.5f, 0, 0);
			TArray<FVector> NewPositions = { PlayerDigitAnchor - Offset, PlayerDigitAnchor + Offset };

			for (size_t i = 0; i < NewPositions.Num(); i++)
			{
				Digits[i]->SetWorldLocation(NewPositions[i]);
				Digits[i]->SetVisibility(true);
				PlayerDigits.Add(Digits[i]);
			}
		}
	}

	// Opponent score
	if (CurrentOpponentScore != OpponentScore)
	{
		CurrentOpponentScore = OpponentScore;

		// Clear
		for (size_t i = 0; i < OpponentDigits.Num(); i++)
		{
			OpponentDigits[i]->SetWorldLocation(HIDDEN_LOCATION);
			OpponentDigits[i]->SetVisibility(false);
		}

		OpponentDigits.Empty(OpponentScore > 9 ? 2 : 1);

		// Get a pointer to each digit (mesh component) that we will be displaying
		// We take from the pool for each digit, but to avoid having to cache the state of the pools, we simply 
		// Grab the second and third index for the opponents score - the players score will take the zeroth and first
		// This way we wont overlap

		TArray<UStaticMeshComponent*> Digits;

		// Least significant digit
		Digits.Add(DigitPool[OpponentScore % 10].Source[2]);

		// Most significant (if the score is > 9)
		if (OpponentScore > 9)
		{
			Digits.Add(DigitPool[OpponentScore / 10].Source[3]);
		}

		// Move the digits out to their respective positions

		if (Digits.Num() == 1)
		{
			FVector Position = OpponentDigitAnchor;
			Digits[0]->SetWorldLocation(Position);
			Digits[0]->SetVisibility(true);

			OpponentDigits.Add(Digits[0]);
		}
		else
		{
			float TotalDigitWidth = Digits[0]->Bounds.GetBox().GetSize().X / 2 + Digits[1]->Bounds.GetBox().GetSize().X / 2;
			TotalDigitWidth += SPACING;

			FVector Offset = FVector(TotalDigitWidth * 0.5f, 0, 0);
			TArray<FVector> NewPositions = { PlayerDigitAnchor - Offset, PlayerDigitAnchor + Offset };

			for (size_t i = 0; i < NewPositions.Num(); i++)
			{
				Digits[i]->SetWorldLocation(NewPositions[i]);
				Digits[i]->SetVisibility(true);
				OpponentDigits.Add(Digits[i]);
			}
		}
	}
}

void UScoreDisplay::Highlight(EPlayer Target)
{
	// Only change if the incoming value is different to the current
	if (Target == CurrentlyHighlightTarget)
	{
		return;
	}
	else
	{
		CurrentlyHighlightTarget = Target;
	}

	// Reset all digits to the standard mesh
	// One of these calls may be redundant (when Target is NOT undecided) but there should be any overhead
	// As both changes are before the next render call anyway
	for (UStaticMeshComponent * SMC : PlayerDigits)
	{
		SMC->SetMaterial(0, DigitMaterialStandard);
	}

	for (UStaticMeshComponent * SMC : OpponentDigits)
	{
		SMC->SetMaterial(0, DigitMaterialStandard);
	}

	// Hide highlighter - again, possibly redundant when not undecided, but easier to switch off, and switch back on if needed
	DigitHighlighter->SetVisibility(false);
	DigitHighlighter->SetWorldLocation(HIDDEN_LOCATION);

	if (Target == EPlayer::Player)
	{
		for (UStaticMeshComponent* SMC : PlayerDigits)
		{
			SMC->SetMaterial(0, DigitMaterialHighlight);
		}

		DigitHighlighter->SetVisibility(true);
		DigitHighlighter->SetWorldLocation(PlayerDigitAnchor);
	}
	else if (Target == EPlayer::Opponent)
	{
		for (UStaticMeshComponent* SMC : OpponentDigits)
		{
			SMC->SetMaterial(0, DigitMaterialHighlight);
		}

		DigitHighlighter->SetVisibility(true);
		DigitHighlighter->SetWorldLocation(OpponentDigitAnchor);
	}
}
