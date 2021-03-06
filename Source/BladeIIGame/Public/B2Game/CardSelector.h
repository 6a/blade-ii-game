#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CardSelector.generated.h"

UCLASS()
class BLADEIIGAME_API ACardSelector : public AActor
{
	GENERATED_BODY()

public:
	/* Helper const for working out the offset of a card that is currently selected */
	static const FVector OFFSET_WHEN_SELECTED;

	/* The mesh for this actor, with read access for blueprints */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	ACardSelector();

	/**
	 * Toggle the visiblity for the selector
	 * @param bIsVisible - whether the selector should be visible or not after this function call
	 */
	void ToggleActorVisibility(bool bVisible);

	/* Returns true if the selector is visible */
	bool IsVisible() const;

private:
	/* Whether the selector is currently visible */
	bool bIsVisible;
};