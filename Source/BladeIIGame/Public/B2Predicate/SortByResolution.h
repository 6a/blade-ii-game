#pragma once

#include "RHI.h"
#include "B2Utility/Log.h"

// Predicate object that can be used to sort resolutions (if they dont match the expected aspect ratio, they will be pushed to the bottom)
struct B2Predicate_SortByResolution
{
	const float MARGIN_FOR_ERROR = 0.005f;
	float ExpectedAspectRatio;

	/*
	 * Sort by resolution, ascending
	 * @param ExpectedAspectRatio - The ratio of height to width (for example, 1920x1080 = 1080/1920 = 0.5625)
	*/
	B2Predicate_SortByResolution(float ExpectedAspectRatio)
		: ExpectedAspectRatio(ExpectedAspectRatio) {}

	/* Find function (finds as described above) */
	bool operator()(const FScreenResolutionRHI& ResLHS, const FScreenResolutionRHI& ResRHS) const
	{
		float AspectRatioLHS = static_cast<float>(ResLHS.Height) / static_cast<float>(ResLHS.Width);
		bool bIsNearlyEqual = FMath::IsNearlyEqual(AspectRatioLHS, ExpectedAspectRatio, MARGIN_FOR_ERROR);
		return !bIsNearlyEqual && ResLHS.Width < ResRHS.Width;
	}
};