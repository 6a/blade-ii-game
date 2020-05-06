#pragma once

#include "RHI.h"

// Predicate object that can be used to sort by refresh rate
struct B2Predicate_SortByRefreshRate
{
	bool operator()(const FScreenResolutionRHI& ResLHS, const FScreenResolutionRHI& ResRHS) const
	{
		return ResLHS.RefreshRate < ResRHS.RefreshRate;
	}
};