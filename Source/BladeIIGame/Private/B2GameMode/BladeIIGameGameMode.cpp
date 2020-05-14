#include "B2GameMode/BladeIIGameMode.h"

#include "EngineUtils.h"
#include "UObject/UObjectGlobals.h"
#include "TimerManager.h"

#include "B2GameMode/BladeIIGameInstance.h"
#include "B2Engine/AIOpponent.h"
#include "B2Engine/NetOpponent.h"
#include "B2Engine/LaunchConfig.h"
#include "B2Engine/Transition.h"
#include "B2Utility/Log.h"

// Game state state machines
#include "B2Engine/GameStateMachine/GSM_State_DrawToEmptyField.h"
#include "B2Engine/GameStateMachine/GSM_State_WaitingForInitialDeal.h"
#include "B2Engine/GameStateMachine/GSM_State_ProcessBoardState.h"
#include "B2Engine/GameStateMachine/GSM_State_PostGame.h"

#include "B2Engine/GameStateMachine/GSM_State_PlayerTurn.h"
#include "B2Engine/GameStateMachine/GSM_State_PlayerRod.h"
#include "B2Engine/GameStateMachine/GSM_State_PlayerBolt.h"
#include "B2Engine/GameStateMachine/GSM_State_PlayerMirror.h"
#include "B2Engine/GameStateMachine/GSM_State_PlayerBlast.h"
#include "B2Engine/GameStateMachine/GSM_State_PlayerBlastTarget.h"
#include "B2Engine/GameStateMachine/GSM_State_PlayerForce.h"

#include "B2Engine/GameStateMachine/GSM_State_OpponentTurn.h"
#include "B2Engine/GameStateMachine/GSM_State_OpponentRod.h"
#include "B2Engine/GameStateMachine/GSM_State_OpponentBolt.h"
#include "B2Engine/GameStateMachine/GSM_State_OpponentMirror.h"
#include "B2Engine/GameStateMachine/GSM_State_OpponentBlast.h"
#include "B2Engine/GameStateMachine/GSM_State_OpponentBlastTarget.h"
#include "B2Engine/GameStateMachine/GSM_State_OpponentForce.h"

const float OUT_OF_BOUNDS_OFFSET_X = 28;
const FString AVATAR_CAPTURE_RIG_BLUEPRINT_PATH = TEXT("Blueprint'/Game/BladeIIGame/Blueprints/GameObjects/BP_AvatarCaptureRig'");
const FString AVATAR_WIDGET_PATH = TEXT("WidgetBlueprint'/Game/BladeIIGame/Blueprints/UI/BP_Avatar'");
const FVector AVATAR_CAPTURE_RIG_SPAWN_LOCATION = FVector(500, 0, 0);
const FString STATUS_INDICATOR_WIDGET_PATH = TEXT("WidgetBlueprint'/Game/BladeIIGame/Blueprints/UI/BP_StatusIndicator'");
const FString LOADING_SCREEN_WIDGET_PATH = TEXT("WidgetBlueprint'/Game/BladeIIGame/Blueprints/UI/BP_LoadingScreen'");
const FString TOOLTIP_PANEL_WIDGET_PATH = TEXT("WidgetBlueprint'/Game/BladeIIGame/Blueprints/UI/BP_ToolTipPanel'");
const FString OPTIONS_MENU_WIDGET_PATH = TEXT("WidgetBlueprint'/Game/BladeIIGame/Blueprints/UI/BP_OptionsMenu'");
const FString ERROR_MODAL_WIDGET_PATH = TEXT("WidgetBlueprint'/Game/BladeIIGame/Blueprints/UI/BP_ErrorModal'");
const FString CARD_CURSOR_BLUEPRINT_PATH = TEXT("Blueprint'/Game/BladeIIGame/Blueprints/GameObjects/BP_Card_Cursor'");
const FString LOCAL_QUIT_METADATA = TEXT("LOCAL_QUIT");
const FString TITLE_BAR_BLUEPRINT_PATH = TEXT("WidgetBlueprint'/Game/BladeIIGame/Blueprints/UI/BP_Titlebar'");

// TODO set to zero for build
#define FAST_DRAW 1

