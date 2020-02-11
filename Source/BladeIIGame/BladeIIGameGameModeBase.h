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

	QCardFactory* CardFactory;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};
