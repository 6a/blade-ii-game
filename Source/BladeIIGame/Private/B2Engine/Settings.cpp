#include "B2Engine/Settings.h"

#include "B2Utility/Log.h"

#include "B2GameMode/BladeIIGameMode.h"

void USettings::Initialise(ABladeIIGameMode* GameMode, const B2LaunchConfig& LaunchConfig)
{
	// Copy and store the settings
	SettingsCache = LaunchConfig;

	// Set game mode instance pointer
	GameModeInstance = GameMode;

	// TODO do this properly
	VersionString = "V0.0.1.202004212113";
}

float USettings::GetFloatSetting(EFloatSetting Setting) const
{
	float OutFloat = -1;

	switch (Setting)
	{
	case EFloatSetting::MasterVolume:
		OutFloat = SettingsCache.MasterVolume / 100.f;
		break;
	case EFloatSetting::BGMVolume:
		OutFloat = SettingsCache.BGMVolume / 100.f;
		break;
	case EFloatSetting::SFXVolume:
		OutFloat = SettingsCache.SFXVolume / 100.f;
		break;
	}

	return OutFloat;
}

void USettings::SetFloatSetting(EFloatSetting Setting, float Value)
{
	switch (Setting)
	{
	case EFloatSetting::MasterVolume:
		SettingsCache.MasterVolume = FMath::Clamp(FMath::FloorToInt(100.f * Value), 0, 100);
		GameModeInstance->GetGameSound()->SetVolume(EAudioChannel::Master, Value);
		break;
	case EFloatSetting::BGMVolume:
		SettingsCache.BGMVolume = FMath::Clamp(FMath::FloorToInt(100.f * Value), 0, 100);
		GameModeInstance->GetGameSound()->SetVolume(EAudioChannel::BGM, Value);
		break;
	case EFloatSetting::SFXVolume:
		SettingsCache.SFXVolume = FMath::Clamp(FMath::FloorToInt(100.f * Value), 0, 100);
		GameModeInstance->GetGameSound()->SetVolume(EAudioChannel::SFX, Value);
		break;
	}
}

FString USettings::GetStringSetting(EStringSetting Setting) const
{
	FString OutString = "";

	switch (Setting)
	{
	case EStringSetting::Version:
		OutString = VersionString;
		break;
	}

	return OutString;
}

void USettings::SetStringSetting(EStringSetting Setting, const FString& Value)
{
	switch (Setting)
	{
	case EStringSetting::Version:
		VersionString = Value;
		break;
	}
}

bool USettings::IsVersusAI() const
{
	return SettingsCache.bIsBotGame;
}