ABladeIIGameMode::ABladeIIGameMode(const FObjectInitializer& ObjectInitializer)
{
	EngineState = EEngineState::Initialisation;

	DefaultPawnClass = ALocalPlayerInput::StaticClass();

	SetupLaunchConfig(ObjectInitializer);

	SetupCardFactory(ObjectInitializer);

	CreateGSM();

	GetUIAvatarWidgetClass();

	GetUIStatusIndicatorClass();

	GetUIToolTipPanelLayer();

	GetUILoadingScreenWidgetClass();

	GetUIOptionsMenuWidgetClass();

	GetUIErrorModalWidgetClass();

	GetUITitleBarWidgetClass();

	GetCursorClass();

	GetAvatarCaptureRigClass();

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

void ABladeIIGameMode::EndState()
{
	UpdateCardState();

	B2Utility::LogInfo(FString::Printf(TEXT("[%d] state ended"), GSM->GetCurrentState()));

	// Switch to the turn switching state
	GSM->ChangeState<GSM_State_ProcessBoardState>();
}

void ABladeIIGameMode::EndGame(EPlayer Victor, EWinCondition WinCondition)
{
	// Handle draws
	if (Victor == EPlayer::Undecided)
	{
		UIAvatarLayer->SetOpponentMessage(EOpponentMessage::TiedGame, AvatarCaptureRig->GetCurrentCharacterName());

		// Play defeat sound effect
		GameSound->PlaySFX(ESFX::Defeat);

		// Play animation
		UIEffectLayer->Play(EUIEffect::Draw, nullptr);
	}
	else
	{
		FString Turn;
		if (Victor == EPlayer::Player)
		{
			Turn = TEXT("Local Player");
			UIAvatarLayer->SetOpponentMessage(EOpponentMessage::Defeat, AvatarCaptureRig->GetCurrentCharacterName());

			// Play victory sound effect
			GameSound->PlaySFX(ESFX::Victory);

			// Play animation
			UIEffectLayer->Play(EUIEffect::Victory, nullptr);
		}
		else if (Victor == EPlayer::Opponent)
		{
			Turn = TEXT("Opponent");
			UIAvatarLayer->SetOpponentMessage(EOpponentMessage::Victory, AvatarCaptureRig->GetCurrentCharacterName());

			// Play defeat sound effect
			GameSound->PlaySFX(ESFX::Defeat);

			// Play animation
			UIEffectLayer->Play(EUIEffect::Defeat, nullptr);
		}

		B2Utility::LogInfo(FString::Printf(TEXT("[%s] Has won ~ Condition [ %d ]"), *Turn, WinCondition));
	}

	AvatarCaptureRig->AnimateMouth();

	if (Arena->OpponentHand->Num() > 0)
	{
		Dealer->RevealOpponentsHand();
	}

	UIStatusIndicatorLayer->SetState(UStatusIndicator::State::GameOver);

	LocalPlayerInput->BlockInputs();

	Opponent->Deactivate();

	UIToolTipPanelLayer->SetActive(false);

	EngineState = EEngineState::PostGame;

	GSM->ChangeState<GSM_State_PostGame>();
}

void ABladeIIGameMode::ChangeTurn()
{
	// If its currently the local players turn, switch to the opponents turn
	if (GameState->Turn == EPlayer::Player)
	{
		GameState->Turn = EPlayer::Opponent;
		GSM->ChangeState<GSM_State_OpponentTurn>();
	}
	else
	{
		GameState->Turn = EPlayer::Player;
		GSM->ChangeState<GSM_State_PlayerTurn>();
	}
}

void ABladeIIGameMode::ContinueTurn()
{
	// Determine whos turn it currently is, then switch to that state again
	if (GameState->Turn == EPlayer::Player)
	{
		GSM->ChangeState<GSM_State_PlayerTurn>();
	}
	else
	{
		GSM->ChangeState<GSM_State_OpponentTurn>();
	}
}

void ABladeIIGameMode::ClearAndDraw(float Delay)
{
	if (Delay > 0)
	{
		if (GetWorldTimerManager().TimerExists(ClearAndDrawHandle))
		{
			GetWorldTimerManager().ClearTimer(ClearAndDrawHandle);
		}

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this]() { this->ClearAndDraw(0); });

		GetWorldTimerManager().SetTimer(ClearAndDrawHandle, TimerDelegate, Delay, false);

		return;
	}

	GameState->Turn = EPlayer::Undecided;

	// Switch state machine to drawing from empty field
	GSM->ChangeState<GSM_State_DrawToEmptyField>();
}

#include "B2Predicate/IsNotEffectCard.h"

void ABladeIIGameMode::StartPlay()
{
	Super::StartPlay();

	bOtherDelayedStartComponentReady = false;

	Settings->ApplyAll();

	ConnectionProgress = 0;
	MatchPrepProgress = 0;

	FindArena();

	FindCamera();

	FindLocalPlayerInput();

	FindGameSoundActor();

	InitialiseCardFactory();

	SetupDealer();

	SetupSelector();

	SetupUIEffectLayer();

	SetupUIAvatarLayer();

	SetupUIStatusIndicatorLayer();

	SetupUIToolTipPanelLayer();

	SetupUILoadingScreenLayer();

	SetupUIOptionsMenuLayer();

	SetupUIErrorModalLayer();

	SetupUITitleBarLayer();

	SetupAvatarCaptureRig();

	RegisterEventListeners();

	InitialiseOpponent();
}

