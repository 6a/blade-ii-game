#include "B2Engine/CardFactory.h"

#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "B2Utility/Log.h"
#include "B2Utility/String.h"

const FString CARD_ACTOR_PATH = TEXT("Blueprint'/Game/BladeIIGame/Blueprints/GameObjects/BP_Card'");

void UB2CardFactory::Initialise(const B2CardFactoryConfig& CardFactoryConfig, AArena* InArena)
{
	Arena = InArena;

	LoadConfig(CardFactoryConfig);

	B2Utility::LogInfo("Card Factory initialized");
}

UB2CardFactory::~UB2CardFactory()
{

}

UB2CardFactory::UB2CardFactory(const FObjectInitializer& ObjectInitialiser)
{
	/* Card actor has to be in constructor as it uses constructor helper */
	ConstructorHelpers::FClassFinder<ACard> ClassFinder(*CARD_ACTOR_PATH);
	if (ensureMsgf(ClassFinder.Succeeded(), TEXT("Could not find the blueprint for the card actor")))
	{
		CardActorClass = ClassFinder.Class;
	}
}

ACard* UB2CardFactory::Make(const ECard& Card, const FVector& StartingPosition, const FRotator& StartingRotation)
{
	ACard* SpawnedCard = World->SpawnActor<ACard>(CardActorClass, StartingPosition, StartingRotation);
	SpawnedCard->Type = Card;

#if WITH_EDITOR
	SpawnedCard->Rename(*B2Utility::CardEnumToString(Card));
	SpawnedCard->SetActorLabel(*B2Utility::CardEnumToString(Card));
#endif

	UMaterialInstanceDynamic* MaterialFront = SpawnedCard->Mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, SpawnedCard->Mesh->GetMaterial(0));
	UMaterialInstanceDynamic* MaterialBack = SpawnedCard->Mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(1, SpawnedCard->Mesh->GetMaterial(1));

	MaterialFront->SetTextureParameterValue(TEXT("Texture"), GetTexture(Card));
	MaterialFront->SetTextureParameterValue(TEXT("MRS Map"), CardFrontMRSTexture);

	MaterialBack->SetTextureParameterValue(TEXT("Texture"), CardBackTexture);
	MaterialBack->SetTextureParameterValue(TEXT("MRS Map"), CardBackMRSTexture);

	SpawnedCard->Mesh->SetMaterial(0, MaterialFront);
	SpawnedCard->Mesh->SetMaterial(1, MaterialBack);
	
	Arena->AttachCard(SpawnedCard);

	return SpawnedCard;
}

void UB2CardFactory::LoadConfig(const B2CardFactoryConfig& CardFactoryConfig)
{
	/* Card front textures */
	for (auto It = CardFactoryConfig.CardFrontPaths.CreateConstIterator(); It; ++It)
	{
		CardFrontTextures.Add(LoadObject<UTexture>(this, **It, NULL, LOAD_None, NULL));
	}

	/* Card back and MRS textures */
	CardBackTexture = LoadObject<UTexture>(this, *CardFactoryConfig.CardBackPath, NULL, LOAD_None, NULL);
	CardFrontMRSTexture = LoadObject<UTexture>(this, *CardFactoryConfig.CardFrontMRSPath, NULL, LOAD_None, NULL);
	CardBackMRSTexture = LoadObject<UTexture>(this, *CardFactoryConfig.CardBackMRSPath, NULL, LOAD_None, NULL);

	/* World */
	World = CardFactoryConfig.World;
}

UTexture* UB2CardFactory::GetTexture(const ECard& Card) const
{
	return CardFrontTextures[static_cast<uint8>(Card)];
}
