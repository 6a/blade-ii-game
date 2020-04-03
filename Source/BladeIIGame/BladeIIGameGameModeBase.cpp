#include "BladeIIGameGameModeBase.h"

#include "EngineUtils.h"
#include "UObject/UObjectGlobals.h"

#include "B2Misc/Utility.h"
#include "B2Engine/AIOpponent.h"
#include "B2Engine/NetOpponent.h"
#include "B2Engine/LaunchConfig.h"
#include "B2Engine/LocalPlayerInput.h"
#include "B2Misc/Transition.h"

ABladeIIGameGameModeBase::ABladeIIGameGameModeBase(const FObjectInitializer& ObjectInitializer)
{
	DefaultPawnClass = ALocalPlayerInput::StaticClass();

	SetupLaunchConfig(ObjectInitializer);
	
	SetupCardFactory();

	RegisterEventListeners();

	B2Utility::LogInfo("GameMode initialized");
}

void ABladeIIGameGameModeBase::StartPlay()
{
	Super::StartPlay();

	FindArena();

	SetupDealer();

	InitialiseBoard(B2BoardState());

	Dealer->Deal();

	B2Utility::LogInfo("ABladeIIGameGameModeBase::StartPlay");
}

void ABladeIIGameGameModeBase::SetupLaunchConfig(const FObjectInitializer& ObjectInitializer)
{
	// Read the launch config
	B2LaunchConfig LaunchConfig("Launch.conf");

	// Create the opponent class based launch config
	if (LaunchConfig.MatchID <= B2LaunchConfig::MATCH_ID_AI_GAME_THRESHOLD)
	{
		UB2AIOpponent* AIOpponent = ObjectInitializer.CreateDefaultSubobject<UB2AIOpponent>(this, TEXT("AI Opponent"));
		AIOpponent->Configure(static_cast<EAIDifficulty>(LaunchConfig.MatchID));

		Opponent = static_cast<UB2Opponent*>(AIOpponent);
	}
	else
	{
		UB2NetOpponent* NetOpponent = ObjectInitializer.CreateDefaultSubobject<UB2NetOpponent>(this, TEXT("Net Opponent"));
		NetOpponent->Configure(LaunchConfig.PublicID, LaunchConfig.AuthToken, LaunchConfig.MatchID);

		Opponent = static_cast<UB2Opponent*>(NetOpponent);
	}
}

void ABladeIIGameGameModeBase::SetupCardFactory()
{
	// Load card config (textures to use, etc)
	B2CardFactoryConfig B2CardFactoryConfig;

	B2CardFactoryConfig.CardFrontPaths = TArray<FString>({
		/* Basic cards */
		TEXT("/Game/BladeIIGame/Textures/T_Card_Staff.T_Card_Staff"),
		TEXT("/Game/BladeIIGame/Textures/T_Card_Gunswords.T_Card_Gunswords"),
		TEXT("/Game/BladeIIGame/Textures/T_Card_Bow.T_Card_Bow"),
		TEXT("/Game/BladeIIGame/Textures/T_Card_Sword.T_Card_Sword"),
		TEXT("/Game/BladeIIGame/Textures/T_Card_Shotgun.T_Card_Shotgun"),
		TEXT("/Game/BladeIIGame/Textures/T_Card_Spear.T_Card_Spear"),
		TEXT("/Game/BladeIIGame/Textures/T_Card_Broadsword.T_Card_Broadsword"),

		/* Special cards */
		TEXT("/Game/BladeIIGame/Textures/T_Card_Bolt.T_Card_Bolt"),
		TEXT("/Game/BladeIIGame/Textures/T_Card_Mirror.T_Card_Mirror"),
		TEXT("/Game/BladeIIGame/Textures/T_Card_Blast.T_Card_Blast"),
		TEXT("/Game/BladeIIGame/Textures/T_Card_Force.T_Card_Force"),
	});

	/* Back and MRS (metallic, roughness, specular) paths */
	B2CardFactoryConfig.CardBackPath = TEXT("/Game/BladeIIGame/Textures/T_Up_Test.T_Up_Test");
	B2CardFactoryConfig.CardFrontMRSPath = TEXT("/Game/BladeIIGame/Textures/T_Card_Front_MRS.T_Card_Front_MRS");
	B2CardFactoryConfig.CardBackMRSPath = TEXT("/Game/BladeIIGame/Textures/T_Card_Back_MRS.T_Card_Back_MRS");

	/* Card actor path */
	B2CardFactoryConfig.CardActorPath = TEXT("Blueprint'/Game/BladeIIGame/Blueprints/GameObjects/BP_Card.BP_Card'");

	/* Pointer to current world */
	B2CardFactoryConfig.World = GetWorld();

	CardFactory = new B2CardFactory(B2CardFactoryConfig);
}

void ABladeIIGameGameModeBase::RegisterEventListeners()
{
	// Register event listeners
	Opponent->OnMoveReceived.AddDynamic(this, &ABladeIIGameGameModeBase::HandleMoveReceived);
	Opponent->OnInstructionReceived.AddDynamic(this, &ABladeIIGameGameModeBase::HandleInstructionReceived);
}

void ABladeIIGameGameModeBase::FindArena()
{
	// Try to get a reference to the arena
	for (TActorIterator<AArena> ArenaIter(GetWorld()); ArenaIter; ++ArenaIter)
	{
		if (ArenaIter)
		{
			Arena = *ArenaIter;
		}
	}

	// Throw if the arena was not found
	check(Arena);
}

void ABladeIIGameGameModeBase::SetupDealer()
{
	Dealer = new B2Dealer();
	Dealer->Arena = Arena;
}

void ABladeIIGameGameModeBase::InitialiseBoard(B2BoardState BoardState)
{
	// Test state

	// Player Deck
	for (int i = 14; i >= 0; --i)
	{
		FB2Transform CardTransform = Arena->PlayerDeck->GetTransformForIndex(i);
		ACard* Card = CardFactory->Make(static_cast<ECard>(FMath::RandRange(0, 10)), CardTransform.Position + FVector(28, 0, 5), CardTransform.Rotation);
		Arena->PlayerDeck->Add(Card);
	}

	// Opponent Deck
	for (int i = 14; i >= 0; --i)
	{
		FB2Transform CardTransform = Arena->OpponentDeck->GetTransformForIndex(i);
		ACard* Card = CardFactory->Make(static_cast<ECard>(FMath::RandRange(0, 10)), CardTransform.Position + FVector(-28, 0, 5), CardTransform.Rotation);
		Arena->OpponentDeck->Add(Card);
	}
}

void ABladeIIGameGameModeBase::HandleMoveReceived(FB2Move& Move)
{

}

void ABladeIIGameGameModeBase::HandleInstructionReceived(EInstruction& Instruction)
{

}