void ABladeIIGameMode::SetupLaunchConfig(const FObjectInitializer& ObjectInitializer)
{
	// Read the launch config
	B2LaunchConfig LaunchConfig("Launch.conf");

	// Create the opponent class based launch config
	if (LaunchConfig.MatchID <= B2LaunchConfig::MATCH_ID_AI_GAME_THRESHOLD)
	{
		UB2AIOpponent* AIOpponent = ObjectInitializer.CreateDefaultSubobject<UB2AIOpponent>(this, TEXT("AI Opponent"));
		Opponent = static_cast<UB2Opponent*>(AIOpponent);
	}
	else
	{
		UB2NetOpponent* NetOpponent = ObjectInitializer.CreateDefaultSubobject<UB2NetOpponent>(this, TEXT("Net Opponent"));
		Opponent = static_cast<UB2Opponent*>(NetOpponent);
	}

	// Set up the settings object
	Settings = ObjectInitializer.CreateDefaultSubobject<USettings>(this, TEXT("Settings"));
	Settings->Initialise(this, LaunchConfig);

	// Set the local player public ID in the player data object
	PlayerData.LocalPlayer.PublicID = LaunchConfig.PublicID;
}

void ABladeIIGameMode::SetupCardFactory(const FObjectInitializer& ObjectInitializer)
{
	CardFactory = ObjectInitializer.CreateDefaultSubobject<UB2CardFactory>(this, TEXT("Card Factory"));
}

void ABladeIIGameMode::InitialiseCardFactory()
{
	// Load card config (textures to use, etc)
	B2CardFactoryConfig B2CardFactoryConfig;

	B2CardFactoryConfig.CardFrontPaths = TArray<FString>({
		/* Basic cards */
		TEXT("/Game/BladeIIGame/Textures/Cards/T_Card_Staff.T_Card_Staff"),
		TEXT("/Game/BladeIIGame/Textures/Cards/T_Card_Gunswords.T_Card_Gunswords"),
		TEXT("/Game/BladeIIGame/Textures/Cards/T_Card_Bow.T_Card_Bow"),
		TEXT("/Game/BladeIIGame/Textures/Cards/T_Card_Sword.T_Card_Sword"),
		TEXT("/Game/BladeIIGame/Textures/Cards/T_Card_Shotgun.T_Card_Shotgun"),
		TEXT("/Game/BladeIIGame/Textures/Cards/T_Card_Spear.T_Card_Spear"),
		TEXT("/Game/BladeIIGame/Textures/Cards/T_Card_Broadsword.T_Card_Broadsword"),

		/* Special cards */
		TEXT("/Game/BladeIIGame/Textures/Cards/T_Card_Bolt.T_Card_Bolt"),
		TEXT("/Game/BladeIIGame/Textures/Cards/T_Card_Mirror.T_Card_Mirror"),
		TEXT("/Game/BladeIIGame/Textures/Cards/T_Card_Blast.T_Card_Blast"),
		TEXT("/Game/BladeIIGame/Textures/Cards/T_Card_Force.T_Card_Force"),
	});

	/* Back and MRS (metallic, roughness, specular) paths */
	B2CardFactoryConfig.CardBackPath = TEXT("/Game/BladeIIGame/Textures/Cards/T_Card_Back.T_Card_Back");
	B2CardFactoryConfig.CardFrontMRSPath = TEXT("/Game/BladeIIGame/Textures/Cards/T_Card_Front_MRS.T_Card_Front_MRS");
	B2CardFactoryConfig.CardBackMRSPath = TEXT("/Game/BladeIIGame/Textures/Cards/T_Card_Back_MRS.T_Card_Back_MRS");

	/* Pointer to current world */
	B2CardFactoryConfig.World = GetWorld();

	CardFactory->Initialise(B2CardFactoryConfig, Arena);
}

void ABladeIIGameMode::CreateGSM()
{
	GSM = new B2GameStateMachine(this);
	GSM->ChangeState<GSM_State_WaitingForInitialDeal>();
}

void ABladeIIGameMode::GetUIAvatarWidgetClass()
{
	ConstructorHelpers::FClassFinder<UAvatar> ClassFinder(*AVATAR_WIDGET_PATH);
	if (ensureMsgf(ClassFinder.Succeeded(), TEXT("Could not find the class for the avatar widget")))
	{
		UIAvatarWidgetClass = ClassFinder.Class;
	}
}

