#include "BladeIIGameGameModeBase.h"
#include "Private/Utility.h"
#include "Private/LaunchConfig.h"

void ABladeIIGameGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	// Load card config (textures to use, etc)
	B2CardFactoryConfig B2CardFactoryConfig;
	CardFactory = new B2CardFactory(B2CardFactoryConfig);

	// Read the launch config
	B2LaunchConfig LaunchConfig("Launch.conf");

	// Do stuff with the launch config

	// Create the opponent class based launch config

	// Register event handlers
	Opponent->OnMoveReceived.AddDynamic(this, &ABladeIIGameGameModeBase::HandleMoveReceived);
	Opponent->OnInstructionReceived.AddDynamic(this, &ABladeIIGameGameModeBase::HandleInstructionReceived);

	B2Utility::LogInfo("GameMode initialized");

	// Pass arguments to base class function
	Super::InitGame(MapName, Options, ErrorMessage);
}

void ABladeIIGameGameModeBase::HandleMoveReceived(FB2Move& Move)
{

}

void ABladeIIGameGameModeBase::HandleInstructionReceived(EInstruction& Instruction)
{

}
