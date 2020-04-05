#include "BladeIIGameGameMode.h"

#include "EngineUtils.h"
#include "UObject/UObjectGlobals.h"

#include "B2Misc/Utility.h"
#include "B2Engine/AIOpponent.h"
#include "B2Engine/NetOpponent.h"
#include "B2Engine/LaunchConfig.h"
#include "B2Misc/Transition.h"

const float OUT_OF_BOUNDS_OFFSET_X = 28;

void ABladeIIGameGameMode::Tick(float DeltaSeconds)
{
	if (EngineState > EEngineState::Initialisation)
	{
		Dealer->Tick(DeltaSeconds);
	}

	Opponent->Tick(DeltaSeconds);

	if (GameState.bAcceptPlayerInput)
	{
		if (!CardSelector->IsHidden())
		{
			EButton Button;
			while (LocalPlayerInput->ButtonInputQueue.Dequeue(Button))
			{
				// Filter out left and right momvement when the cursor is on the players deck
				if (GameState.CursorPosition->GetType() == ETableSlot::PlayerDeck && (Button == EButton::NavigateLeft || Button == EButton::NavigateRight))
				{
					continue;
				}

				switch (Button)
				{
				case EButton::Menu:
					// Handle menu open / close etc
					break;
				case EButton::NavigateLeft:
					// Handle nav left
					break;
				case EButton::NavigateRight:
					// handle nav right
					break;
				case EButton::Select:
					// Handle select logic
					break;
				}
			}
		}
	}

	LocalPlayerInput->ButtonInputQueue.Empty();
}

ABladeIIGameGameMode::ABladeIIGameGameMode(const FObjectInitializer& ObjectInitializer)
{
	DefaultPawnClass = ALocalPlayerInput::StaticClass();

	SetupLaunchConfig(ObjectInitializer);
	
	SetupCardFactory();

	EngineState = EEngineState::Initialisation;

	B2Utility::LogInfo("GameMode initialized");
}

void ABladeIIGameGameMode::StartPlay()
{
	Super::StartPlay();

	FindArena();

	SetupDealer();

	FindLocalPlayerInput();

	RegisterEventListeners();

	SetupSelector();

	B2Utility::LogInfo("ABladeIIGameGameModeBase::StartPlay");
}

void ABladeIIGameGameMode::SetupLaunchConfig(const FObjectInitializer& ObjectInitializer)
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

void ABladeIIGameGameMode::SetupCardFactory()
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

void ABladeIIGameGameMode::RegisterEventListeners()
{
	// Register event listeners

	// From Opponent
	Opponent->OnMoveReceived.AddDynamic(this, &ABladeIIGameGameMode::HandleMoveReceived);
	Opponent->OnInstructionReceived.AddDynamic(this, &ABladeIIGameGameMode::HandleInstructionReceived);
	Opponent->OnCardsReceived.AddDynamic(this, &ABladeIIGameGameMode::HandleCardsReceived);

	// From Dealer
	Dealer->EnterGamePlayState.AddDynamic(this, &ABladeIIGameGameMode::HandleEventUpdate);
}

void ABladeIIGameGameMode::FindArena()
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

void ABladeIIGameGameMode::FindLocalPlayerInput()
{
	// Try to get a reference to the local player input actor
	for (TActorIterator<ALocalPlayerInput> InputIter(GetWorld()); InputIter; ++InputIter)
	{
		if (InputIter)
		{
			LocalPlayerInput = *InputIter;
		}
	}

	// Throw if the input actor was not found
	check(LocalPlayerInput);
}

void ABladeIIGameGameMode::SetupDealer()
{
	Dealer = NewObject<UB2Dealer>(this, TEXT("Dealer"));
	Dealer->Arena = Arena;
}

void ABladeIIGameGameMode::SetupSelector()
{
	UObject* CardSelectorActor = StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/BladeIIGame/Blueprints/GameObjects/BP_CardSelector.BP_CardSelector"));
	ensureMsgf(CardSelectorActor, TEXT("Could not load card selector actor - static load failed"));

	UBlueprint* CardSelectorBlueprint = Cast<UBlueprint>(CardSelectorActor);
	ensureMsgf(CardSelectorBlueprint, TEXT("Could not cast UObject to UBlueprint"));

	UClass* CardSelectorClass = CardSelectorBlueprint->GeneratedClass;
	ensureMsgf(CardSelectorClass, TEXT("Could not get class from card selector blueprint"));

	CardSelector = GetWorld()->SpawnActor<ACardSelector>(CardSelectorClass, FVector::ZeroVector, FRotator::ZeroRotator);
}

void ABladeIIGameGameMode::InitialiseBoard(B2GameState State)
{
	GameState = State;

	// Player Deck
	for (int i = 0; i < GameState.Cards.PlayerDeck.Num(); ++i)
	{
		FB2Transform CardTransform = Arena->PlayerDeck->GetTransformForIndex(i);
		ACard* Card = CardFactory->Make(GameState.Cards.PlayerDeck[i], CardTransform.Position + FVector(OUT_OF_BOUNDS_OFFSET_X, 0, 5), CardTransform.Rotation);
		Arena->PlayerDeck->Add(Card);
	}

	// Opponent Deck
	for (int i = 0; i < GameState.Cards.OpponentDeck.Num(); ++i)
	{
		FB2Transform CardTransform = Arena->OpponentDeck->GetTransformForIndex(i);
		ACard* Card = CardFactory->Make(GameState.Cards.OpponentDeck[i], CardTransform.Position + FVector(-OUT_OF_BOUNDS_OFFSET_X, 0, 5), CardTransform.Rotation);
		Arena->OpponentDeck->Add(Card); 
	}
}

void ABladeIIGameGameMode::EnterGamePlayState()
{
	FVector SelectorStartingPosition = Arena->PlayerDeck->GetTransformForIndex(Arena->PlayerDeck->Size() - 1).Position;

	CardSelector->SetActorLocationAndRotation(SelectorStartingPosition, FRotator::ZeroRotator);
	CardSelector->ToggleActorVisibility(true);

	GameState.bAcceptPlayerInput = true;
	GameState.CursorPosition = Arena->PlayerDeck;
}

void ABladeIIGameGameMode::HandleCardsReceived(const FB2Cards& Cards)
{
	B2GameState State = B2GameState
	{
		Cards /* Cards for this game */,
		0 /* Player Score */,
		0 /* Opponent Score */,
		false /* (Not) accepting player input */ ,
	};

	InitialiseBoard(State);

	EngineState = EEngineState::Dealing;

	Dealer->Deal();
}

void ABladeIIGameGameMode::HandleMoveReceived(const FB2Move& Move)
{

}

void ABladeIIGameGameMode::HandleInstructionReceived(EInstruction Instruction)
{

}

void ABladeIIGameGameMode::HandleEventUpdate(EDealerEvent Event)
{
	switch (Event)
	{
	case EDealerEvent::CardsDealt:
		EnterGamePlayState();
		break;
	case EDealerEvent::CardPlaced:

		break;
	}
}
