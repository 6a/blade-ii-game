#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "B2Engine/CardFactory.h"
#include "B2Engine/Opponent.h"
#include "B2Engine/Dealer.h"
#include "B2Engine/BoardState.h"
#include "B2Game/Arena.h"

#include "BladeIIGameGameMode.generated.h"

UCLASS()
class BLADEIIGAME_API ABladeIIGameGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void StartPlay() override;

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

	/* Set up the internal dealer instance */
	void SetupDealer();

	/* Set the board state based on the specified state */
	void InitialiseBoard(B2BoardState BoardState);

	/**
	 * Event handler for moves received from the server.
	 * @param Move - The move that was received
	 */
	UFUNCTION()
	void HandleMoveReceived(FB2Move& Move);

	/**
	 * Event handler for instructions from the server.
	 * @param Move - The instruction that was received
	 */
	UFUNCTION()
	void HandleInstructionReceived(EInstruction& Instruction);
};
