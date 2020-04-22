#pragma once

#include "CoreMinimal.h"

#include "Sound/SoundWave.h"
#include "GameFramework/Actor.h"

#include "B2Enum/SFXEnum.h"
#include "B2Enum/AudioChannelEnum.h"

#include "GameSound.generated.h"

UCLASS()
class BLADEIIGAME_API AGameSound : public AActor
{
	GENERATED_BODY()
	
public:	

	/* The BGM audio component for this actor, used for playing BGM */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* BGMAudioComponent;

	AGameSound();

	virtual void Tick(float DeltaTime) override;

	/**
	 * Start playing the BGM track - fades in over the specified duration, or instant if <= 0
	 * @param FadeInDuration - Duration of the fade in
	 */
	void PlayBGM(float FadeInDuration);

	/**
	 * Play an SFX oneshot of the specified type
	 * @param SFX - The effect to play
	 * @param Delay - Optional delay before playing
	 */
	void PlaySFX(ESFX SFX, float Delay = 0);

	/**
	 * Change the volume for the specified audio channel
	 * @param Channel - The channel to modify
	 * @param Value - The new volume (0-1)
	 */
	void SetVolume(EAudioChannel Channel, float Value);

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

	/* Victory sound */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SoundBank|Game")
	USoundWave* VictorySound;

	/* Defeat sound */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SoundBank|Game")
	USoundWave* DefeatSound;

	/* Master sound class */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SoundBank|Config")
	USoundClass* SoundClassMaster;

	/* SFX sound class */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SoundBank|Config")
	USoundClass* SoundClassSFX;

	/* BGM sound class */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SoundBank|Config")
	USoundClass* SoundClassBGM;

	/* BGM fade sound class */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SoundBank|Config")
	USoundClass* SoundClassBGMFade;

	/* BGM sound class */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SoundBank|Config")
	USoundMix* SoundMixMaster;

	virtual void BeginPlay() override;

private:

	/* Pointer to the game mode instance */
	class ABladeIIGameMode* GameModeInstance;

	/* Various const values */
	float ALPHA_MAX = 1.f;
	float EASE_EXP = 2.f;

	/* Whether or not the BGM is currently fading in or out */
	bool bIsFadingBGM;

	/* The current alpha for the BGM fade */
	float BGMFadeAlpha;

	/* The target alpha for the current BGM fade */
	float BGMTargetAlpha;

	/* The duration over which the BGM will fade */
	float BGMFadeDuration;

	/**
	 * Play oneshot of the specifed sound
	 * @param Sound - The sound to play
	 */
	void PlayOneshot(USoundWave* Sound);
};
