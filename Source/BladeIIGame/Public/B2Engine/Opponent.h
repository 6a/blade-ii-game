#pragma once

#include "CoreMinimal.h"

#include "B2Engine/Move.h"
#include "B2Engine/Server.h"
#include "B2Engine/Cards.h"
#include "B2Misc/Enum/InstructionEnum.h"

#include "Opponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveReceivedDelegate, const FB2Move&, Move);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInstructionReceivedDelegate, EInstruction, Move);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCardsReceivedDelegate, const FB2Cards&, Cards);

UCLASS()
class UB2Opponent : public UObject
{
	GENERATED_BODY()

public:
	/* Callback for receiving moves from the server */
	FMoveReceivedDelegate OnMoveReceived;

	/* Callback for receiving instructions from the server */
	FInstructionReceivedDelegate OnInstructionReceived;

	/* Callback for when the cards for this game are received from the server */
	FCardsReceivedDelegate OnCardsReceived;

	/**
	 * Tick this opponent instance.
	 * @param DeltaSeconds - DeltaSeconds for the previous frame.
	 */
	virtual void Tick(float DeltaSeconds);

	/**
	 * Send a move to the server.
	 * @param Move - The move to send
	 */
	void SendMove(const FB2Move& Move);

	/**
	 * Send an instruction to the server.
	 * @param Instruction - The instruction to send
	 */
	void SendInstruction(const EInstruction& Instruction);

protected:
	/* The back end for this opponent. Could be an AI opponent, or a network opponent */
	B2Server* BackEnd;

};

