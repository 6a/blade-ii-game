#include "CardFactory.h"
#include "Utility.h"

QCardFactory::QCardFactory(CardFactoryConfig& config)
{

	Utility::LogInfo("Card Factory Initialized");
}

QCardFactory::~QCardFactory()
{

}

ACard* QCardFactory::Make(ECard card, const FVector& targetPosition)
{
	return nullptr;
}
