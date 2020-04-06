#pragma once

#include "CoreMinimal.h"

#include "B2Game/Card.h"

// Predicate object that sorts cards based on their (square) distance from <RootLocation>
struct B2Predicate_SortCardsByDistanceAscending
{
	FVector RootLocation;
 	
	/* @param RootLocation - The location from which the distance calculations will be performed */
	B2Predicate_SortCardsByDistanceAscending(const FVector& RootLocation)
		:RootLocation(RootLocation) {}

	/* Sort function (sorts as described above) */
	bool operator()(const ACard& CardLHS, const ACard& CardRHS) const {

		// Distance square check so that sqrt isnt needed - we dont need the actual value, just have to compare relative to eachother
		float SquareDistanceLHS = FVector::DistSquared(RootLocation, CardLHS.GetActorLocation());
		float SquareDistanceRHS = FVector::DistSquared(RootLocation, CardRHS.GetActorLocation());

		return SquareDistanceLHS < SquareDistanceRHS;
	}
};