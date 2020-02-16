#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

#include "B2Misc/Enum.h"

#include "Card.generated.h"

UCLASS()
class ACard : public AActor
{
	GENERATED_BODY()
	
public:
	/* The mesh for this card actor, with read access for blueprints */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	/* The type of this card, with read access for blueprints */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ECard Type;

	ACard();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

};
