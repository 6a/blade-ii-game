#include "BladeIIGameGameModeBase.h"
#include "Private/Utility.h"
#include "Private/LaunchConfig.h"

void ABladeIIGameGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	B2CardFactoryConfig B2CardFactoryConfig;
	CardFactory = new B2CardFactory(B2CardFactoryConfig);

	B2LaunchConfig LaunchConfig("Launch.conf");

	B2Utility::LogInfo("GameMode initialized");
}