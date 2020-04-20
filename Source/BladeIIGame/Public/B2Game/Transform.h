#pragma once

#include "CoreMinimal.h"

#include "Transform.generated.h"

/* Wrapper class for a position and a rotation */
USTRUCT(BlueprintType)
struct FB2Transform
{
	GENERATED_BODY()

public:
	FB2Transform()
		: Position(FVector::ZeroVector), Rotation(FRotator::ZeroRotator){ }

	FB2Transform(FVector Position, FRotator Rotation)
		: Position(Position), Rotation(Rotation) { }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Transform)
	FVector Position;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Transform)
	FRotator Rotation;
};