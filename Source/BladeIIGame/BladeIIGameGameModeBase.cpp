#include "BladeIIGameGameModeBase.h"

#include "UObject/UObjectGlobals.h"

#include "B2Misc/Utility.h"
#include "B2Engine/AIOpponent.h"
#include "B2Engine/NetOpponent.h"
#include "B2Engine/LaunchConfig.h"

ABladeIIGameGameModeBase::ABladeIIGameGameModeBase(const FObjectInitializer& ObjectInitializer)
{
		// Load card config (textures to use, etc)
	B2CardFactoryConfig B2CardFactoryConfig;
	CardFactory = new B2CardFactory(B2CardFactoryConfig);

	// Read the launch config
	B2LaunchConfig LaunchConfig("Launch.conf");

	// Do stuff with the launch config

	// Create the opponent class based launch config
	if (LaunchConfig.MatchID <= B2LaunchConfig::MATCH_ID_AI_GAME_THRESHOLD)
	{
		Opponent = ObjectInitializer.CreateDefaultSubobject<UB2AIOpponent>(this, TEXT("AI Opponent"));
		Opponent->Configure(static_cast<EAIDifficulty>(LaunchConfig.MatchID));
	}
	else
	{
		Opponent = ObjectInitializer.CreateDefaultSubobject<UB2NetOpponent>(this, TEXT("Net Opponent"));
		Opponent->Configure(LaunchConfig.PublicID, LaunchConfig.AuthToken, LaunchConfig.MatchID);
	}

	// Register event handlers
	Opponent->OnMoveReceived.AddDynamic(this, &ABladeIIGameGameModeBase::HandleMoveReceived);
	Opponent->OnInstructionReceived.AddDynamic(this, &ABladeIIGameGameModeBase::HandleInstructionReceived);

	B2Utility::LogInfo("GameMode initialized");
}

void ABladeIIGameGameModeBase::HandleMoveReceived(FB2Move& Move)
{

}

void ABladeIIGameGameModeBase::HandleInstructionReceived(EInstruction& Instruction)
{

}
