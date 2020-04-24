#pragma once

#include "CoreMinimal.h"

#include "JsonObjectConverter.h"

#include "WebSocketPacket.generated.h"

USTRUCT()
struct FB2WebSocketPacket
{
	GENERATED_BODY()

	UPROPERTY()
	uint16 Code;

	UPROPERTY()
	FString Message;

	/**
	 * Returns the JSON serialised version of this server message
	 * @return The update as a string
	 */
	FString GetSerialised()
	{
		FString OutJsonString;

		bool bSuccess = FJsonObjectConverter::UStructToJsonObjectString<FB2WebSocketPacket>(*this, OutJsonString);

		return OutJsonString;
	}

	/**
	 * Static helper - converts a string representation of a server message to a FB2WebSocketPacket
	 * @return The converted message object
	 */
	static FB2WebSocketPacket FromJSONString(const FString& JSONString)
	{
		FB2WebSocketPacket OutUpdate{ 0 };

		if (!FJsonObjectConverter::JsonObjectStringToUStruct<FB2WebSocketPacket>(JSONString, &OutUpdate, 0, 0))
		{
			// Report error?
		}

		return OutUpdate;
	}
};

