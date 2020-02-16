#include "B2Engine/LocalPlayerInput.h"

ALocalPlayerInput::ALocalPlayerInput()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ALocalPlayerInput::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALocalPlayerInput::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALocalPlayerInput::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

