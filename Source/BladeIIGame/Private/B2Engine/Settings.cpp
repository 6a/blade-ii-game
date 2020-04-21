#include "B2Engine/Settings.h"

void USettings::Initialise(const B2LaunchConfig& LaunchConfig)
{
	// Copy and store the settings

	SettingsCache = LaunchConfig;
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