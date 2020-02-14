#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Private/CardFactory.h"
#include "Private/Opponent.h"
#include "BladeIIGameGameModeBase.generated.h"

UCLASS()
class BLADEIIGAME_API ABladeIIGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

protected:
	/**
	 * Entry point for the game's lifecycle. The parameters are passed in unmodified.
	 * @param MapName - Name of the first map to load
	 * @param Options - Any options passed in as arguments
	 * @param ErrorMessage - Any errors that have occurred elsewhere
	 */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

private:
	/* Pointer to the cardfactory that will be used throughout this match */
	B2CardFactory* CardFactory;

	/* Pointer to the opponent that will be used throughout this match */
	UB2Opponent* Opponent;

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
