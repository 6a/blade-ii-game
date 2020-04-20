#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "GameSound.generated.h"

UCLASS()
class BLADEIIGAME_API AGameSound : public AActor
{
	GENERATED_BODY()
	
public:	

	virtual void Tick(float DeltaTime) override;

	AGameSound();

protected:

	///* The positions for each possible card location */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Configuration)
	//TArray<USound> CardTransforms;

	virtual void BeginPlay() override;

public:	

};
