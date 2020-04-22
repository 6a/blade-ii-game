#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"

#include "AvatarCaptureRig.generated.h"

UCLASS()
class BLADEIIGAME_API AAvatarCaptureRig : public AActor
{
	GENERATED_BODY()
	
public:	
	/* The main mesh for the avatar, with read access for blueprints */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* AvatarMesh;

	/* The boom for the camera, with read access for blueprints */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* Boom;

	/* The scene capture component the avatar, with read access for blueprints */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneCaptureComponent2D* CaptureComponent;

	AAvatarCaptureRig();

	/* Set the current game mode instance */
	void SetGameModeInstance(class ABladeIIGameMode* InGameModeInstance);

	/* Switch to a random set of eyes - will automatically revert after the timeout period */
	void ChangeEyes();

	/* Revert the avatars eyes to the default ones */
	UFUNCTION()
	void RevertEyes();

	/* Initiates a mouth animation that lasts for the specified amount of time */
	void AnimateMouth();

	/* Returns the name of the current character */
	const FString& GetCurrentCharacterName() const;

protected:

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

private:
	enum class Folder : uint8
	{
		Mouth,
		Eyes
	};

	/* Class consts */

	/* The root path for all mouth and eye variations */
	const FString VARIATON_PATH = "/Game/BladeIIGame/Textures/Characters/";

	/* The folder name for mouth variations */
	const FString VARIATION_FOLDER_MOUTH = "Mouths";

	/* The folder name for eye variations */
	const FString VARIATION_FOLDER_EYE = "Eyes";

	/* Names of the eye and mouth material slots */
	const FName MATERIAL_SLOT_EYE = FName(TEXT("Eye"));
	const FName MATERIAL_SLOT_MOUTH = FName(TEXT("Mouth"));

	/* Name of the eye and mouth texture parameter */
	const FName TEXTURE_PARAM_NAME = FName(TEXT("Dynamic"));

	/* The folder delimiter */
	const FString F_DLIM = "/";

	/* The total number of eye textures to load */
	const uint32 NUM_EYE_TEX = 6;

	/* The total number of mouth textures to load */
	const uint32 NUM_MOUTH_TEX = 12;

	/* Number of characters for each file's index suffix */
	const uint32 INDEX_SIZE = 3;

	/* Time between mouth animation updates */
	const float ANIMATION_TICK = 0.1f;

	/* How long to wait before automatically resetting eye changes */
	const float MAX_EYE_CHANGE_DURATION = 4.f;

	/* The character name for the current avatar (used to find mouth and eye textures) */
	FString AvatarCharacterName;

	/* Reference to eye material */
	UPROPERTY()
	UMaterialInstanceDynamic* EyeMaterialInstance;

	/* Reference to mouth material */
	UPROPERTY()
	UMaterialInstanceDynamic* MouthMaterialInstance;

	/* Loaded eye textures */
	UPROPERTY()
	TArray<UTexture*> EyeTextures;

	/* Loaded mouth textures */
	UPROPERTY()
	TArray<UTexture*> MouthTextures;

	/* Timer handle for mouth animations */
	FTimerHandle MouthAnimationHandle;

	/* Timer handle for resetting eye animations */
	FTimerHandle EyeResetHandle;

	/* Forward declaration for game mode instance so we can check the avatar UI layer for stuff */
	UPROPERTY()
	class ABladeIIGameMode* GameModeInstance;

	/* Swaps the mouth texture for a random texture */
	UFUNCTION()
	void RandomMouthTexture();

	/**
	 * Returns the reference path for the specific asset of the specific index
	 * @param Folder - The folder to search
	 * @param Index - The index for the texture
	 * @return the reference path as a string
	 */
	const FString GetPathForIndex(Folder Folder, uint32 Index) const;

	/* Load the mouth and eye textures for this avatar */
	void LoadTextures();

	/**
	 * Sets the texture with the specified index for the specified target
	 * @param Target - The target for which the texture will be changed
	 * @param Index - The index for the texture
	 */
	void SetTexture(Folder Target, uint32 Index);
};