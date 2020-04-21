#include "B2Game/GameSound.h"

#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundMix.h"
#include "AudioDeviceManager.h"
#include "AudioDevice.h"
#include "Kismet/GameplayStatics.h"

#include "B2GameMode/BladeIIGameMode.h"

#include "B2Utility/Log.h"

AGameSound::AGameSound()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	BGMAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGM Audio Component"));
	BGMAudioComponent->SetupAttachment(RootComponent);

	SFXAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SFX Audio Component"));
	SFXAudioComponent->SetupAttachment(RootComponent);
}

void AGameSound::PlayBGM(float FadeInDuration)
{
	BGMAudioComponent->SetSound(BGM);
	BGMAudioComponent->Play();

	if (FadeInDuration > 0)
	{
		bIsFadingBGM = true;
		BGMTargetAlpha = 1;
		BGMFadeDuration = FadeInDuration;
	}
}

void AGameSound::PlaySFX(ESFX SFX)
{
	USoundWave* SFXSoundToPlay = nullptr;

	switch (SFX)
	{
	case ESFX::TextPulse:
		SFXSoundToPlay = TextPulseSound;
		break;
	case ESFX::CardDeal:
		SFXSoundToPlay = CardDealingSounds[FMath::RandRange(0, CardDealingSounds.Num() - 1)];
		break;
	case ESFX::CursorNavigate:
		SFXSoundToPlay = CursorNavigationSound;
		break;
	case ESFX::CursorSelect:
		SFXSoundToPlay = CursorSelectSound;
		break;
	case ESFX::EffectBase:
		SFXSoundToPlay = EffectBaseSound;
		break;
	case ESFX::EffectBolt:
		SFXSoundToPlay = EffectBoltSound;
		break;
	case ESFX::EffectBlastStart:
		SFXSoundToPlay = EffectBlastStartSound;
		break;
	case ESFX::EffectBlastSelect:
		SFXSoundToPlay = EffectBlastSelectSound;
		break;
	case ESFX::EffectForce:
		SFXSoundToPlay = EffectForceSound;
		break;
	}

	if (SFXSoundToPlay)
	{
		FAudioDevice* CurrentAudioDevice = GetWorld()->GetAudioDevice()->GetAudioDeviceManager()->GetActiveAudioDevice();

		if (CurrentAudioDevice)
		{
			UGameplayStatics::PlaySound2D(this, SFXSoundToPlay);
		}
	}
}

void AGameSound::BeginPlay()
{
	Super::BeginPlay();

}

void AGameSound::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFadingBGM)
	{
		float Step = ALPHA_MAX / (BGMFadeDuration / DeltaTime);

		if (BGMFadeAlpha > BGMTargetAlpha)
		{
			Step *= -1.f;
		}

		BGMFadeAlpha = FMath::Clamp(BGMFadeAlpha + Step, 0.f, ALPHA_MAX);

		float NewVolume = FMath::InterpEaseIn(0.f, GameModeInstance->GetSettings()->GetFloatSetting(EFloatSetting::BGMVolume), BGMFadeAlpha, EASE_EXP);

		FAudioDevice* CurrentAudioDevice = GetWorld()->GetAudioDevice()->GetAudioDeviceManager()->GetActiveAudioDevice();
		if (CurrentAudioDevice)
		{
			CurrentAudioDevice->SetSoundMixClassOverride(SoundMixMaster, SoundClassBGMFade, NewVolume, 1.f, 0, false);
		}

		if (BGMFadeAlpha <= 0 || BGMFadeAlpha >= 1)
		{
			bIsFadingBGM = false;
		}
	}
}

void AGameSound::SetGameModeInstance(ABladeIIGameMode* GameMode)
{
	GameModeInstance = GameMode;
}

