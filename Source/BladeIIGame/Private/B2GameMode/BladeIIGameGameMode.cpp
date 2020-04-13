#include "B2GameMode/BladeIIGameMode.h"

#include "EngineUtils.h"
#include "UObject/UObjectGlobals.h"

#include "B2GameMode/BladeIIGameInstance.h"
#include "B2Engine/AIOpponent.h"
#include "B2Engine/NetOpponent.h"
#include "B2Engine/LaunchConfig.h"
#include "B2Engine/Transition.h"
#include "B2Utility/Log.h"

// Game phase state machines
#include "B2Engine/GameStateMachine/GSM_State_DrawToEmptyField.h"
#include "B2Engine/GameStateMachine/GSM_State_WaitingForInitialDeal.h"
#include "B2Engine/GameStateMachine/GSM_State_PlayerTurn.h"
#include "B2Engine/GameStateMachine/GSM_State_WaitingForOpponentMove.h"
#include "B2Engine/GameStateMachine/GSM_State_PlayerRod.h"
#include "B2Engine/GameStateMachine/GSM_State_PlayerBolt.h"
#include "B2Engine/GameStateMachine/GSM_State_PlayerMirror.h"
#include "B2Engine/GameStateMachine/GSM_State_PlayerBlast.h"
#include "B2Engine/GameStateMachine/GSM_State_PlayerBlastTarget.h"
#include "B2Engine/GameStateMachine/GSM_State_PlayerForce.h"


const float OUT_OF_BOUNDS_OFFSET_X = 28;

ABladeIIGameMode::ABladeIIGameMode(const FObjectInitializer& ObjectInitializer)
{
	EngineState = EEngineState::Initialisation;

	DefaultPawnClass = ALocalPlayerInput::StaticClass();

	SetupLaunchConfig(ObjectInitializer);

	CreateGSM();

	B2Utility::LogInfo("GameMode initialized");
}

void ABladeIIGameMode::Tick(float DeltaSeconds)
{
	if (EngineState > EEngineState::Initialisation)
	{
		Dealer->Tick(DeltaSeconds);
	}

	Opponent->Tick(DeltaSeconds);

	if (EngineState >= EEngineState::InPlay)
	{
		GSM->Tick(DeltaSeconds);
	}

	LocalPlayerInput->ButtonInputQueue.Empty();
	UIEffectLayer->EventQueue.Empty();
}

void ABladeIIGameMode::FinishTurn()
{
	UpdateCardState();
	
	FString Turn = GameState->Turn == EPlayer::Player ? TEXT("Player's") : TEXT("Opponent's");

	B2Utility::LogWarning(FString::Printf(TEXT("[%s] turn finished"), *Turn));
}

void ABladeIIGameMode::StartPlay()
{
	Super::StartPlay();

	FindArena();

	SetupCardFactory();

	SetupDealer();

	SetupSelector();

	SetupUIEffectLayer();

	FindLocalPlayerInput();

	RegisterEventListeners();

	B2Utility::LogWarning(FString::Printf(TEXT("%f"), FVector::Dist(FVector(-25.668430, -17.709171, 1.045005), FVector(-29.668430, -18.179813, 1.045005))));
}

void ABladeIIGameMode::SetupLaunchConfig(const FObjectInitializer& ObjectInitializer)
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

void ABladeIIGameMode::SetupCardFactory()
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

	CardFactory = new B2CardFactory(B2CardFactoryConfig, Arena);
}

void ABladeIIGameMode::CreateGSM()
{
	GSM = new B2GameStateMachine(this);
	GSM->ChangeState<GSM_State_WaitingForInitialDeal>();
}

void ABladeIIGameMode::RegisterEventListeners()
{
	// Register event listeners

	// From Opponent
	Opponent->OnServerUpdateReceived.AddDynamic(this, &ABladeIIGameMode::HandleServerUpdate);
	Opponent->OnCardsReceived.AddDynamic(this, &ABladeIIGameMode::HandleCardsReceived);

	// From Dealer
	Dealer->OnDealerEvent.AddDynamic(this, &ABladeIIGameMode::HandleDealerEvent);
}

void ABladeIIGameMode::FindArena()
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

void ABladeIIGameMode::FindLocalPlayerInput()
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

