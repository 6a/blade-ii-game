// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	ACard();
	virtual void Tick(float DeltaTime) override;

	static FString EnumToString(ECard card);

protected:
	virtual void BeginPlay() override;

private:


};
