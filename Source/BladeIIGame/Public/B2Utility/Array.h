#pragma once

#include "CoreMinimal.h"

namespace B2Utility
{
	template<typename InElementType, typename InAllocator>
	void ShuffleArray(TArray<InElementType, InAllocator>& InArray)
	{
		int32 LastIndex = InArray.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				InArray.SwapMemory(i, Index);
			}
		}
	}
}