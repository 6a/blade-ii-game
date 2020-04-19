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

	/* Switch to a random set of eyes - will automatically revert after the timeout period */
	void ChangeEyes();

	/* Revert the avatars eyes to the default ones */
	void RevertEyes();

	/**
	 * Initiates a mouth animation that lasts for the specified amount of time
	 * @param Duration - how long the animation should last
	 */
	void AnimateMouth(float Duration);

protected:

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

private:
	enum class Folder : uint8
	{
		Mouth,
		Eyes
	};
	
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

	/**
	 * Returns the reference path for the specific asset of the specific index
	 * @param Folder - The folder to search
	 * @param Index - The index for the texture
	 * @return the reference path as a string
	 */
	const FString GetPathForIndex(Folder Folder, uint32 Index) const;

	/* Load the mouth and eye textures for this avatar */
	void LoadTextures();
};
