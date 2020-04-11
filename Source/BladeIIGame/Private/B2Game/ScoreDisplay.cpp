#include "B2Game/ScoreDisplay.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"

const uint32 DIGIT_MAX = 9;
const uint32 DIGIT_POOL_SIZE = 4;
const FString DIGIT_PATH_ROOT = "StaticMesh'/Game/BladeIIGame/Meshes/Digits/SM_Digit__";
const FVector HIDDEN_DIGIT_LOCATION = FVector(0, 0, -20);
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
			PlayerDigits[i]->SetWorldLocation(HIDDEN_DIGIT_LOCATION);
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
			float TotalDigitWidth = Digits[0]->Bounds.GetBox().GetSize().X + Digits[1]->Bounds.GetBox().GetSize().X;
			TotalDigitWidth += SPACING;

			FVector Offset = FVector(TotalDigitWidth * 0.5f, 0, 0);
			TArray<FVector> NewPositions = { PlayerDigitAnchor + Offset, PlayerDigitAnchor - Offset };

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
			OpponentDigits[i]->SetWorldLocation(HIDDEN_DIGIT_LOCATION);
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
			float TotalDigitWidth = Digits[0]->Bounds.GetBox().GetSize().X + Digits[1]->Bounds.GetBox().GetSize().X;
			TotalDigitWidth += SPACING;

			FVector Offset = FVector(TotalDigitWidth * 0.5f, 0, 0);
			TArray<FVector> NewPositions = { OpponentDigitAnchor + Offset, OpponentDigitAnchor - Offset };

			for (size_t i = 0; i < NewPositions.Num(); i++)
			{
				Digits[i]->SetWorldLocation(NewPositions[i]);
				Digits[i]->SetVisibility(true);
				OpponentDigits.Add(Digits[i]);
			}
		}
	}
}
