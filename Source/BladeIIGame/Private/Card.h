// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Utility.h"
#include "Card.generated.h"

UCLASS()
class ACard : public AActor
{
	GENERATED_BODY()
	
public:
	/* The mesh for this card actor, with read access for blueprints */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	ACard();

	/* The type of this card, with read access for blueprints */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ECard Type;

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

private:

};
