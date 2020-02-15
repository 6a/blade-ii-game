#include "Materials/MaterialInstanceDynamic.h"

#include "B2Engine/CardFactory.h"
#include "B2Misc/Utility.h"

B2CardFactory::B2CardFactory(const B2CardFactoryConfig& CardFactoryConfig)
{
	LoadConfig(CardFactoryConfig);

	B2Utility::LogInfo("Card Factory initialized");
}

B2CardFactory::~B2CardFactory()
{

}

ACard* B2CardFactory::Make(const ECard& Card, const FVector& TargetPosition)
{
	ACard* SpawnedCard = World->SpawnActor<ACard>(CardActorClass, TargetPosition, FRotator::ZeroRotator);
	SpawnedCard->Rename(*B2Utility::EnumToString(Card));
	SpawnedCard->SetActorLabel(*B2Utility::EnumToString(Card));

	UMaterialInstanceDynamic* MaterialFront = UMaterialInstanceDynamic::Create(SpawnedCard->Mesh->GetMaterial(0), NULL);
	UMaterialInstanceDynamic* MaterialBack = UMaterialInstanceDynamic::Create(SpawnedCard->Mesh->GetMaterial(1), NULL);

	if (GetTexture(Card))
	{
		B2Utility::LogInfo(TEXT("Card front texture loaded"));

	}

	MaterialFront->SetTextureParameterValue(TEXT("Texture"), GetTexture(Card));
	MaterialFront->SetTextureParameterValue(TEXT("Metallic / Roughness / Specular Map"), CardFrontMRSTexture);

	MaterialBack->SetTextureParameterValue(TEXT("Texture"), CardBackTexture);
	MaterialBack->SetTextureParameterValue(TEXT("Metallic / Roughness / Specular Map"), CardBackMRSTexture);

	SpawnedCard->Mesh->SetMaterial(0, MaterialFront);
	SpawnedCard->Mesh->SetMaterial(1, MaterialBack);

	B2Utility::LogInfo(FString::Format(TEXT("Card Factory created card: [{0}]"), { B2Utility::EnumToString(Card) }));

	return SpawnedCard;
}

void B2CardFactory::LoadConfig(const B2CardFactoryConfig& CardFactoryConfig)
{
	/* Card front textures */
	for (auto It = CardFactoryConfig.CardFrontPaths.CreateConstIterator(); It; ++It)
	{
		CardFrontTextures.Add(LoadObject<UTexture>(NULL, **It, NULL, LOAD_None, NULL));
	}

	/* Card back and MRS textures */
	CardBackTexture = LoadObject<UTexture>(NULL, *CardFactoryConfig.CardBackPath, NULL, LOAD_None, NULL);
	CardFrontMRSTexture = LoadObject<UTexture>(NULL, *CardFactoryConfig.CardFrontMRSPath, NULL, LOAD_None, NULL);
	CardBackMRSTexture = LoadObject<UTexture>(NULL, *CardFactoryConfig.CardBackMRSPath, NULL, LOAD_None, NULL);

	/* Card actor */
	UObject* CardActor = StaticLoadObject(UObject::StaticClass(), NULL, *CardFactoryConfig.CardActorPath);
	ensureMsgf(CardActor, TEXT("Could not load card actor - static load failed"));

	UBlueprint* CardBlueprint = Cast<UBlueprint>(CardActor);
	ensureMsgf(CardBlueprint, TEXT("Could not cast UObject to UBlueprint"));
	
	CardActorClass = CardBlueprint->GeneratedClass;
	ensureMsgf(CardActorClass, TEXT("Could not get classs from card blueprint"));

	/* World */
	World = CardFactoryConfig.World;
}

UTexture* B2CardFactory::GetTexture(const ECard& Card) const
{
	return CardFrontTextures[static_cast<uint8>(Card)];
}
