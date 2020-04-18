#pragma once

#include "CoreMinimal.h"

#include "B2Game/Card.h"

// Predicate object that sorts cards based on their type
struct B2Predicate_SortCardsByTypeAscending
{
	/* Sort function (sorts as described above) */
	bool operator()(const ACard& CardLHS, const ACard& CardRHS) const 
	{
		return CardLHS.Type > CardRHS.Type;
	}
};