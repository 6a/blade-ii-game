#pragma once

#include "RHI.h"

// Predicate object that can be used to identify a matching resolution (ignores refresh rate)
struct B2Predicate_MatchResolution
{
	FScreenResolutionRHI ResolutionToCheck;

	B2Predicate_MatchResolution(const FScreenResolutionRHI& ResolutionToCheck)
		: ResolutionToCheck(ResolutionToCheck) {}

	/* Find function (finds as described above) */
	bool operator()(const FScreenResolutionRHI& Res) const
	{
		return Res.Width == ResolutionToCheck.Width && Res.Height == ResolutionToCheck.Height;
	}
};