#pragma once

#include "CoreMinimal.h"

#include "B2Enum/ServerUpdateEnum.h"

struct B2ServerUpdate
{
	EServerUpdate Update;
	FString Metadata;
};