void ABladeIIGameMode::SetupDealer()
{
	Dealer = NewObject<UB2Dealer>(this, TEXT("Dealer"));
	Dealer->Arena = Arena;
}

void ABladeIIGameMode::SetupSelector()
{
	UObject* CardSelectorActor = StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/BladeIIGame/Blueprints/GameObjects/BP_Card_Cursor.BP_Card_Cursor'"));
	ensureMsgf(CardSelectorActor, TEXT("Could not load card selector actor - static load failed"));

	UBlueprint* CardSelectorBlueprint = Cast<UBlueprint>(CardSelectorActor);
	ensureMsgf(CardSelectorBlueprint, TEXT("Could not cast UObject to UBlueprint"));

	UClass* CardSelectorClass = CardSelectorBlueprint->GeneratedClass;
	ensureMsgf(CardSelectorClass, TEXT("Could not get class from card selector blueprint"));

	Cursor = GetWorld()->SpawnActor<ACardSelector>(CardSelectorClass, FVector::ZeroVector, FRotator::ZeroRotator);
}

void ABladeIIGameMode::SetupUIEffectLayer()
{
	UIEffectLayer = NewObject<UB2UIEffectLayer>(this, TEXT("UI Effect Layer"));

	UIEffectLayer->Initialise();
}

void ABladeIIGameMode::InitialiseBoard()
{
	// Player Deck
	for (int i = 0; i < GameState->Cards.PlayerDeck.Num(); ++i)
	{
		FB2Transform CardTransform = Arena->PlayerDeck->GetTransformForIndex(i);
		ACard* Card = CardFactory->Make(GameState->Cards.PlayerDeck[i], CardTransform.Position + FVector(OUT_OF_BOUNDS_OFFSET_X, 0, 5), CardTransform.Rotation);
		Arena->PlayerDeck->Add(Card);
	}

	// Opponent Deck
	for (int i = 0; i < GameState->Cards.OpponentDeck.Num(); ++i)
	{
		FB2Transform CardTransform = Arena->OpponentDeck->GetTransformForIndex(i);
		ACard* Card = CardFactory->Make(GameState->Cards.OpponentDeck[i], CardTransform.Position + FVector(-OUT_OF_BOUNDS_OFFSET_X, 0, 5), CardTransform.Rotation);
		Arena->OpponentDeck->Add(Card); 
	}
}

void ABladeIIGameMode::OnCardsDealt()
{
	FVector SelectorStartingPosition = Arena->PlayerDeck->GetTransformForIndex(Arena->PlayerDeck->Num() - 1).Position;

	Cursor->SetActorLocationAndRotation(SelectorStartingPosition, FRotator::ZeroRotator);
	Cursor->ToggleActorVisibility(true);

	GameState->bAcceptPlayerInput = true;
	GameState->CursorPosition = ECardSlot::PlayerDeck;

	GSM->ChangeState<GSM_State_DrawToEmptyField>();

	EngineState = EEngineState::InPlay;
}

void ABladeIIGameMode::OnEffectReady()
{
	// Blast edge cases -  we should switch to the blast select state from the blast state, or set the bBlastAnimationPending flag from
	// the blast select state
	if (GSM->IsCurrentState(EGameState::PlayerBlast))
	{
		// Switch to player blast target state
		GSM->ChangeState<GSM_State_PlayerBlastTarget>();

		return;
	}
	else if (GSM->IsCurrentState(EGameState::OpponentBlast))
	{
		// Switch to opponent blast target state

		return;
	}
	else if (GSM->IsCurrentState(EGameState::OpponentBlastTarget) || GSM->IsCurrentState(EGameState::PlayerBlastTarget))
	{
		GameState->bBlastAnimationPending = true;
		return;
	}

	ACard* Card = GetCardSlot(GameState->CursorPosition)->GetCardByIndex(GameState->CursorSlotIndex);
	ECard Type = Card->Type;

	switch (Type)
	{
	case ECard::ElliotsOrbalStaff:
		if (GameState->Turn == EPlayer::Player)
		{
			// Switch state machine to player rod
			GSM->ChangeState<GSM_State_PlayerRod>();
		}
		else
		{

		}
		break;
	case ECard::Bolt:
		if (GameState->Turn == EPlayer::Player)
		{
			// Switch state machine to player bolt
			GSM->ChangeState<GSM_State_PlayerBolt>();
		}
		else
		{

		}
		break;
	case ECard::Mirror:
		if (GameState->Turn == EPlayer::Player)
		{
			// Switch state machine to player force
			GSM->ChangeState<GSM_State_PlayerMirror>();
		}
		else
		{

		}
		break;
	case ECard::Blast:
		GameState->MostRecentBlastCardID = Card->GetID();

		if (GameState->Turn == EPlayer::Player)
		{
			// Switch state machine to player blast
			GSM->ChangeState<GSM_State_PlayerBlast>();
		}
		else
		{

		}
		break;
	case ECard::Force:
		if (GameState->Turn == EPlayer::Player)
		{
			// Switch state machine to player force
			GSM->ChangeState<GSM_State_PlayerForce>();
		}
		else
		{

		}
		break;
	}
}

