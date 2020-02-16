

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LocalPlayerInput.generated.h"

UCLASS()
class BLADEIIGAME_API ALocalPlayerInput : public APawn
{
	GENERATED_BODY()

public:
	ALocalPlayerInput();

	virtual void Tick(float DeltaTime) override;

	/* Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

};
