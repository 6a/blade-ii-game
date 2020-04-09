#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

#include "B2Misc/Enum/CardEnum.h"
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

	/* The type of this cards */
	ECard Type;

	ACard();

	/**
	 * Add a transition to the transition queue, which will play after any preceeding transitions have finished playing.
	 * @param Transition - A transition helper object
	 */
	void QueueTransition(const B2Transition& Transition);

	/* Returns true if this card is currently transitioning */
	bool IsTransitioning() const;

	/* Returns the ID for this card */
	const FString GetID() const;

	/**
	 * Change the active state for this card.
	 * @param bNewActive - The new State
	 */
	void SetActive(bool bNewActive);

	/* Returns true if this card is currently active */
	bool IsActive();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

private:
	/* The queue of transitions for this card */
	TQueue<B2Transition> Transitions;

	/* The randomly generated ID for this card */
	FString ID;

	bool bIsActive;
};
