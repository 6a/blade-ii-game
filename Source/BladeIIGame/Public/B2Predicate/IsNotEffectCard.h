#pragma once

#include "CoreMinimal.h"

#include "B2Game/Card.h"

// Predicate object that can be used to identify cards that are NOT effect cards (rod, bolt, blast, force, and mirror)
struct B2Predicate_IsNotEffectCard
{
	/* Find function (finds as described above) */
	bool operator()(ACard* Card) const {
		return Card->Type > ECard::ElliotsOrbalStaff && Card->Type < ECard::Bolt;
	}
};