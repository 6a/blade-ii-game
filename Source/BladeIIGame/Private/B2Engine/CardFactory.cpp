#include "B2Engine/CardFactory.h"

B2CardFactory::B2CardFactory(B2CardFactoryConfig& Config)
{
	B2Utility::LogInfo("Card Factory initialized");
}

B2CardFactory::~B2CardFactory()
{

}

ACard* B2CardFactory::Make(ECard Card, const FVector& TargetPosition)
{
	B2Utility::LogInfo(FString::Format(TEXT("Card Factory created card: [{0}]"), { B2Utility::EnumToString(Card) }));

	// Card generation logic	

	return nullptr;
}
