#pragma once

#include "CoreMinimal.h"

#include "B2Game/Card.h"

// Predicate object that can be used to identify force cards
struct B2Predicate_IsForce
{
	/* Find function (finds as described above) */
	bool operator()(ACard* Card) const {
		return Card->Type == ECard::Force;
	}
};