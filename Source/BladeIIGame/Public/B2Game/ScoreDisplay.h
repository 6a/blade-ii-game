

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ScoreDisplay.generated.h"


UCLASS(Blueprintable)
class BLADEIIGAME_API UScoreDisplay : public USceneComponent
{
	GENERATED_BODY()

public:	
	UScoreDisplay();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
