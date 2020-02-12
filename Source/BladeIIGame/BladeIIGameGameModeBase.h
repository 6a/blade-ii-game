#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Private/CardFactory.h"
#include "BladeIIGameGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BLADEIIGAME_API ABladeIIGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	B2CardFactory* CardFactory;

	/**
	 * Entry point for the game's lifecycle. The parameters are passed in unmodified.	
	 * @param MapName - Name of the first map to load
	 * @param Options - Any options passed in as arguments
	 * @param ErrorMessage - Any errors that have occurred elsewhere
	 */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};
