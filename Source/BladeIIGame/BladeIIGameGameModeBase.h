#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "B2Engine/CardFactory.h"
#include "B2Engine/Opponent.h"

#include "BladeIIGameGameModeBase.generated.h"

UCLASS()
class BLADEIIGAME_API ABladeIIGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void StartPlay() override;

protected:
	ABladeIIGameGameModeBase(const FObjectInitializer& ObjectInitializer);

private:
	/* Pointer to the cardfactory that will be used throughout this match */
	B2CardFactory* CardFactory;

	/* Pointer to the opponent that will be used throughout this match */
	UB2Opponent* Opponent;

	/**
	 * Reads the launch config and sets up the engine accordingly.
	 * @param ObjectInitializer - ObjectInitializer helper from constructor
	 */
	void SetupLaunchConfig(const FObjectInitializer& ObjectInitializer);

	/* Set up the internal card factory */
	void SetupCardFactory();

	/* Set up any event listeners */
	void RegisterEventListeners();

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