void ABladeIIGameMode::GetUIStatusIndicatorClass()
{
	ConstructorHelpers::FClassFinder<UStatusIndicator> ClassFinder(*STATUS_INDICATOR_WIDGET_PATH);
	if (ensureMsgf(ClassFinder.Succeeded(), TEXT("Could not find the class for the status indicator widget")))
	{
		UIStatusIndicatorWidgetClass = ClassFinder.Class;
	}
}

void ABladeIIGameMode::GetUIToolTipPanelLayer()
{
	ConstructorHelpers::FClassFinder<UToolTipPanel> ClassFinder(*TOOLTIP_PANEL_WIDGET_PATH);
	if (ensureMsgf(ClassFinder.Succeeded(), TEXT("Could not find the class for the tooltip panel widget")))
	{
		UIToolTipPanelWidgetClass = ClassFinder.Class;
	}
}

void ABladeIIGameMode::GetUILoadingScreenWidgetClass()
{
	ConstructorHelpers::FClassFinder<ULoadingScreen> ClassFinder(*LOADING_SCREEN_WIDGET_PATH);
	if (ensureMsgf(ClassFinder.Succeeded(), TEXT("Could not find the class for the loading screen widget")))
	{
		UILoadingScreenWidgetClass = ClassFinder.Class;
	}
}

void ABladeIIGameMode::GetUIOptionsMenuWidgetClass()
{
	ConstructorHelpers::FClassFinder<UOptionsMenu> ClassFinder(*OPTIONS_MENU_WIDGET_PATH);
	if (ensureMsgf(ClassFinder.Succeeded(), TEXT("Could not find the class for the options menu widget")))
	{
		UIOptionsMenuWidgetClass = ClassFinder.Class;
	}
}

void ABladeIIGameMode::GetUIErrorModalWidgetClass()
{
	ConstructorHelpers::FClassFinder<UErrorModal> ClassFinder(*ERROR_MODAL_WIDGET_PATH);
	if (ensureMsgf(ClassFinder.Succeeded(), TEXT("Could not find the class for the error modal widget")))
	{
		UIErrorModalWidgetClass = ClassFinder.Class;
	}
}

void ABladeIIGameMode::GetUITitleBarWidgetClass()
{
	ConstructorHelpers::FClassFinder<UUserWidget> ClassFinder(*TITLE_BAR_BLUEPRINT_PATH);
	if (ensureMsgf(ClassFinder.Succeeded(), TEXT("Could not find the class for the title bar widget")))
	{
		UITitleBarClass = ClassFinder.Class;
	}
}

void ABladeIIGameMode::GetCursorClass()
{
	ConstructorHelpers::FClassFinder<ACardSelector> ClassFinder(*CARD_CURSOR_BLUEPRINT_PATH);
	if (ensureMsgf(ClassFinder.Succeeded(), TEXT("Could not find the class for the card cursor actor")))
	{
		CursorClass = ClassFinder.Class;
	}
}

void ABladeIIGameMode::GetAvatarCaptureRigClass()
{
	ConstructorHelpers::FClassFinder<AAvatarCaptureRig> ClassFinder(*AVATAR_CAPTURE_RIG_BLUEPRINT_PATH);
	if (ensureMsgf(ClassFinder.Succeeded(), TEXT("Could not find the class for the avatar capture rig actor")))
	{
		AvatarCaptureRigClass = ClassFinder.Class;
	}
}

void ABladeIIGameMode::RegisterEventListeners()
{
	// Register event listeners

	// From Opponent
	if (Opponent)
	{
		Opponent->OnInstructionReceived.AddDynamic(this, &ABladeIIGameMode::HandleServerInstruction);
		Opponent->OnCardsReceived.AddDynamic(this, &ABladeIIGameMode::HandleCardsReceived);
	}

	// From Dealer
	if (Dealer)
	{
		Dealer->OnDealerEvent.AddDynamic(this, &ABladeIIGameMode::HandleDealerEvent);
	}

	// From input actor
	if (LocalPlayerInput)
	{
		LocalPlayerInput->OnMenuButtonPressed.AddDynamic(this, &ABladeIIGameMode::HandleMenuButtonPressed);
	}

	// From Error modal
	if (UIErrorModalLayer)
	{
		UIErrorModalLayer->OnButtonPressed.AddDynamic(this, &ABladeIIGameMode::HandleErrorModalButtonPressed);
	}
}

