#include "B2Engine/Cards.h"

#include "Misc/DefaultValueHelper.h"

#include "B2Utility/Log.h"
#include "B2Utility/String.h"

const int32 EXPECTED_PARSED_ARRAY_SIZE = 3;
const FString DELIMITER = ".";

FB2Cards::FB2Cards()
{
}

FB2Cards::FB2Cards(const FString& Cards)
{
	TArray<FString> OutArray;
	int32 Count = Cards.ParseIntoArray(OutArray, *DELIMITER);

	ensureMsgf(Count == EXPECTED_PARSED_ARRAY_SIZE, TEXT("Could not parse incoming cards data. Expected %d data entities, received %d"), EXPECTED_PARSED_ARRAY_SIZE, Count);

	int32 PlayerNumber;
	ensureMsgf(FDefaultValueHelper::ParseInt(OutArray[0], PlayerNumber), TEXT("Could not parse player number"));

	// This parsing method is naiive and assumes that the input string contains consists of (array size) hex values, all of which are less than 16
	// and therefore only take up 1 char each.

	for (const TCHAR Char : OutArray[1])
	{
		FString CharString = FString().AppendChar(Char);
		uint32 ParsedInt = FParse::HexNumber(*CharString);

		if (PlayerNumber == 0)
		{
			PlayerDeck.Add(static_cast<ECard>(ParsedInt));
		}
		else
		{
			OpponentDeck.Add(static_cast<ECard>(ParsedInt));
		}
	}

	for (TCHAR Char : OutArray[2])
	{
		FString CharString = FString().AppendChar(Char);
		uint32 ParsedInt = FParse::HexNumber(*CharString);

		if (PlayerNumber == 1)
		{
			PlayerDeck.Add(static_cast<ECard>(ParsedInt));
		}
		else
		{
			OpponentDeck.Add(static_cast<ECard>(ParsedInt));
		}
	}
}

const FString FB2Cards::GetSerialised(uint32 PlayerNumber)
{
	FString OutString = B2Utility::UInt32ToHexString(PlayerNumber);

	OutString += DELIMITER;

	for (ECard Card : PlayerDeck)
	{
		OutString += B2Utility::UInt32ToHexString(static_cast<uint32>(Card));
	}

	OutString += DELIMITER;

	for (ECard Card : OpponentDeck)
	{
		OutString += B2Utility::UInt32ToHexString(static_cast<int32>(Card));
	}

	return OutString;
}
