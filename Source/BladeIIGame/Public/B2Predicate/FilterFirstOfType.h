#pragma once

#include "B2Enum/CardEnum.h"

// Predicate object that will identify the first of the specified type
struct B2Predicate_FilterFirstOfType
{
	bool bFound;
	ECard Type;

	B2Predicate_FilterFirstOfType(ECard Type)
		: Type(Type)
	{
		bFound = false;
	}

	/* Find function (finds as described above) */
	bool operator()(ECard Card) {
		if (bFound)
		{
			return false;
		}
		
		if (Card == Type)
		{
			bFound = true;
		}

		return bFound;
	}
};