void ABladeIIGameMode::FindArena()
{
	// Try to get a reference to the arena actor
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

void ABladeIIGameMode::FindCamera()
{
	// Try to get a reference to the camera actor
	for (TActorIterator<ACamera> CamerIter(GetWorld()); CamerIter; ++CamerIter)
	{
		if (CamerIter)
		{
			Camera = *CamerIter;
		}
	}

	// Throw if the camera was not found
	check(Camera);
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

void ABladeIIGameMode::FindGameSoundActor()
{
	// Try to get a reference to the game sound actor
	for (TActorIterator<AGameSound> GameSoundIter(GetWorld()); GameSoundIter; ++GameSoundIter)
	{
		if (GameSoundIter)
		{
			GameSound = *GameSoundIter;
		}
	}

	// Throw if the game sound actor was not found
	check(GameSound);
}

void ABladeIIGameMode::SetupDealer()
{
	Dealer = NewObject<UB2Dealer>(this, TEXT("Dealer"));
	Dealer->Arena = Arena;
}

void ABladeIIGameMode::SetupSelector()
{
	Cursor = GetWorld()->SpawnActor<ACardSelector>(CursorClass, FVector::ZeroVector, FRotator::ZeroRotator);
}

void ABladeIIGameMode::SetupUIEffectLayer()
{
	UIEffectLayer = NewObject<UB2UIEffectLayer>(this, TEXT("UI Effect Layer"));

	UIEffectLayer->Initialise();
}

void ABladeIIGameMode::SetupUIAvatarLayer()
{
	if (UIAvatarWidgetClass)
	{
		UIAvatarLayer = CreateWidget<UAvatar>(GetWorld()->GetGameInstance(), UIAvatarWidgetClass, TEXT("UI Avatar Layer"));
		if (UIAvatarLayer)
		{
			UIAvatarLayer->AddToPlayerScreen();
		}
	}
}

void ABladeIIGameMode::SetupUIStatusIndicatorLayer()
{
	if (UIStatusIndicatorWidgetClass)
	{
		UIStatusIndicatorLayer = CreateWidget<UStatusIndicator>(GetWorld()->GetGameInstance(), UIStatusIndicatorWidgetClass, TEXT("UI Status Indicator Layer"));
		if (UIStatusIndicatorLayer)
		{
			UIStatusIndicatorLayer->AddToPlayerScreen();
		}
	}
}

void ABladeIIGameMode::SetupUIToolTipPanelLayer()
{
	if (UIToolTipPanelWidgetClass)
	{
		UIToolTipPanelLayer = CreateWidget<UToolTipPanel>(GetWorld()->GetGameInstance(), UIToolTipPanelWidgetClass, TEXT("UI ToolTip Panel Layer"));
		if (UIToolTipPanelLayer)
		{
			UIToolTipPanelLayer->AddToPlayerScreen();
			UIToolTipPanelLayer->SetLocalPlayerInput(LocalPlayerInput);
		}
	}
}

void ABladeIIGameMode::SetupUILoadingScreenLayer()
{
	if (UILoadingScreenWidgetClass)
	{
		UILoadingScreenLayer = CreateWidget<ULoadingScreen>(GetWorld()->GetGameInstance(), UILoadingScreenWidgetClass, TEXT("UI Loading Screen Layer"));
		if (UILoadingScreenLayer)
		{
			UILoadingScreenLayer->AddToPlayerScreen();
			UILoadingScreenLayer->Initialise(Settings->IsVersusAI());
		}
	}
}

void ABladeIIGameMode::SetupUIOptionsMenuLayer()
{
	if (UIOptionsMenuWidgetClass)
	{
		UIOptionsMenuLayer = CreateWidget<UOptionsMenu>(GetWorld()->GetGameInstance(), UIOptionsMenuWidgetClass, TEXT("UI Options Menu Layer"));
		if (UIOptionsMenuLayer)
		{
			UIOptionsMenuLayer->AddToPlayerScreen();
		}
	}
}

void ABladeIIGameMode::SetupUIErrorModalLayer()
{
	if (UIErrorModalWidgetClass)
	{
		UIErrorModalLayer = CreateWidget<UErrorModal>(GetWorld()->GetGameInstance(), UIErrorModalWidgetClass, TEXT("UI Error Modal Layer"));
		if (UIErrorModalLayer)
		{
			UIErrorModalLayer->AddToPlayerScreen();
		}
	}
}

void ABladeIIGameMode::SetupUITitleBarLayer()
{
	if (Settings)
	{
		if (Settings->GetIntSetting(EIntSetting::ScreenMode) == 2)
		{
			if (UITitleBarClass)
			{
				UITitleBarLayer = CreateWidget<UUserWidget>(GetWorld()->GetGameInstance(), UITitleBarClass, TEXT("UI Title Bar Layer"));
				if (UITitleBarLayer)
				{
					UITitleBarLayer->AddToPlayerScreen(1);
				}
			}
		}
	}
}

void ABladeIIGameMode::SetupAvatarCaptureRig()
{
	if (AvatarCaptureRigClass)
	{
		AvatarCaptureRig = GetWorld()->SpawnActor<AAvatarCaptureRig>(AvatarCaptureRigClass, AVATAR_CAPTURE_RIG_SPAWN_LOCATION, FRotator::ZeroRotator);
	}
}

void ABladeIIGameMode::InitialiseBoard()
{
	if (GameState)
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
}

void ABladeIIGameMode::InitialiseOpponent()
{
	if (Settings->IsVersusAI())
	{
		static_cast<UB2AIOpponent*>(Opponent)->Configure(static_cast<EAIDifficulty>(Settings->GetIntSetting(EIntSetting::MatchID)));

		AvatarCaptureRig->Initialise(AAvatarCaptureRig::Character::Laura);
	}
	else
	{
		uint64 MatchID = Settings->GetIntSetting(EIntSetting::MatchID);
		FString PublicID = Settings->GetStringSetting(EStringSetting::PublicID);
		FString AuthToken = Settings->GetStringSetting(EStringSetting::AuthToken);

		static_cast<UB2NetOpponent*>(Opponent)->Configure(PublicID, AuthToken, MatchID, GetWorld());
	}
}

void ABladeIIGameMode::DelayedStart()
{
	if (!bOtherDelayedStartComponentReady)
	{
		bOtherDelayedStartComponentReady = true;
	}
	else
	{
		InitialiseBoard();

		EngineState = EEngineState::Dealing;
			
		if (FAST_DRAW)
		{
			Dealer->FastDeal();
		}
		else
		{
			Dealer->Deal();
		}

		// Animate the opponent avatar
		UIAvatarLayer->SetOpponentMessage(EOpponentMessage::Greeting, AvatarCaptureRig->GetCurrentCharacterName());
		AvatarCaptureRig->AnimateMouth();

		// Start the BGM track
		GameSound->PlayBGM(0.2f);
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

	UIAvatarLayer->SetOpponentMessage(EOpponentMessage::Draw, AvatarCaptureRig->GetCurrentCharacterName());
	AvatarCaptureRig->AnimateMouth();
}

void ABladeIIGameMode::OnEffectReady()
{
	ACard* Card = GetCardSlot(GameState->CursorPosition)->GetCardByIndex(GameState->CursorSlotIndex);
	ECard Type = Card->Type;

	// "React" to the effect card if its the players turn
	if (GameState->Turn == EPlayer::Player && !GSM->IsCurrentState(EGameState::PlayerBlast))
	{
		// Changes the eyes to a random one
		AvatarCaptureRig->ChangeEyes();

		// Add reaction text to the opponent avatar and animate the mouth if the effect is not a rod
		if (Type != ECard::ElliotsOrbalStaff)
		{
			UIAvatarLayer->SetOpponentMessage(EOpponentMessage::Reaction, AvatarCaptureRig->GetCurrentCharacterName());
			AvatarCaptureRig->AnimateMouth();
		}
	}

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
		GSM->ChangeState<GSM_State_OpponentBlastTarget>();

		return;
	}
	else if (GSM->IsCurrentState(EGameState::OpponentBlastTarget) || GSM->IsCurrentState(EGameState::PlayerBlastTarget))
	{
		GameState->bBlastAnimationPending = true;
		return;
	}

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
			// Switch state machine to opponent rod
			GSM->ChangeState<GSM_State_OpponentRod>();
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
			// Switch state machine to opponent bolt
			GSM->ChangeState<GSM_State_OpponentBolt>();
		}
		break;
	case ECard::Mirror:
		if (GameState->Turn == EPlayer::Player)
		{
			// Switch state machine to player mirror
			GSM->ChangeState<GSM_State_PlayerMirror>();
		}
		else
		{
			// Switch state machine to opponent mirror
			GSM->ChangeState<GSM_State_OpponentMirror>();
		}
		break;
	case ECard::Blast:
		if (GameState->Turn == EPlayer::Player)
		{
			// Switch state machine to player blast
			GSM->ChangeState<GSM_State_PlayerBlast>();
		}
		else
		{
			// Switch state machine to opponent blast
			GSM->ChangeState<GSM_State_OpponentBlast>();
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
			// Switch state machine to player force
			GSM->ChangeState<GSM_State_OpponentForce>();
		}
		break;
	}
}

