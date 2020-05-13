#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraShake.h"
#include "Camera.generated.h"

UCLASS()
class BLADEIIGAME_API ACamera : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shake)
	TSubclassOf<UCameraShake> CameraShakeClass;

	ACamera(const FObjectInitializer& ObjectInitializer);

	/**
	 * Shake the camera
	 * @param Delay - How long to wait before playing the shake
	 */
	void Shake(float Delay = 0.0f);

private:

	/* Handle for any delayed shake requests */
	FTimerHandle DelayedShakeHandle;

	/* The camera component for this camera */
	UPROPERTY(Category = CameraActor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	/* The component on which all transforms will be based on */
	UPROPERTY(Category = CameraActor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* TransformRoot;

	virtual void BeginPlay() override;

	/* Utility function to call shake, for delayed calls */
	UFUNCTION()
	void ShakeTimerCallback();

};
