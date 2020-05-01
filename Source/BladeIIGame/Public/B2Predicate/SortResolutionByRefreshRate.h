#pragma once

#include "RHI.h"

// Predicate object that can be used to sort resolutions by refresh rate (also takes into account width and height
struct B2Predicate_SortResolutionByRefreshRate
{
	/* Find function (finds as described above) */
	bool operator()(const FScreenResolutionRHI& ResLHS, const FScreenResolutionRHI& ResRHS) const
	{
		return (ResLHS.RefreshRate < ResRHS.RefreshRate) || (ResLHS.Width < ResRHS.Width) || (ResLHS.Height < ResRHS.Height);
	}
};