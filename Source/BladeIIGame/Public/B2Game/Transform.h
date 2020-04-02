#pragma once

#include "CoreMinimal.h"

/* Wrapper class for a position and a rotation */
static struct B2Transform
{
public:
	B2Transform()
		: Position(FVector()), Rotation(FRotator()){}

	B2Transform(FVector Position, FRotator Rotation)
		: Position(Position), Rotation(Rotation) { }

	FVector Position;
	FRotator Rotation;
};