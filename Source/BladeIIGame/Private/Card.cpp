// Fill out your copyright notice in the Description page of Project Settings.


#include "Card.h"

// Sets default values
ACard::ACard()
{
}

// Called when the game starts or when spawned
void ACard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString ACard::EnumToString(ECard card)
{
	switch (card)
	{
	case ECard::ElliotsOrbalStaff:
		return "01 Elliot's Orbal Staff";
	case ECard::FiesTwinGunswords:
		return "02 Fie's Twin Gunswords";
	case ECard::AlisasOrbalBow:
		return "03 Alisa's Orbal Bow";
	case ECard::JusisSword:
		return "04 Jusis' Sword";
	case ECard::MachiasOrbalShotgun:
		return "05 Machias' Orbal Shotgun";
	case ECard::GaiusSpear:
		return "06 Gaius' Spear";
	case ECard::LaurasGreatsword:
		return "07 Laura's Greatsword";
	case ECard::ReansTachi:
		return "08 Rean's Tachi";
	case ECard::EmmasOrbalStaff:
		return "09 Emma's Orbal Staff";
	case ECard::ElisesRapier:
		return "10 Elise's Rapier";
	case ECard::SarasSwordAndGun:
		return "11 Sara's Sword and Gun";
	}

	return "";
}

