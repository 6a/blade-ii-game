#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "B2Engine/CardFactory.h"
#include "B2Engine/Opponent.h"
#include "B2Engine/Dealer.h"
#include "B2Engine/GameState.h"
#include "B2Engine/Cards.h"
#include "B2Engine/LocalPlayerInput.h"
#include "B2Engine/UIEffectLayer.h"
#include "B2Engine/Settings.h"
#include "B2Engine/PlayerData.h"
#include "B2Engine/GameStateMachine/GSM.h"
#include "B2Game/Arena.h"
#include "B2Game/CardSelector.h"
#include "B2Game/AvatarCaptureRig.h"
#include "B2Game/GameSound.h"
#include "B2Game/Camera.h"
#include "B2Enum/EngineStateEnum.h"
#include "B2Enum/UIEffectEventEnum.h"
#include "B2Enum/WinConditionEnum.h"
#include "B2UI/Avatar.h"
#include "B2UI/StatusIndicator.h"
#include "B2UI/LoadingScreen.h"
#include "B2UI/OptionsMenu.h"
#include "B2UI/ErrorModal.h"
#include "B2UI/ToolTipPanel.h"

#include "BladeIIGameMode.generated.h"

UCLASS()
class BLADEIIGAME_API ABladeIIGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABladeIIGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	/* Informs the engine that the current turn has finished */
	void EndState();

	/* Informs the engine that the local player won */

	/**
	 * Informs the engine that the game has ended
	 * @param Victor - The player that has won, or undecided if the game ended in a draw
	 * @param WinCondition - The win condition that they achieved (ignored for draws)
	 */
	void EndGame(EPlayer Victor,  EWinCondition WinCondition);

	/* Informs the engine that it is safe to switch turns */
	void ChangeTurn();

	/* Informs the engine to continue with the same player again, after a blast card was used */
	void ContinueTurn();

	/**
	 * Inform the engine that the scores are tied and the board needs to be cleared and reset, with an optional delay
	 * @param Delay - Optional delay in seconds, defaults to zero
	 */
	UFUNCTION()
	void ClearAndDraw(float Delay = 0.f);

	/* Get a reference to the card slot of the specified type */
	UCardSlot* GetCardSlot(ECardSlot Slot) const;

	/**
	 * Add up all the card values for a particular card slot
	 * @param Slot - The slot to aggregate the score for
	 * @return The aggregate score
	 */
	int32 AggregateScore(UCardSlot* Slot) const;

	/* Update the cards in the game state */
	void UpdateCardState();

	/* Helper function to inform the game mode that the automatic load has finished (when vs AI) */
	void LoadingFinished();

	/**
	 * Helper function to initialise a match quit + shutdown by the local player
	 * @param bReportForfeit - Whether or not to report the quit to the server (defaults to true)
	 */
	void LocalQuit(bool bReportForfeit = true);

	/* Getters for various references */
	AArena* GetArena() const { return Arena; }
	ACamera* GetCamera() const { return Camera; }
	ACardSelector* GetCursor() const { return Cursor; }
	ALocalPlayerInput* GetLocalPlayerInput() const { return LocalPlayerInput; }
	AAvatarCaptureRig* GetOpponentAvatar() const { return AvatarCaptureRig; }
	AGameSound* GetGameSound() const { return GameSound;  }
	UB2Dealer* GetDealer() const { return Dealer; }
	UB2Opponent* GetOpponent() const { return Opponent; }
	UB2UIEffectLayer* GetUIEffectLayer() const { return UIEffectLayer; }
	UAvatar* GetUIAvatarLayer() const { return UIAvatarLayer; }
	UStatusIndicator* GetUIStatusIndicatorLayer() const { return UIStatusIndicatorLayer; }
	ULoadingScreen* GetUILoadingScreenLayer() const { return UILoadingScreenLayer; }
	USettings* GetSettings() const { return Settings; }
	
	UB2CardFactory* GetCardFactory() const { return CardFactory; }
	B2GameState* GetGameState() const { return GameState; }

