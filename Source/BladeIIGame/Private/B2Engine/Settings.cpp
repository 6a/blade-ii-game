#include "B2Engine/Settings.h"

#include "Internationalization/Internationalization.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine/Engine.h"
#include "RHI.h"
#include "TimerManager.h"

#include "B2Utility/Log.h"
#include "B2Predicate/MatchResolution.h"
#include "B2Predicate/SortByResolution.h"
#include "B2Predicate/SortByRefreshRate.h"
#include "B2GameMode/BladeIIGameMode.h"

void USettings::Initialise(ABladeIIGameMode* GameMode, const B2LaunchConfig& LaunchConfig)
{
	// Copy and store the settings
	SettingsCache = LaunchConfig;

	// Set game mode instance pointer
	GameModeInstance = GameMode;

	// TODO do this properly
	VersionString = "0.0.1.202004270020";
}

float USettings::GetFloatSetting(EFloatSetting Setting) const
{
	float OutFloat = -1;

	switch (Setting)
	{
	case EFloatSetting::MasterVolume:
		OutFloat = SettingsCache.MasterVolume;
		break;
	case EFloatSetting::BGMVolume:
		OutFloat = SettingsCache.BGMVolume;
		break;
	case EFloatSetting::SFXVolume:
		OutFloat = SettingsCache.SFXVolume;
		break;
	}

	return OutFloat;
}

void USettings::SetFloatSetting(EFloatSetting Setting, float Value)
{
	switch (Setting)
	{
	case EFloatSetting::MasterVolume:
		SettingsCache.MasterVolume = Value;
		GameModeInstance->GetGameSound()->SetVolume(EAudioChannel::Master, Value);
		break;
	case EFloatSetting::BGMVolume:
		SettingsCache.BGMVolume = Value;
		GameModeInstance->GetGameSound()->SetVolume(EAudioChannel::BGM, Value);
		break;
	case EFloatSetting::SFXVolume:
		SettingsCache.SFXVolume = Value;
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
	case EStringSetting::Language:
		OutString = ShortLocaleStringToFull(SettingsCache.Language);
		break;
	case EStringSetting::PublicID:
		OutString = SettingsCache.PublicID;
		break;
	case EStringSetting::AuthToken:
		OutString = SettingsCache.AuthToken;
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
	case EStringSetting::Language:
		SettingsCache.Language = FullLocaleStringToShort(Value);
		FInternationalization::Get().SetCurrentCulture(SettingsCache.Language);
		break;
	case EStringSetting::PublicID:
		SettingsCache.PublicID = Value;
		break;
	case EStringSetting::AuthToken:
		SettingsCache.AuthToken = Value;
		break;
	}
}

uint64 USettings::GetIntSetting(EIntSetting Setting) const
{
	uint64 OutInt = 0;

	switch (Setting)
	{
	case EIntSetting::MatchID:
		OutInt = SettingsCache.MatchID;
		break;
	case EIntSetting::ScreenMode:
		OutInt = SettingsCache.ScreenMode;
		break;
	}

	return OutInt;
}

void USettings::SetIntSetting(EIntSetting Setting, uint64 Value)
{

}

bool USettings::IsVersusAI() const
{
	return SettingsCache.bIsBotGame;
}

void USettings::ApplyAll()
{
	// Language
	FInternationalization::Get().SetCurrentLanguageAndLocale(SettingsCache.Language);

	// Volume is handled by the game sound actor at game start

	// Screen settings (only if not in editor mode)
	if (/*!WITH_EDITOR && */GEngine)
	{
		UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
		if (GameUserSettings)
		{
			// Screen resolution & refresh rate
			FScreenResolutionArray AvailableResolutions;
			if (RHIGetAvailableResolutions(AvailableResolutions, false))
			{
				FScreenResolutionRHI TargetResolution{ SettingsCache.Resolution.X, SettingsCache.Resolution.Y };

				FScreenResolutionRHI ResolutionToApply;

				FScreenResolutionArray MatchingResolutions = AvailableResolutions.FilterByPredicate(B2Predicate_MatchResolution(TargetResolution));
				if (SettingsCache.ScreenMode != EWindowMode::Type::Fullscreen && MatchingResolutions.Num() > 0)
				{
					MatchingResolutions.Sort(B2Predicate_SortByRefreshRate());
					ResolutionToApply = MatchingResolutions.Last();
				}
				else
				{
					AvailableResolutions.Sort(B2Predicate_SortByResolution(1080.f / 1920.f));
					FScreenResolutionArray HighestResolutionsFiltered = AvailableResolutions.FilterByPredicate(B2Predicate_MatchResolution(AvailableResolutions.Last()));

					if (HighestResolutionsFiltered.Num() > 0)
					{
						HighestResolutionsFiltered.Sort(B2Predicate_SortByRefreshRate());
						ResolutionToApply = HighestResolutionsFiltered.Last();
					} 
					else
					{
						ResolutionToApply = AvailableResolutions.Last();
					}
				}

				// Resolution
				GameUserSettings->SetScreenResolution(FIntPoint(ResolutionToApply.Width, ResolutionToApply.Height));

				// Refresh rate
				GameUserSettings->SetFrameRateLimit(ResolutionToApply.RefreshRate);
			}

			// Screen mode
			GameUserSettings->SetFullscreenMode(SettingsCache.ScreenMode);

			// Set Vsync
			GameUserSettings->SetVSyncEnabled(SettingsCache.VSyncOn);

			// AA, Shadows and PP
			GameUserSettings->SetAntiAliasingQuality(SettingsCache.AntiAliasing);
			GameUserSettings->SetShadowQuality(SettingsCache.ShadowQuality);
			GameUserSettings->SetPostProcessingQuality(SettingsCache.PostProcessing);

			// Apply all settings
			GameUserSettings->ConfirmVideoMode();
			GameUserSettings->ApplyResolutionSettings(false);
		}
	}
}

FString USettings::ShortLocaleStringToFull(const FString& ShortLocaleString) const
{
	FString OutString = LOCALE_DEFAULT;

	if (ShortLocaleString.Compare(SHORT_LOCALE_EN) == 0)
	{
		OutString =  LOCALE_EN;
	}
	else if (ShortLocaleString.Compare(SHORT_LOCALE_JP) == 0)
	{
		OutString = LOCALE_JP;
	}

	return OutString;
}

FString USettings::FullLocaleStringToShort(const FString& FullLocaleString) const
{
	FString OutString = SHORT_LOCALE_DEFAULT;

	if (FullLocaleString.Compare(LOCALE_EN) == 0)
	{
		OutString = SHORT_LOCALE_EN;
	}
	else if (FullLocaleString.Compare(LOCALE_JP) == 0)
	{
		OutString = SHORT_LOCALE_JP;
	}

	return OutString;
}