void ABladeIIGameMode::OnCardPlaced()
{
	EndState();
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
			else
			{
				Total += ACard::TypeToValue(Card->Type);
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
		ACard* Card = Arena->PlayerDeck->GetCardByIndex(i);
		ECard Type = Card->Type;
		if (!Card->IsActive())
		{
			Type = static_cast<ECard>(static_cast<uint32>(Type) + 11);
		}

		GameState->Cards.PlayerDeck.Add(Type);
	}

	// Player Hand
	GameState->Cards.PlayerHand.Empty(Arena->PlayerHand->Num());
	for (size_t i = 0; i < Arena->PlayerHand->Num(); i++)
	{
		ACard* Card = Arena->PlayerHand->GetCardByIndex(i);
		ECard Type = Card->Type;
		if (!Card->IsActive())
		{
			Type = static_cast<ECard>(static_cast<uint32>(Type) + 11);
		}

		GameState->Cards.PlayerHand.Add(Type);
	}

	// Player Field
	GameState->Cards.PlayerField.Empty(Arena->PlayerField->Num());
	for (size_t i = 0; i < Arena->PlayerField->Num(); i++)
	{
		ACard* Card = Arena->PlayerField->GetCardByIndex(i);
		ECard Type = Card->Type;
		if (!Card->IsActive())
		{
			Type = static_cast<ECard>(static_cast<uint32>(Type) + 11);
		}

		GameState->Cards.PlayerField.Add(Type);
	}

	// Player Discard
	GameState->Cards.PlayerDiscard.Empty(Arena->PlayerDiscard->Num());
	for (size_t i = 0; i < Arena->PlayerDiscard->Num(); i++)
	{
		ACard* Card = Arena->PlayerDiscard->GetCardByIndex(i);
		ECard Type = Card->Type;
		if (!Card->IsActive())
		{
			Type = static_cast<ECard>(static_cast<uint32>(Type) + 11);
		}

		GameState->Cards.PlayerDiscard.Add(Type);
	}

	// Opponent Deck
	GameState->Cards.OpponentDeck.Empty(Arena->OpponentDeck->Num());
	for (size_t i = 0; i < Arena->OpponentDeck->Num(); i++)
	{
		ACard* Card = Arena->OpponentDeck->GetCardByIndex(i);
		ECard Type = Card->Type;
		if (!Card->IsActive())
		{
			Type = static_cast<ECard>(static_cast<uint32>(Type) + 11);
		}

		GameState->Cards.OpponentDeck.Add(Type);
	}

	// Opponent Hand
	GameState->Cards.OpponentHand.Empty(Arena->OpponentHand->Num());
	for (size_t i = 0; i < Arena->OpponentHand->Num(); i++)
	{
		ACard* Card = Arena->OpponentHand->GetCardByIndex(i);
		ECard Type = Card->Type;
		if (!Card->IsActive())
		{
			Type = static_cast<ECard>(static_cast<uint32>(Type) + 11);
		}

		GameState->Cards.OpponentHand.Add(Type);
	}

	// Opponent Field
	GameState->Cards.OpponentField.Empty(Arena->OpponentField->Num());
	for (size_t i = 0; i < Arena->OpponentField->Num(); i++)
	{
		ACard* Card = Arena->OpponentField->GetCardByIndex(i);
		ECard Type = Card->Type;
		if (!Card->IsActive())
		{
			Type = static_cast<ECard>(static_cast<uint32>(Type) + 11);
		}

		GameState->Cards.OpponentField.Add(Type);
	}

	// Opponent Discard
	GameState->Cards.OpponentDiscard.Empty(Arena->OpponentDiscard->Num());
	for (size_t i = 0; i < Arena->OpponentDiscard->Num(); i++)
	{
		ACard* Card = Arena->OpponentDiscard->GetCardByIndex(i);
		ECard Type = Card->Type;
		if (!Card->IsActive())
		{
			Type = static_cast<ECard>(static_cast<uint32>(Type) + 11);
		}

		GameState->Cards.OpponentDiscard.Add(Type);
	}

	// Update scores
	GameState->PlayerScore = AggregateScore(Arena->PlayerField);
	GameState->OpponentScore = AggregateScore(Arena->OpponentField);

	Arena->ScoreDisplay->Update(GameState->PlayerScore, GameState->OpponentScore);
}

