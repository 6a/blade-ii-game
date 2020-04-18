#pragma once

#include "B2Enum/CardEnum.h"

// Predicate object that will identify specific card types (for ACard)
struct B2Predicate_MatchCardActorType
{
	ECard Type;

	B2Predicate_MatchCardActorType(ECard Type)
		: Type(Type) {}

	/* Find function (finds as described above) */
	bool operator()(const ACard* const Card) const 
	{
		return Card->Type == Type;
	}
};

// Predicate object that will identify specific card types (for ECard)
struct B2Predicate_MatchCardEnumType
{
	ECard Type;

	B2Predicate_MatchCardEnumType(ECard Type)
		: Type(Type) {}

	/* Find function (finds as described above) */
	bool operator()(const ECard& Card) const 
	{
		return Card == Type;
	}
};