#include "Card.h"
#include "UObject/ConstructorHelpers.h"

ACard::ACard()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Card Mesh"));
	RootComponent = Mesh;
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

FString ACard::EnumToString(ECard Card)
{
	switch (Card)
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

	return "00 Unknown";
}

