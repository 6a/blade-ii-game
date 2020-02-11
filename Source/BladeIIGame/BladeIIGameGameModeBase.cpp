#include "BladeIIGameGameModeBase.h"
#include "Private/Utility.h"

void ABladeIIGameGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	CardFactoryConfig cardFactoryConfig;

	CardFactory = QCardFactory(cardFactoryConfig);

	Utility::LogInfo("GameMode Initialized");
}