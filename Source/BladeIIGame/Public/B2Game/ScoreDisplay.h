#pragma once

#include "CoreMinimal.h"

#include "Components/SceneComponent.h"

#include "ScoreDisplay.generated.h"

USTRUCT()
struct BLADEIIGAME_API FDigitPuddle
{
	GENERATED_BODY()

	/* Static mesh digits for this pool */
	UPROPERTY()
	TArray<UStaticMeshComponent*> Source;
};

UCLASS(Blueprintable)
class BLADEIIGAME_API UScoreDisplay : public USceneComponent
{
	GENERATED_BODY()

public:	
	UScoreDisplay();

	/**
	 * Update the score display
	 * @param PlayerScore - The player's score
	 * @param OpponentScore - The opponent's score
	 */
	void Update(uint32 PlayerScore, uint32 OpponentScore);

protected:

	/* Positions for the players score */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Configuration)
	TArray<FVector> PlayerScoreDigitPositions;

	/* Positions for the opponents score */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Configuration)
	TArray<FVector> OpponentScoreDigitPositions;

private:
	/* Root scene component for digit pool */
	UPROPERTY()
	USceneComponent* DigitPoolRoot;

	/* Pool of digits */
	UPROPERTY()
	TMap<uint32, FDigitPuddle> DigitPool;

	/* Static mesh digits for the player */
	UPROPERTY()
	TArray<UStaticMeshComponent*> PlayerDigits;

	/* Static mesh digits for the opponent */
	UPROPERTY()
	TArray<UStaticMeshComponent*> OpponentDigits;

	uint32 CurrentPlayerScore;
	uint32 CurrentOpponentScore;
};
