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
#include "B2Engine/GameStateMachine/GSM.h"
#include "B2Game/Arena.h"
#include "B2Game/CardSelector.h"
#include "B2Game/AvatarCaptureRig.h"
#include "B2Game/GameSound.h"
#include "B2Enum/EngineStateEnum.h"
#include "B2Enum/UIEffectEventEnum.h"
#include "B2Enum/WinConditionEnum.h"
#include "B2UI/Avatar.h"
#include "B2UI/LoadingScreen.h"
#include "B2UI/OptionsMenu.h"

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
	 * Informs the engine that one of the players has achieved victory
	 * @param Player - The player that has won
	 * @param WinCondition - The win condition that they achieved
	 */
	void VictoryAchieved(EPlayer Player,  EWinCondition WinCondition);

	/* Informs the engine that it is safe to switch turns */
	void ChangeTurn();

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
	void AutoLoadFinished();

	/* Getters for various references */
	AArena* GetArena() const { return Arena; }
	ACardSelector* GetCursor() const { return Cursor; }
	ALocalPlayerInput* GetLocalPlayerInput() const { return LocalPlayerInput; }
	AAvatarCaptureRig* GetOpponentAvatar() const { return AvatarCaptureRig; }
	AGameSound* GetGameSound() const { return GameSound;  }
	UB2Dealer* GetDealer() const { return Dealer; }
	UB2Opponent* GetOpponent() const { return Opponent; }
	UB2UIEffectLayer* GetUIEffectLayer() const { return UIEffectLayer; }
	UAvatar* GetUIAvatarLayer() const { return UIAvatarLayer; }
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

	/* Pointer to the game sound actor*/
	UPROPERTY()
	AGameSound* GameSound;

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

	/* Loading screen UI layer*/
	UPROPERTY()
	ULoadingScreen* UILoadingScreenLayer;

	/* Loading screen UI layer*/
	UPROPERTY()
	UOptionsMenu* UIOptionsMenuLayer;

	/* Pointer to the settings object */
	UPROPERTY()
	USettings* Settings;

	/* Pointer to the cardfactory that will be used throughout this match */
	UB2CardFactory* CardFactory;

	/* The gamestate state machine instance */
	B2GameStateMachine* GSM;

	/* The current state of the game */
	B2GameState* GameState;

	/* The current state of the engine */
	EEngineState EngineState;

	/* Class type for avatar widget */
	TSubclassOf<UAvatar> UIAvatarWidgetClass;

	/* Class type for avatar widget */
	TSubclassOf<ULoadingScreen> UILoadingScreenWidgetClass;

	/* Class type for avatar widget */
	TSubclassOf<UOptionsMenu> UIOptionsMenuWidgetClass;

	/* Class type for the cursor actor */
	TSubclassOf<ACardSelector> CursorClass;

	/* Class type for the cursor actor */
	TSubclassOf<AAvatarCaptureRig> AvatarCaptureRigClass;

	/* Timer handle for any delayed clear and draw calls */
	FTimerHandle ClearAndDrawHandle;

	bool bOtherDelayedStartComponentReady;

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

	/* Gets and stores a reference to the UI Avatar layer class */
	void GetUIAvatarWidgetClass();

	/* Gets and stores a reference to the UI loading screen layer class */
	void GetUILoadingScreenWidgetClass();

	/* Gets and stores a reference to the UI options menu layer class */
	void GetUIOptionsMenuWidgetClass();

	/* Gets and stores a reference to the cursor actor class */
	void GetCursorClass();

	/* Gets and stores a reference to the avatar capture rig actor class */
	void GetAvatarCaptureRigClass();

	/* Set up any event listeners */
	void RegisterEventListeners();

	/* Find and store a reference to the arena */
	void FindArena();

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

	/* Set up the UI loading screen layer */
	void SetupUILoadingScreenLayer();

	/* Set up the UI loading screen layer */
	void SetupUIOptionsMenuLayer();

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
};
