#include "B2Engine/Settings.h"

void USettings::Initialise(const B2LaunchConfig& LaunchConfig)
{
	// Copy and store the settings

	SettingsCache = LaunchConfig;

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

bool USettings::IsVersusAI() const
{
	return SettingsCache.bIsBotGame;
}
