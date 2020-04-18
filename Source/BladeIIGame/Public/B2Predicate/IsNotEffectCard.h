#pragma once

#include "B2Enum/CardEnum.h"

// Predicate object that can be used to identify cards that are NOT effect cards (bolt, blast, force, and mirror)
struct B2Predicate_IsNotEffectCard
{
	/* Find function (finds as described above) */
	bool operator()(const ECard& Card) const 
	{
		return Card < ECard::Bolt;
	}
};