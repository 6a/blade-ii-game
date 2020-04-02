#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "B2Game/CardSlot.h"

#include "Arena.generated.h"

UCLASS()
class BLADEIIGAME_API AArena : public AActor
{
	GENERATED_BODY()
	
public:	
	/* The main mesh for the arena actor, with read access for blueprints */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MainMesh;

	/* The divider for the arena actor, with read access for blueprints */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* DividerMesh;

	/* The VS badge body for the arena actor, with read access for blueprints */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* VSBadgeBody;

	/* The VS badge text for the arena actor, with read access for blueprints */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* VSBadgeText;

	/* Root for all the card anchors */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Slots;

	/* Slot for local players deck slot */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCardSlot* PlayerDeck;

	/* Slot for local players hand slots */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCardSlot* PlayerHand;

	/* Slot for local players field slots */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCardSlot* PlayerField;

	/* Slot for opponents deck slot */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCardSlot* OpponentDeck;

	/* Slot for opponents hand slots */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCardSlot* OpponentHand;

	/* Slot for opponents field slots */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCardSlot* OpponentField;

	AArena();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:

};
