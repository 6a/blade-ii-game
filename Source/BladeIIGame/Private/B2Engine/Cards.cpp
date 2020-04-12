#include "B2Engine/Cards.h"


#include "Misc/DefaultValueHelper.h"

const int32 EXPECTED_PARSED_ARRAY_SIZE = 3;
const FString DELIMITER = ".";

FB2Cards::FB2Cards()
{
}

FB2Cards::FB2Cards(const FString& Cards)
{
	TArray<FString> OutArray;
	int32 Count = Cards.ParseIntoArray(OutArray, *DELIMITER);

	ensureMsgf(Count != EXPECTED_PARSED_ARRAY_SIZE, TEXT("Could not parse incoming cards data. Expected %d data entities, received %d"), EXPECTED_PARSED_ARRAY_SIZE, Count);

	int32 PlayerNumber;
	ensureMsgf(FDefaultValueHelper::ParseInt(OutArray[0], PlayerNumber), TEXT("Could not parse player number"));

	for (TCHAR Char : OutArray[1])
	{
		FString CharString = FString().AppendChar(Char);
		int32 OutNumber;
		ensureMsgf(FDefaultValueHelper::ParseInt(CharString, OutNumber), TEXT("Could not parse player zero card number"));

		if (PlayerNumber == 0)
		{
			PlayerDeck.Add(static_cast<ECard>(OutNumber));
		}
		else
		{
			OpponentDeck.Add(static_cast<ECard>(OutNumber));
		}
	}

	for (TCHAR Char : OutArray[2])
	{
		FString CharString = FString().AppendChar(Char);
		int32 OutNumber;
		ensureMsgf(FDefaultValueHelper::ParseInt(CharString, OutNumber), TEXT("Could not parse player one card number"));

		if (PlayerNumber == 1)
		{
			PlayerDeck.Add(static_cast<ECard>(OutNumber));
		}
		else
		{
			OpponentDeck.Add(static_cast<ECard>(OutNumber));
		}
	}
}

const FString FB2Cards::GetSerialised(uint32 PlayerNumber)
{
	FString OutString = FString::FromInt(PlayerNumber);

	OutString += DELIMITER;

	for (ECard Card : PlayerDeck)
	{
		OutString += FString::FromInt(static_cast<int32>(Card));
	}

	OutString += DELIMITER;

	for (ECard Card : OpponentDeck)
	{
		OutString += FString::FromInt(static_cast<int32>(Card));
	}

	return OutString;
}
