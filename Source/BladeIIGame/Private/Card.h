// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Card.generated.h"

UENUM(BlueprintType)
enum class ECard : uint8
{
	ElliotsOrbalStaff UMETA(DisplayName = "Elliot's Orbal Staff"),
	FiesTwinGunswords UMETA(DisplayName = "Fie's Twin Gunswords"),
	AlisasOrbalBow UMETA(DisplayName = "Alisa's Orbal Bow"),
	JusisSword UMETA(DisplayName = "Jusis' Sword"),
	MachiasOrbalShotgun UMETA(DisplayName = "Machias' Orbal Shotgun"),
	GaiusSpear UMETA(DisplayName = "Gaius' Spear"),
	LaurasGreatsword UMETA(DisplayName = "Laura's Greatsword"),
	ReansTachi UMETA(DisplayName = "Rean's Tachi"),
	EmmasOrbalStaff UMETA(DisplayName = "Emma's Orbal Staff"),
	ElisesRapier UMETA(DisplayName = "Elise's Rapier"),
	SarasSwordAndGun UMETA(DisplayName = "Sara's Sword and Gun")
};

UCLASS()
class ACard : public AActor
{
	GENERATED_BODY()
	
public:
	/* The mesh for this card actor - exposed to any child blueprints */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	ACard();

	/**
	 * Helper function that returns the string representation of a card enum.
	 * @param Card - The card enum to return as a string.
	 * @return the card enum as a string
	 */
	static FString EnumToString(ECard Card);

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

private:

};
