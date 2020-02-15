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

protected:
	ABladeIIGameGameModeBase(const FObjectInitializer& ObjectInitializer);

	virtual void StartPlay() override;

private:
	/* Pointer to the cardfactory that will be used throughout this match */
	B2CardFactory* CardFactory;

	/* Pointer to the opponent that will be used throughout this match */
	UB2Opponent* Opponent;

	void SetupCardFactory();

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