private:
	/* Pointer to the arena that will be used throughout this match */
	UPROPERTY()
	AArena* Arena;

	/* Pointer to the card selector that will be used throughout this match */
	UPROPERTY()
	ACardSelector* Cursor;

	/* Pointer to the local player input receiver */
	UPROPERTY()
	ALocalPlayerInput* LocalPlayerInput;

	/* Pointer to the avatar capture rig */
	UPROPERTY()
	AAvatarCaptureRig* AvatarCaptureRig;

	/* Pointer to the game sound actor */
	UPROPERTY()
	AGameSound* GameSound;

	/* Pointer to the camera */
	UPROPERTY()
	ACamera* Camera;

	/* Pointer the dealer that will be used throughout this match */
	UPROPERTY()
	UB2Dealer* Dealer;

	/* Pointer to the opponent that will be used throughout this match */
	UPROPERTY()
	UB2Opponent* Opponent;

	/* Pointer to the UI effects layer */
	UPROPERTY()
	UB2UIEffectLayer* UIEffectLayer;

	/* Pointer to UI avatar layer */
	UPROPERTY()
	UAvatar* UIAvatarLayer;

	/* Pointer to UI status indicator layer */
	UPROPERTY()
	UStatusIndicator* UIStatusIndicatorLayer;

	/* Pointer to UI tool tip panel layer */
	UPROPERTY()
	UToolTipPanel* UIToolTipPanelLayer;

	/* Loading screen UI layer*/
	UPROPERTY()
	ULoadingScreen* UILoadingScreenLayer;

	/* Loading screen UI layer*/
	UPROPERTY()
	UOptionsMenu* UIOptionsMenuLayer;

	UPROPERTY()
	UErrorModal* UIErrorModalLayer;

	UPROPERTY()
	UUserWidget* UITitleBarLayer;

	/* Pointer to the settings object */
	UPROPERTY()
	USettings* Settings;

	/* Pointer to the cardfactory that will be used throughout this match */
	UPROPERTY()
	UB2CardFactory* CardFactory;

	/* The gamestate state machine instance */
	B2GameStateMachine* GSM;

	/* The current state of the game */
	B2GameState* GameState;

	/* Data relating to each player (display name, avatar etc. */
	B2PlayerData PlayerData;

	/* The current state of the engine */
	EEngineState EngineState;

	/* Class type for the avatar widget */
	TSubclassOf<UAvatar> UIAvatarWidgetClass;

	/* Class type for the status indicator widget */
	TSubclassOf<UStatusIndicator> UIStatusIndicatorWidgetClass;

	/* Class type for the tooltip panel widget */
	TSubclassOf<UToolTipPanel> UIToolTipPanelWidgetClass;

	/* Class type for the loading screen widget */
	TSubclassOf<ULoadingScreen> UILoadingScreenWidgetClass;

	/* Class type for the options menu widget */
	TSubclassOf<UOptionsMenu> UIOptionsMenuWidgetClass;

	/* Class type for the error modal widget */
	TSubclassOf<UErrorModal> UIErrorModalWidgetClass;

	/* Class type for the title bar widget */
	TSubclassOf<UUserWidget> UITitleBarClass;

	/* Class type for the cursor actor */
	TSubclassOf<ACardSelector> CursorClass;

	/* Class type for the cursor actor */
	TSubclassOf<AAvatarCaptureRig> AvatarCaptureRigClass;

	/* Timer handle for any delayed clear and draw calls */
	FTimerHandle ClearAndDrawHandle;

	/* Used to ensure that one of the calls to DelayedStart results in the game starting */
	bool bOtherDelayedStartComponentReady;

	/* For tracking connection progress - Stored as a float, increment by one for each event */
	float ConnectionProgress;
	float MatchPrepProgress;

	// Loading progress max values
	const float CONNECTION_PROGRESS_TARGET = 5;
	const float MATCH_PREP_PROGRESS_TARGET = 3;

	/**
	 * Reads the launch config and sets up the engine accordingly.
	 * @param ObjectInitializer - ObjectInitializer helper from constructor
	 */
	void SetupLaunchConfig(const FObjectInitializer& ObjectInitializer);

	/**
	 * Creates the card factory (initialisation is done later in begin play)
	 * @param ObjectInitializer - ObjectInitializer helper from constructor
	 */
	void SetupCardFactory(const FObjectInitializer& ObjectInitializer);

	/* Set up the internal card factory */
	void InitialiseCardFactory();

	/* Set up the gameplay state machine */
	void CreateGSM();

	/* Gets and stores a reference to the UI Avatar widget class */
	void GetUIAvatarWidgetClass();

	/* Gets and stores a reference to the status indicator widget class */
	void GetUIStatusIndicatorClass();

	/* Gets and stores a reference to the UI tooltip panel widget class */
	void GetUIToolTipPanelLayer();

	/* Gets and stores a reference to the UI loading screen widget class */
	void GetUILoadingScreenWidgetClass();

	/* Gets and stores a reference to the UI options menu widget class */
	void GetUIOptionsMenuWidgetClass();

	/* Gets and stores a reference to the UI error modal widget class */
	void GetUIErrorModalWidgetClass();

	/* Gets and stores a reference to the UI title bar widget class */
	void GetUITitleBarWidgetClass();

	/* Gets and stores a reference to the cursor actor class */
	void GetCursorClass();

	/* Gets and stores a reference to the avatar capture rig actor class */
	void GetAvatarCaptureRigClass();

	/* Set up any event listeners */
	void RegisterEventListeners();

	/* Initialise the opponent object */
	void InitialiseOpponent();

	/* Find and store a reference to the arena */
	void FindArena();

	/* Find and store a reference to the camera */
	void FindCamera();

	/* Find and store a reference to the local player input actors */
	void FindLocalPlayerInput();

	/* Find and store a reference to the game sound actor */
	void FindGameSoundActor();

	/* Set up the internal dealer instance */
	void SetupDealer();

	/* Set up the card selector actor */
	void SetupSelector();

	/* Set up the UI Effect layer */
	void SetupUIEffectLayer();

	/* Set up the UI avatar layer */
	void SetupUIAvatarLayer();

	/* Set up the UI status indicator layer */
	void SetupUIStatusIndicatorLayer();

	/* Set up the UI tooltip panel layer */
	void SetupUIToolTipPanelLayer();

	/* Set up the UI loading screen layer */
	void SetupUILoadingScreenLayer();

	/* Set up the UI options menu layer */
	void SetupUIOptionsMenuLayer();

	/* Set up the UI error modal layer */
	void SetupUIErrorModalLayer();

	/* Set up the UI title bar layer */
	void SetupUITitleBarLayer();

	/* Set up the avatar capture rig */
	void SetupAvatarCaptureRig();

	/* Set the board state based on the current state */
	void InitialiseBoard();

	/* Start the game if both the opponent and the loading screen are ready - else increment the internal counter */
	void DelayedStart();

	/* Performs whatever is required for when the game enters play (post deal) */
	void OnCardsDealt();

	/* Performs whatever is required for when the dealer has finished an effect related transition */
	void OnEffectReady();
	
	/* Performs whatever is required for when the dealer has finished moving a card */
	void OnCardPlaced();

	/* Event listeners */

	/**
	 * Event handler for receiving the cards for this game.
	 * @param Cards - The cards for this game
	 */
	UFUNCTION()
	void HandleCardsReceived(const FB2Cards& Cards);

	/**
	 * Event handler for receiving an instruction from the server
	 * @param Instruction - The instruction
	 */
	UFUNCTION()
	void HandleServerInstruction(const FB2ServerUpdate& Instruction);

	/**
	 * Event handler for updates from the dealer instance.
	 * @param Event - The update type
	 */
	UFUNCTION()
	void HandleDealerEvent(EDealerEvent Event);

	/* Event handler for when a menu button is received by the input actor */
	UFUNCTION()
	void HandleMenuButtonPressed();

	/* Event handler for when the error modal close button is pressed */
	UFUNCTION()
	void HandleErrorModalButtonPressed();
};
