#pragma once

#include "B2Enum/CardEnum.h"

// Predicate object that can be used to identify rod cards
struct B2Predicate_IsRod
{
	/* Find function (finds as described above) */
	bool operator()(ECard Card) const {
		return Card == ECard::ElliotsOrbalStaff;
	}
};