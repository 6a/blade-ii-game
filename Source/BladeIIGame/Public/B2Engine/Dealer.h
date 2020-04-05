#pragma once

#include "CoreMinimal.h"

#include "B2Game/Arena.h"
#include "B2Misc/Enum.h"

#include "Dealer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCardsDealtDelegate, EDealerEvent, Update);

UCLASS()
class UB2Dealer : public UObject
{
	GENERATED_BODY()

public:	
	/* Callback for when dealing has finished */
	FCardsDealtDelegate EnterGamePlayState;
	
	/* Pointer to the arena */
	AArena* Arena;

	UB2Dealer();

	/* Deal the cards out onto the arena. Can only be called once */
	void Deal();

	/* Tick the dealer so that it can perform tasks such as calling-back after transitions are finished etc. */
	void Tick(float DeltaSeconds);
private:
	
	/* Whether or not the cards have already been dealt (to avoid dealing twice) */
	bool bCardsDealt;

	/* Waitgroups for firing various events */
	B2WaitGroup WaitGroupDealFinished;
};

