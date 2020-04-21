#include "B2Game/AvatarCaptureRig.h"

#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "B2UI/Avatar.h"
#include "B2Utility/Log.h"
#include "B2GameMode/BladeIIGameMode.h"

AAvatarCaptureRig::AAvatarCaptureRig()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	AvatarMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Avatar"));
	AvatarMesh->SetupAttachment(RootComponent);

	Boom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Boom"));
	Boom->SetupAttachment(RootComponent);

	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scene Capture Component"));
	CaptureComponent->SetupAttachment(Boom);

	/* TODO change hardcode to loading in some way */
	AvatarCharacterName = "Laura";

	LoadTextures();
}

void AAvatarCaptureRig::SetGameModeInstance(ABladeIIGameMode* InGameModeInstance)
{
	GameModeInstance = InGameModeInstance;
}

void AAvatarCaptureRig::ChangeEyes()
{
	uint32 TextureIndex = FMath::RandRange(1, EyeTextures.Num() - 1);
	SetTexture(Folder::Eyes, TextureIndex);

	if (GetWorldTimerManager().TimerExists(EyeResetHandle))
	{
		GetWorldTimerManager().ClearTimer(EyeResetHandle);
	}

	GetWorldTimerManager().SetTimer(EyeResetHandle, this, &AAvatarCaptureRig::RevertEyes, MAX_EYE_CHANGE_DURATION, false);
}

void AAvatarCaptureRig::RevertEyes()
{
	SetTexture(Folder::Eyes, 0);

	if (GetWorldTimerManager().TimerExists(EyeResetHandle))
	{
		GetWorldTimerManager().ClearTimer(EyeResetHandle);
	}
}

void AAvatarCaptureRig::BeginPlay()
{
	Super::BeginPlay();

	uint32 EyeMaterialSlot = AvatarMesh->GetMaterialIndex(MATERIAL_SLOT_EYE);
	uint32 MouthMaterialSlot = AvatarMesh->GetMaterialIndex(MATERIAL_SLOT_MOUTH);

	EyeMaterialInstance = AvatarMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(EyeMaterialSlot, AvatarMesh->GetMaterial(EyeMaterialSlot));
	MouthMaterialInstance = AvatarMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(MouthMaterialSlot, AvatarMesh->GetMaterial(MouthMaterialSlot));
	
	ensureMsgf(EyeMaterialInstance, TEXT("Couldnt get a reference to and/or cast the eye material for the avatar"));
	ensureMsgf(MouthMaterialInstance, TEXT("Couldnt get a reference to and/or cast the mouth material for the avatar"));
}

void AAvatarCaptureRig::RandomMouthTexture()
{
	uint32 TextureIndex = FMath::RandRange(1, MouthTextures.Num() - 1);
	SetTexture(Folder::Mouth, TextureIndex);

	if (!GameModeInstance->GetUIAvatarLayer()->IsAnimatingCalloutText())
	{
		if (GetWorldTimerManager().TimerExists(MouthAnimationHandle))
		{
			GetWorldTimerManager().ClearTimer(MouthAnimationHandle);

			SetTexture(Folder::Mouth, 0);
		}
	}
}

const FString AAvatarCaptureRig::GetPathForIndex(Folder Folder, uint32 Index) const
{
	FString FolderString;
	FString FileName;

	FString IndexString = FString::FromInt(Index);
	IndexString = IndexString.LeftPad(INDEX_SIZE);
	IndexString = IndexString.Replace(TEXT(" "), TEXT("0"));

	if (Folder == Folder::Eyes)
	{
		FolderString = VARIATION_FOLDER_EYE;
		FileName = FString::Printf(TEXT("T_Eyes_%s_Var_%s"), *AvatarCharacterName, *IndexString);
	}
	else
	{
		FolderString = VARIATION_FOLDER_MOUTH;
		FileName = FString::Printf(TEXT("T_Mouth_%s_Var_%s"), *AvatarCharacterName, *IndexString);
	}

	FileName = FString::Printf(TEXT("%s.%s"), *FileName, *FileName);

	FString OutString = FString::Printf(TEXT("%s%s%s%s%s%s"), *VARIATON_PATH, *AvatarCharacterName, *F_DLIM, *FolderString, *F_DLIM, *FileName);

	return OutString;
}

void AAvatarCaptureRig::LoadTextures()
{
	for (size_t i = 0; i < NUM_EYE_TEX; i++)
	{
		FString Path = GetPathForIndex(Folder::Eyes, i);
		UTexture* LoadedTexture = LoadObject<UTexture>(NULL, *Path, NULL, LOAD_None, NULL);

		if (ensureMsgf(LoadedTexture, TEXT("Could not load texture file: \n%s"), *Path))
		{
			EyeTextures.Add(LoadedTexture);
		}
		else
		{
			B2Utility::LogInfo(FString::Printf(TEXT("Could not load character eye texture: %s"), *Path));
		}
	}

	for (size_t i = 0; i < NUM_MOUTH_TEX; i++)
	{
		FString Path = GetPathForIndex(Folder::Mouth, i);
		UTexture* LoadedTexture = LoadObject<UTexture>(NULL, *Path, NULL, LOAD_None, NULL);

		if (ensureMsgf(LoadedTexture, TEXT("Could not load texture file:\n%s"), *Path))
		{
			MouthTextures.Add(LoadedTexture);
		}
		else
		{
			B2Utility::LogInfo(FString::Printf(TEXT("Could not load character mouth texture: %s"), *Path));
		}
	}
}

void AAvatarCaptureRig::SetTexture(Folder Target, uint32 Index)
{
	TArray<UTexture*> Textures = Target == Folder::Eyes ? EyeTextures : MouthTextures;
	UMaterialInstanceDynamic* MaterialInstance = Target == Folder::Eyes ? EyeMaterialInstance : MouthMaterialInstance;

	if (Textures.IsValidIndex(Index))
	{
		UTexture* Texture = Textures[Index];

		MaterialInstance->SetTextureParameterValue(TEXTURE_PARAM_NAME, Texture);
	}
}

void AAvatarCaptureRig::AnimateMouth()
{
	if (GetWorldTimerManager().TimerExists(MouthAnimationHandle))
	{
		GetWorldTimerManager().ClearTimer(MouthAnimationHandle);
	}

	GetWorldTimerManager().SetTimer(MouthAnimationHandle, this, &AAvatarCaptureRig::RandomMouthTexture, ANIMATION_TICK, true, 0);
}

const FString& AAvatarCaptureRig::GetCurrentCharacterName() const
{
	return AvatarCharacterName;
}

void AAvatarCaptureRig::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