void ABladeIIGameMode::LoadingFinished()
{
	DelayedStart();
}

void ABladeIIGameMode::LocalQuit(bool bReportForfeit)
{
	if (bReportForfeit) Opponent->SendUpdate(EServerUpdate::InstructionForfeit, LOCAL_QUIT_METADATA);

	Settings->SaveSettings();

	// TODO add this to a callback instead, that quits after a timeout OR when receiving an OK from the server or something.
	FGenericPlatformMisc::RequestExit(false);
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
		CardSlot = Arena->PlayerDiscard;
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
		CardSlot = Arena->OpponentDiscard;
		break;
	}

	return CardSlot;
}

void ABladeIIGameMode::HandleCardsReceived(const FB2Cards& Cards)
{
	GameState = new B2GameState(Cards);

	if (!Settings->IsVersusAI())
	{
		UILoadingScreenLayer->SetProgress(ULoadingScreen::LoadingBar::PreparingMatch, ++MatchPrepProgress / MATCH_PREP_PROGRESS_TARGET);
	}

	DelayedStart();
}

void ABladeIIGameMode::HandleServerInstruction(const FB2ServerUpdate& Instruction)
{
	// Handle errors first
	if (Instruction.Code >= EServerUpdate::InstructionConnectionError)
	{
		// Unable to connect (initial connection) 
		if (ConnectionProgress < CONNECTION_PROGRESS_TARGET)
		{
			UIErrorModalLayer->SetErrorType(UErrorModal::ErrorType::UnableToConnect);
		}
		else
		{
			if (Instruction.Code == EServerUpdate::InstructionMatchMutualTimeOut)
			{
				UIErrorModalLayer->SetErrorType(UErrorModal::ErrorType::MutualTimeOut);
			}
			else if (Instruction.Code == EServerUpdate::InstructionMatchTimeOut)
			{
				UIErrorModalLayer->SetErrorType(UErrorModal::ErrorType::TimeOut);
			}
			else
			{
				UIErrorModalLayer->SetErrorType(UErrorModal::ErrorType::Disconnected);
			}
		}

		UIOptionsMenuLayer->ClearLanguageComboBoxFocus();
		UIErrorModalLayer->SetActive(true);
	}
	// Opponent forfeit
	else if (Instruction.Code == EServerUpdate::InstructionForfeit)
	{
		UIErrorModalLayer->SetErrorType(UErrorModal::ErrorType::OpponentForfeit);
		UIOptionsMenuLayer->ClearLanguageComboBoxFocus();
		UIErrorModalLayer->SetActive(true);
	}
	// Websocket connection died
	else if (Instruction.Code == EServerUpdate::InstructionConnectionClosed)
	{
		if (EngineState == EEngineState::Initialisation)
		{
			UIErrorModalLayer->SetErrorType(UErrorModal::ErrorType::UnableToConnect);
			UIOptionsMenuLayer->ClearLanguageComboBoxFocus();
			UIErrorModalLayer->SetActive(true);
		}
		else
		{
			UIErrorModalLayer->SetErrorType(UErrorModal::ErrorType::Disconnected);
			UIOptionsMenuLayer->ClearLanguageComboBoxFocus();
			UIErrorModalLayer->SetActive(true);
		}
	}
	// Connection updates
	else if (Instruction.Code == EServerUpdate::InstructionConnectionProgress)
	{
		UILoadingScreenLayer->SetProgress(ULoadingScreen::LoadingBar::Connecting, ++ConnectionProgress / CONNECTION_PROGRESS_TARGET);

		B2Utility::LogInfo(FString::FromInt(ConnectionProgress));
	}
	// Match prep updates
	else if (Instruction.Code == EServerUpdate::InstructionPlayerData || Instruction.Code == EServerUpdate::InstructionOpponentData)
	{
		UILoadingScreenLayer->SetProgress(ULoadingScreen::LoadingBar::PreparingMatch, ++MatchPrepProgress / MATCH_PREP_PROGRESS_TARGET);

		if (Instruction.Code == EServerUpdate::InstructionPlayerData)
		{
			PlayerData.AddLocalPlayerData(Instruction.Payload);
		}
		else
		{
			PlayerData.AddOpponentData(Instruction.Payload);
			AvatarCaptureRig->Initialise(static_cast<AAvatarCaptureRig::Character>(PlayerData.Opponent.Avatar));
			UIAvatarLayer->SetOpponentName(PlayerData.Opponent.DisplayName);
		}
	}

	B2Utility::LogInfo(FString::Printf(TEXT("Server Instruction: [%d] Metadata: [ %s ]"), Instruction.Code, *Instruction.Payload));
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
	case EDealerEvent::CardPositionUpdateFinished:
		// Handling card position updates for blast only

		if (GSM->IsCurrentState(EGameState::PlayerBlastTarget) || GSM->IsCurrentState(EGameState::OpponentBlastTarget))
		{
			GameState->bHandleBlastEdgeCase = true;
			EndState();
		}

		break;
	}
}

void ABladeIIGameMode::HandleMenuButtonPressed()
{
	if (!UIErrorModalLayer->IsActive())
	{
		UIOptionsMenuLayer->ToggleMenu();
	}
}

void ABladeIIGameMode::HandleErrorModalButtonPressed()
{
	// As this callback should only be fired when the retry button was pressed, we don't check anything - just initiate a recheck
	if (Opponent->ReConnect())
	{
		B2Utility::LogInfo("Attempting to reconnect");
		ConnectionProgress = 0;
	}
	else
	{
		B2Utility::LogWarning("Reconnection attempt invalid");
	}
}
