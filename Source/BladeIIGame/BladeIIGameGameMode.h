#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "B2Engine/CardFactory.h"
#include "B2Engine/Opponent.h"
#include "B2Engine/Dealer.h"
#include "B2Engine/BoardState.h"
#include "B2Engine/Cards.h"
#include "B2Engine/LocalPlayerInput.h"
#include "B2Game/Arena.h"
#include "B2Game/CardSelector.h"

#include "BladeIIGameGameMode.generated.h"

UCLASS()
class BLADEIIGAME_API ABladeIIGameGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	ABladeIIGameGameMode(const FObjectInitializer& ObjectInitializer);

private:
	/* Pointer to the cardfactory that will be used throughout this match */
	B2CardFactory* CardFactory;

	/* Pointer to the opponent that will be used throughout this match */
	UB2Opponent* Opponent;

	/* Pointer to the arena that will be used throughout this match */
	AArena* Arena;

	/* Pointer the dealer that will be used throughout this match */
	UB2Dealer* Dealer;

	/* Pointer to the card selector that will be used throughout this match */
	ACardSelector* CardSelector;

	/* The current state of the board */
	B2BoardState BoardState;

	/* The current state of the engine */
	EEngineState EngineState;

	/* Pointer to the local player input receiver */
	ALocalPlayerInput* LocalPlayerInput;

	/**
	 * Reads the launch config and sets up the engine accordingly.
	 * @param ObjectInitializer - ObjectInitializer helper from constructor
	 */
	void SetupLaunchConfig(const FObjectInitializer& ObjectInitializer);

	/* Set up the internal card factory */
	void SetupCardFactory();

	/* Set up any event listeners */
	void RegisterEventListeners();

	/* Find and store a reference to the arena */
	void FindArena();

	/* Find and store a reference to the local player input actors */
	void FindLocalPlayerInput();

	/* Set up the internal dealer instance */
	void SetupDealer();

	/* Set up the card selector actor */
	void SetupSelector();

	/* Set the board state based on the specified state */
	void InitialiseBoard(B2BoardState BoardState);

	/* Performs whatever is required for when the game enters play (post deal) */
	void EnterGamePlayState();

	/* Event listeners */

	/**
	 * Event handler for when the player presses a button.
	 * @param Button - The button that was pressed
	 */
	UFUNCTION()
	void HandleButtonPressed(EButton Button);

	/**
	 * Event handler for receiving the cards for this game.
	 * @param Cards - The cards for this game
	 */
	UFUNCTION()
	void HandleCardsReceived(const FB2Cards& Cards);

	/**
	 * Event handler for moves received from the server.
	 * @param Move - The move that was received
	 */
	UFUNCTION()
	void HandleMoveReceived(const FB2Move& Move);

	/**
	 * Event handler for instructions from the server.
	 * @param Instruction - The instruction that was received
	 */
	UFUNCTION()
	void HandleInstructionReceived(EInstruction Instruction);

	/**
	 * Event handler for updates from the dealer instance.
	 * @param Event - The update type
	 */
	UFUNCTION()
	void HandleEventUpdate(EDealerEvent Event);
};
