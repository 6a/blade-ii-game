#pragma once

#include "B2Enum/CardEnum.h"

// Predicate object that will identify specific card types
struct B2Predicate_MatchCardType
{
	ECard Type;

	B2Predicate_MatchCardType(ECard Type)
		: Type(Type) {}

	/* Find function (finds as described above) */
	bool operator()(ACard* Card) {
		return Card->Type == Type;
	}
};