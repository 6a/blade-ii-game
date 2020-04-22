#include "B2UI/Avatar.h"

#include "B2Utility/Log.h"

/* Const values for building localisation keys */
const FString LOC_KEY_PREFIX = "CHAR_";
const FString LOC_KEY_DELIM = "_";
const FString LOC_KEY_GREETING = "_GREETING";
const FString LOC_KEY_DRAW = "_DRAW";
const FString LOC_KEY_PLAYERFIRST = "_PLAYERFIRST";
const FString LOC_KEY_OPPONENTFIRST = "_OPPONENTFIRST";
const FString LOC_KEY_MATCHEDSCORES = "_MATCHEDSCORES";
const FString LOC_KEY_BOLT = "_BOLT";
const FString LOC_KEY_MIRROR = "_MIRROR";
const FString LOC_KEY_BLAST = "_BLAST";
const FString LOC_KEY_FORCE = "_FORCE";
const FString LOC_KEY_REACTION = "_REACTION";
const FString LOC_KEY_VICTORY = "_VICTORY";
const FString LOC_KEY_DEFEAT = "_DEFEAT";
const FString LOC_KEY_SUFFIX_000 = "000";
const FString LOC_KEY_SUFFIX_001 = "001";

const FTextKey LOC_KEY_CHAR_NAMESPACE = "LOC_CHAR";

void UAvatar::SetOpponentMessage(EOpponentMessage Message, const FString& Character)
{
	Callout->SetText(GetTextForMessage(Message, Character).ToString());
}

bool UAvatar::IsAnimatingCalloutText() const
{
	return Callout->IsAnimatingText();
}

bool UAvatar::IsCalloutActive() const
{
	return Callout->IsActive();
}

const FText UAvatar::GetTextForMessage(EOpponentMessage Message, const FString& Character) const
{
	const FString CharNameAllCaps = Character.ToUpper();

	FString Suffix = "";
	if (Message != EOpponentMessage::Reaction)
	{
		Suffix = LOC_KEY_SUFFIX_000;
	}
	else
	{
		Suffix = FMath::RandBool() ? LOC_KEY_SUFFIX_000 : LOC_KEY_SUFFIX_001;
	}

	const FTextKey ConstructedLocKey(FString::Printf(TEXT("%s%s%s%s"), *LOC_KEY_PREFIX, *CharNameAllCaps, *(MessageTypeToString(Message)), *Suffix));

	FText OutText;
	bool bFoundText = FText::FindText(LOC_KEY_CHAR_NAMESPACE, ConstructedLocKey, OutText);

	if (!bFoundText)
	{
		B2Utility::LogWarning(FString("Could not load localisation text for key: ").Append(FString(ConstructedLocKey.GetChars())));
	}

	return OutText;
}

FString UAvatar::MessageTypeToString(EOpponentMessage Message) const
{
	FString OutMessageString;

	switch (Message)
	{
	case EOpponentMessage::Greeting:
		OutMessageString = LOC_KEY_GREETING;
		break;
	case EOpponentMessage::Draw:
		OutMessageString = LOC_KEY_DRAW;
		break;
	case EOpponentMessage::PlayerGoesFirst:
		OutMessageString = LOC_KEY_PLAYERFIRST;
		break;
	case EOpponentMessage::OpponentGoesFirst:
		OutMessageString = LOC_KEY_OPPONENTFIRST;
		break;
	case EOpponentMessage::MatchedScores:
		OutMessageString = LOC_KEY_MATCHEDSCORES;
		break;
	case EOpponentMessage::Bolt:
		OutMessageString = LOC_KEY_BOLT;
		break;
	case EOpponentMessage::Mirror:
		OutMessageString = LOC_KEY_MIRROR;
		break;
	case EOpponentMessage::Blast:
		OutMessageString = LOC_KEY_BLAST;
		break;
	case EOpponentMessage::Force:
		OutMessageString = LOC_KEY_FORCE;
		break;
	case EOpponentMessage::Reaction:
		OutMessageString = LOC_KEY_REACTION;
		break;
	case EOpponentMessage::Victory:
		OutMessageString = LOC_KEY_VICTORY;
		break;
	case EOpponentMessage::Defeat:
		OutMessageString = LOC_KEY_DEFEAT;
		break;
	}

	return OutMessageString.Append(LOC_KEY_DELIM);
}
