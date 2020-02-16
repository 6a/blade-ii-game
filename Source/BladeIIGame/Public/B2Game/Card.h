#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

#include "B2Misc/Enum.h"
#include "B2Misc/Transition.h"

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

	/**
	 * Initiate an asynchronous transition.
	 * @param Transition - A transition helper objectq
	 */
	void StartTransitionAsync(const B2Transition& Transition);

	/* Returns true if this card is currently transitioning */
	bool IsTransitioning() const;

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

private:
	/* The current transition handler for this card */
	B2Transition CurrentTransition;
};
