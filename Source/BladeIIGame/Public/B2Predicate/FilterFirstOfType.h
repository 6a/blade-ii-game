#pragma once

#include "B2Enum/CardEnum.h"

// Predicate object that will filter the first of the specified type
struct B2Predicate_FilterFirstOfType
{
	bool bFiltered;
	ECard Type;

	B2Predicate_FilterFirstOfType(ECard Type)
		: Type(Type)
	{
		bFiltered = false;
	}

	/* Find function (finds as described above) */
	bool operator()(const ECard& Card) 
	{
		if (!bFiltered)
		{
			if (Card == Type)
			{
				bFiltered = true;
				return false;
			}
		}

		return true;
	}
};