void ABladeIIGameMode::OnCardPlaced()
{
	if (GSM->IsCurrentState(EGameState::DrawToEmptyField))
	{
		Dealer->FlipFieldCard(Arena->PlayerField->GetLast(), false);

		// Change state to the turn of the player with the highest score, or each draw another on draw
		if (GameState->PlayerScore == GameState->OpponentScore)
		{
			// Edge case - what do we do if the deck(s) are empty?
			uint32 PlayerDeckSize = Arena->PlayerDeck->Num();
			uint32 OpponentDeckSize = Arena->OpponentDeck->Num();

			if (PlayerDeckSize == 0 || OpponentDeckSize == 0)
			{
				B2Utility::LogWarning("Either the player deck, or the opponent deck is empty!");

				// TODO do we end the game or something? Probably send it to end state...
				// TODO 2 looks like you just start drawing from the hand. do we get to choose? i guess so

				return;
			}

			// Lets go round again.wav
			GameState->Turn = EPlayer::Undecided;

			// Switch state machine to drawing from empty field
			GSM->ChangeState<GSM_State_DrawToEmptyField>();
		}
		else if (GameState->PlayerScore > GameState->OpponentScore)
		{
			// Switch game state to player turn
			GameState->Turn = EPlayer::Player;

			// Switch state machine to player turn
			GSM->ChangeState<GSM_State_PlayerTurn>();
		}
		else
		{
			//// Switch game state to opponent
			//GameState->Turn = EPlayer::Opponent;

			//// Switch state machine to opponent turn
			//GSM->ChangeState<GSM_State_WaitingForOpponentMove>();

			//// Fire off any animations / on screen stuff...

			// For testing
			// TODO remove and revert to above code
			// Switch game state to player turn
			GameState->Turn = EPlayer::Player;

			// Switch state machine to player turn
			GSM->ChangeState<GSM_State_PlayerTurn>();
		}
	}
}

int32 ABladeIIGameMode::AggregateScore(UCardSlot* Slot) const
{
	int32 Total = 0;

	for (size_t i = 0; i < Slot->Num(); i++)
	{
		ACard* Card = Slot->GetCardByIndex(i);

		if (Card->IsActive())
		{
			if (Card->Type == ECard::Force && i > 0)
			{
				Total *= 2;
			}
			else if (Card->Type >= ECard::Bolt)
			{
				Total += 1;
			}
			else
			{
				Total += (static_cast<uint32>(Card->Type) + 1);
			}
		}
	}

	return Total;
}

