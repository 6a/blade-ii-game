#pragma once

#include "B2Game/Card.h"

// Predicate object that can be used to identify cards that have a score that is equal to, or greater than, the specified value
struct B2Predicate_HasCardOfHighEnoughValue
{
	uint32 Min;

	B2Predicate_HasCardOfHighEnoughValue(uint32 Min) : Min(Min) {}

	/* Find function (finds as described above) */
	bool operator()(ECard Card) const {
		return ACard::TypeToValue(Card) >= Min;
	}
};