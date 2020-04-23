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

void AGameSound::PlaySFX(ESFX SFX, float Delay)
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
	case ESFX::EffectRod:
		SFXSoundToPlay = EffectRodSound;
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
	case ESFX::Victory:
		SFXSoundToPlay = VictorySound;
		break;
	case ESFX::Defeat:
		SFXSoundToPlay = DefeatSound;
		break;
	}

	if (SFXSoundToPlay)
	{
		if (Delay > 0)
		{
			FTimerHandle SFXPlayHandle;

			// We fire off the timer with an anonymous lamda and a handle that we dont track - this is a oneshot so it doesnt need to be handled
			// after firing
			FTimerDelegate TimerDelegate; 
			TimerDelegate.BindLambda([this, SFXSoundToPlay]() { this->PlayOneshot(SFXSoundToPlay); });

			GetWorldTimerManager().SetTimer(SFXPlayHandle, TimerDelegate, Delay, false);
		}
		else
		{
			PlayOneshot(SFXSoundToPlay);
		}
	}
}

void AGameSound::SetVolume(EAudioChannel Channel, float Value)
{
	FAudioDevice* CurrentAudioDevice = GetWorld()->GetAudioDevice()->GetAudioDeviceManager()->GetActiveAudioDevice();
	if (CurrentAudioDevice)
	{
		float NewVolume = FMath::Clamp(Value, 0.f, 1.f);

		USoundClass* TargetSoundClass = nullptr;

		switch (Channel)
		{
		case EAudioChannel::Master:
			TargetSoundClass = SoundClassMaster;
			break;
		case EAudioChannel::BGM:
			TargetSoundClass = SoundClassBGM;
			break;
		case EAudioChannel::SFX:
			TargetSoundClass = SoundClassSFX;
			break;
		}

		TargetSoundClass->Properties.Volume = NewVolume;
		CurrentAudioDevice->SetSoundMixClassOverride(SoundMixMaster, TargetSoundClass, 1.f, 1.f, 0, false);
	}
}

void AGameSound::BeginPlay()
{
	Super::BeginPlay();

	GameModeInstance = Cast<ABladeIIGameMode>(GetWorld()->GetAuthGameMode());

	FAudioDevice* CurrentAudioDevice = GetWorld()->GetAudioDevice()->GetAudioDeviceManager()->GetActiveAudioDevice();

	SoundClassMaster->Properties.Volume = GameModeInstance->GetSettings()->GetFloatSetting(EFloatSetting::MasterVolume);
	CurrentAudioDevice->SetSoundMixClassOverride(SoundMixMaster, SoundClassMaster, 1.f, 1.f, 0, false);

	SoundClassBGM->Properties.Volume = GameModeInstance->GetSettings()->GetFloatSetting(EFloatSetting::BGMVolume);
	CurrentAudioDevice->SetSoundMixClassOverride(SoundMixMaster, SoundClassBGM, 1.f, 1.f, 0, false);

	SoundClassSFX->Properties.Volume = GameModeInstance->GetSettings()->GetFloatSetting(EFloatSetting::SFXVolume);
	CurrentAudioDevice->SetSoundMixClassOverride(SoundMixMaster, SoundClassSFX, 1.f, 1.f, 0, false);

}

void AGameSound::PlayOneshot(USoundWave* Sound)
{
	FAudioDevice* CurrentAudioDevice = GetWorld()->GetAudioDevice()->GetAudioDeviceManager()->GetActiveAudioDevice();

	if (CurrentAudioDevice)
	{
		UGameplayStatics::PlaySound2D(this, Sound, GameModeInstance->GetSettings()->GetFloatSetting(EFloatSetting::SFXVolume));
	}
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

		float NewVolume = FMath::InterpEaseIn(0.f, 1.f, BGMFadeAlpha, EASE_EXP);

		FAudioDevice* CurrentAudioDevice = GetWorld()->GetAudioDevice()->GetAudioDeviceManager()->GetActiveAudioDevice();
		if (CurrentAudioDevice)
		{
			SoundClassBGMFade->Properties.Volume = NewVolume;
			CurrentAudioDevice->SetSoundMixClassOverride(SoundMixMaster, SoundClassBGMFade, 1.f, 1.f, 0, false);
		}

		if (BGMFadeAlpha <= 0 || BGMFadeAlpha >= 1)
		{
			bIsFadingBGM = false;
		}
	}
}
