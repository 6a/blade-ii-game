#pragma once

#include "CoreMinimal.h"

#include "Sound/SoundWave.h"
#include "GameFramework/Actor.h"

#include "GameSound.generated.h"

UCLASS()
class BLADEIIGAME_API AGameSound : public AActor
{
	GENERATED_BODY()
	
public:	

	/* The BGM audio component for this actor, used for playing BGM */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* BGMAudioComponent;

	/* The Effects audio component for this actor, used for playing sound effects */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* SFXAudioComponent;

	virtual void Tick(float DeltaTime) override;

	AGameSound();

protected:

	/* Card Dealing shounds */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SoundBank|CardDealing")
	TArray<USoundWave*> CardDealingSounds;

	/* BGM Track */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SoundBank|BGM")
	USoundWave* BGM;

	/* Text pulse sound for callouts */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SoundBank|Callout")
	USoundWave* TextPulseSound;

	/* Cursor navigation sound */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SoundBank|Cursor")
	USoundWave* CursorNavigationSound;

	/* Cursor select sound */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SoundBank|Cursor")
	USoundWave* CursorSelectSound;

	/* Effect base sound */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SoundBank|Effects")
	USoundWave* EffectBaseSound;

	/* rod sound */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SoundBank|Effects")
	USoundWave* EffectRodSound;

	/* Bolt sound */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SoundBank|Effects")
	USoundWave* EffectBoltSound;

	/* Blast start sound */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SoundBank|Effects")
	USoundWave* EffectBlastStartSound;

	/* Blast select sound */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SoundBank|Effects")
	USoundWave* EffectBlastSelectSound;

	/* Force sound */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SoundBank|Effects")
	USoundWave* EffectForceSound;

	virtual void BeginPlay() override;	

};
