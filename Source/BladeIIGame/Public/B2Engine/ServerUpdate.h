#pragma once

#include "CoreMinimal.h"

#include "B2Enum/ServerUpdateEnum.h"

#include "ServerUpdate.generated.h"

USTRUCT()
struct FB2ServerUpdate
{
	GENERATED_BODY()

	EServerUpdate Update;
	FString Metadata;
};

