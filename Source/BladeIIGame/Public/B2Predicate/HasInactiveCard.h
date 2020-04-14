#pragma once

#include "CoreMinimal.h"

#include "B2Game/Card.h"

// Predicate object that can be used to identify cards that are inactive (got bolted)
struct B2Predicate_HasInactiveCard
{
	/* Find function (finds as described above) */
	bool operator()(ACard* Card) const {
		return !Card->IsActive();
	}
};