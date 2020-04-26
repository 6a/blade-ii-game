#pragma once

#include "CoreMinimal.h"

#include "Misc/DefaultValueHelper.h"

#include "B2Enum/ServerUpdateEnum.h"

#include "ServerUpdate.generated.h"

const FString SERIALIZED_SERVER_UPDATE_DELIMITER = TEXT(":");
const uint32 DESERIALIZED_SERVER_UPDATE_COMPONENTS = 2;

USTRUCT()
struct FB2ServerUpdate
{
	GENERATED_BODY()

	EServerUpdate Code;
	FString Payload;

	/**
	 * Returns the serialized version of this server update 
	 * @return The update as a string
	 */
	FString GetSerialised()
	{
		FString dlm = SERIALIZED_SERVER_UPDATE_DELIMITER;

		return FString::Printf(TEXT("%d%s%s"), Code, *dlm, *Payload);
	}

	/**
	 * Static helper - converts a string representation of a server message to a FB2ServerUpdate
	 * @return The converted update object
	 */
	static FB2ServerUpdate UnSerialise(const FString& InString)
	{
		FB2ServerUpdate OutUpdate{ EServerUpdate::None };

		TArray<FString> OutArray;
		int32 Count = InString.ParseIntoArray(OutArray, *SERIALIZED_SERVER_UPDATE_DELIMITER, false);

		if (OutArray.Num() == DESERIALIZED_SERVER_UPDATE_COMPONENTS)
		{
			int32 OutInt;
			bool bSuccess = FDefaultValueHelper::ParseInt(OutArray[0], OutInt);

			// Probably should check if its valid before casting?
			OutUpdate.Code = bSuccess ? static_cast<EServerUpdate>(OutInt) : EServerUpdate::None;
			OutUpdate.Payload = OutArray[1];
		}

		return OutUpdate;
	}
};

