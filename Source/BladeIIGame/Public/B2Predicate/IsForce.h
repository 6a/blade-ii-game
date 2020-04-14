#pragma once

#include "B2Enum/CardEnum.h"

// Predicate object that can be used to identify force cards
struct B2Predicate_IsForce
{
	/* Find function (finds as described above) */
	bool operator()(ECard Card) const {
		return Card == ECard::Force;
	}
};