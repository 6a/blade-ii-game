#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "AvatarCaptureRig.generated.h"

UCLASS()
class BLADEIIGAME_API AAvatarCaptureRig : public AActor
{
	GENERATED_BODY()
	
public:	
	/* The main mesh for the avatar, with read access for blueprints */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Avatar;

	/* The boom for the camera, with read access for blueprints */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* Boom;

	/* The camera component the avatar, with read access for blueprints */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;

	AAvatarCaptureRig();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

};
