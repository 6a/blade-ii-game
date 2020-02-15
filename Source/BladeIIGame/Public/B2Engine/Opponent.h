#pragma once

#include "CoreMinimal.h"

#include "B2Engine/Move.h"
#include "B2Enum/Instruction.h"
#include "B2Engine/Server.h"

#include "Opponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveReceivedDelegate, FB2Move&, Move);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInstructionReceivedDelegate, EInstruction&, Move);

UCLASS()
class UB2Opponent : public UObject
{
	GENERATED_BODY()

public:
	/* Callback for receiving moves from the server */
	FMoveReceivedDelegate OnMoveReceived;

	/* Callback for receiving instructions from the server */
	FInstructionReceivedDelegate OnInstructionReceived;

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

private:
	/* The back end for this opponent. Could be an AI opponent, or a network opponent */
	B2Server BackEnd;

};

