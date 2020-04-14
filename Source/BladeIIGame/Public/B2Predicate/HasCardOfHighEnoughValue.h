#pragma once

#include "B2Game/Card.h"

// Predicate object returns the a card if it has the same type as specified by Type
struct B2Predicate_HasCardOfHighEnoughValue
{
	uint32 Min;

	B2Predicate_HasCardOfHighEnoughValue(uint32 Min) : Min(Min) {}

	/* Find function (finds as described above) */
	bool operator()(ECard Card) const {
		return ACard::TypeToValue(Card) > Min;
	}
};