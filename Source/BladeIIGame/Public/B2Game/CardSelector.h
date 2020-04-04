#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CardSelector.generated.h"

UCLASS()
class BLADEIIGAME_API ACardSelector : public AActor
{
	GENERATED_BODY()

public:
	/* The mesh for this actor, with read access for blueprints */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	ACardSelector();
};
