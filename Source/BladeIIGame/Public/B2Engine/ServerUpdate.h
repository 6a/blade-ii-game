#pragma once

#include "CoreMinimal.h"

#include "JsonObjectConverter.h"

#include "B2Enum/ServerUpdateEnum.h"

#include "ServerUpdate.generated.h"

USTRUCT()
struct FB2ServerUpdate
{
	GENERATED_BODY()

	UPROPERTY()
	EServerUpdate Update;

	UPROPERTY()
	FString Metadata;

	/**
	 * Returns the JSON serialised version of this server update
	 * @return The update as a string
	 */
	FString GetSerialised()
	{
		FString OutJsonString;

		bool bSuccess = FJsonObjectConverter::UStructToJsonObjectString<FB2ServerUpdate>(*this, OutJsonString);

		return OutJsonString;
	}

	/**
	 * Static helper - converts a string representation of a server message to a FB2ServerUpdate
	 * @return The converted update object
	 */
	static FB2ServerUpdate FromJSONString(const FString& JSONString)
	{
		FB2ServerUpdate OutUpdate{ EServerUpdate::None };

		if (!FJsonObjectConverter::JsonObjectStringToUStruct<FB2ServerUpdate>(JSONString, &OutUpdate, 0, 0))
		{
			// Report error?
		}

		return OutUpdate;
	}
};