void ABladeIIGameMode::UpdateCardState()
{
	// Player Deck
	GameState->Cards.PlayerDeck.Empty(Arena->PlayerDeck->Num());
	for (size_t i = 0; i < Arena->PlayerDeck->Num(); i++)
	{
		GameState->Cards.PlayerDeck.Add(Arena->PlayerDeck->GetCardByIndex(i)->Type);
	}

	// Player Hand
	GameState->Cards.PlayerHand.Empty(Arena->PlayerHand->Num());
	for (size_t i = 0; i < Arena->PlayerHand->Num(); i++)
	{
		GameState->Cards.PlayerHand.Add(Arena->PlayerHand->GetCardByIndex(i)->Type);
	}

	// Player Field
	GameState->Cards.PlayerField.Empty(Arena->PlayerField->Num());
	for (size_t i = 0; i < Arena->PlayerField->Num(); i++)
	{
		GameState->Cards.PlayerField.Add(Arena->PlayerField->GetCardByIndex(i)->Type);
	}

	// Player Discard
	GameState->Cards.PlayerDiscard.Empty(Arena->PlayerDiscard->Num());
	for (size_t i = 0; i < Arena->PlayerDiscard->Num(); i++)
	{
		GameState->Cards.PlayerDiscard.Add(Arena->PlayerDiscard->GetCardByIndex(i)->Type);
	}

	// Opponent Deck
	GameState->Cards.OpponentDeck.Empty(Arena->OpponentDeck->Num());
	for (size_t i = 0; i < Arena->OpponentDeck->Num(); i++)
	{
		GameState->Cards.OpponentDeck.Add(Arena->OpponentDeck->GetCardByIndex(i)->Type);
	}

	// Opponent Hand
	GameState->Cards.OpponentHand.Empty(Arena->OpponentHand->Num());
	for (size_t i = 0; i < Arena->OpponentHand->Num(); i++)
	{
		GameState->Cards.OpponentHand.Add(Arena->OpponentHand->GetCardByIndex(i)->Type);
	}

	// Opponent Field
	GameState->Cards.OpponentField.Empty(Arena->OpponentField->Num());
	for (size_t i = 0; i < Arena->OpponentField->Num(); i++)
	{
		GameState->Cards.OpponentField.Add(Arena->OpponentField->GetCardByIndex(i)->Type);
	}

	// Opponent Discard
	GameState->Cards.OpponentDiscard.Empty(Arena->OpponentDiscard->Num());
	for (size_t i = 0; i < Arena->OpponentDiscard->Num(); i++)
	{
		GameState->Cards.OpponentDiscard.Add(Arena->OpponentDiscard->GetCardByIndex(i)->Type);
	}

	// Update scores
	GameState->PlayerScore = AggregateScore(Arena->PlayerField);
	GameState->OpponentScore = AggregateScore(Arena->OpponentField);

	Arena->ScoreDisplay->Update(GameState->PlayerScore, GameState->OpponentScore);
}

UCardSlot* ABladeIIGameMode::GetCardSlot(ECardSlot Slot) const
{
	UCardSlot* CardSlot = nullptr;

	switch (Slot)
	{
	case ECardSlot::PlayerDeck:
		CardSlot = Arena->PlayerDeck;
		break;
	case ECardSlot::PlayerHand:
		CardSlot = Arena->PlayerHand;
		break;
	case ECardSlot::PlayerField:
		CardSlot = Arena->PlayerField;
		break;
	case ECardSlot::PlayerDiscard:
		// TODO impl?
		B2Utility::LogWarning("PlayerDiscard does not exist!!!");
		break;
	case ECardSlot::OpponentDeck:
		CardSlot = Arena->OpponentDeck;
		break;
	case ECardSlot::OpponentHand:
		CardSlot = Arena->OpponentHand;
		break;
	case ECardSlot::OpponentField:
		CardSlot = Arena->OpponentField;
		break;
	case ECardSlot::OpponentDiscard:
		// TODO impl?
		B2Utility::LogWarning("OpponentDiscard does not exist!!!");
		break;
	}

	return CardSlot;
}

void ABladeIIGameMode::HandleCardsReceived(const FB2Cards& Cards)
{
	GameState = new B2GameState(Cards);

	InitialiseBoard();

	EngineState = EEngineState::Dealing;

	 //Dealer->Deal();
	Dealer->FastDeal();
}

void ABladeIIGameMode::HandleServerUpdate(EServerUpdate Update, const FString& MetaData)
{

}

void ABladeIIGameMode::HandleDealerEvent(EDealerEvent Event)
{
	UpdateCardState();

	switch (Event)
	{
	case EDealerEvent::CardsDealt:
		OnCardsDealt();
		break;
	case EDealerEvent::CardPlaced:
		OnCardPlaced();
		break;
	case EDealerEvent::EffectReady:
		OnEffectReady();
		break;
	case EDealerEvent::BlastFinished:
		FinishTurn();
		break;
	}
}