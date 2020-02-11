#include "CardFactory.h"
#include "Utility.h"

QCardFactory::QCardFactory(CardFactoryConfig& config)
{
	bIsInitialised = true;

	Utility::LogInfo("Card Factory Initialized");
}

QCardFactory::~QCardFactory()
{

}

ACard* QCardFactory::Make(ECard card, const FVector& targetPosition)
{
	if (ensure(bIsInitialised))
	{
		Utility::LogInfo(FString::Format(TEXT("Card Factory created card: [{0}]"), { ACard::EnumToString(card) }));
	
		// Card generation logic	
	}

	return nullptr;
}
