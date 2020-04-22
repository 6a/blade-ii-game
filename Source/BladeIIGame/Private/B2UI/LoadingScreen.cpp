#include "B2UI/LoadingScreen.h"

#include "B2GameMode/BladeIIGameMode.h"

// TODO set to zero for build
#define FAST_LOAD 1

void ULoadingScreen::Initialise(bool bIsVersusAI)
{
	GameModeInstance = Cast<ABladeIIGameMode>(GetWorld()->GetAuthGameMode());

	VersionText->SetText(FText::FromString(FString(TEXT_VERSION_PREFIX).Append(GameModeInstance->GetSettings()->GetStringSetting(EStringSetting::Version))));

	TextInitialising->SetText(FText::FromString(AddLoadingBar(TEXT_INITIALIZING, 0)));
	TextConnecting->SetText(FText());
	TextPreparingMatch->SetText(FText());
	TextStartingMatch->SetText(FText());

	if (FAST_LOAD)
	{
		SetRenderOpacity(0);
		SetVisibility(ESlateVisibility::Hidden);
		GameModeInstance->AutoLoadFinished();
	}
	else
	{
		LoadAlpha = 0;
		FadeAlpha = 1;
		AutoLoadPhase = 0;
		NextAutoProgressBarStartTime = GetWorld()->GetTimeSeconds() + 0.4f;

		bAutoLoad = bIsVersusAI;
	}
}

void ULoadingScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (!FAST_LOAD && bAutoLoad && GetWorld()->GetTimeSeconds() >= NextAutoProgressBarStartTime)
	{
		if (LoadAlpha >= LOAD_ALPHA_MAX)
		{
			if (FadeAlpha <= 0)
			{
				bAutoLoad = false;
				SetVisibility(ESlateVisibility::Hidden);
				GameModeInstance->AutoLoadFinished();

			}
			else
			{
				float Step = FADE_ALPHA_MAX / ((ALPHA_FADE_TIME) / InDeltaTime);

				FadeAlpha = FMath::Clamp(FadeAlpha - Step, 0.f, FADE_ALPHA_MAX);

				float FadeAlphaEased = FMath::InterpEaseInOut(0.f, FADE_ALPHA_MAX, FadeAlpha, FADE_EASE_EXP);

				SetRenderOpacity(FadeAlphaEased);
			}
		}
		else
		{
			float Step = LOAD_ALPHA_MAX / ((AUTO_LOAD_TIME) / InDeltaTime);

			LoadAlpha = FMath::Clamp(LoadAlpha + Step, 0.f, LOAD_ALPHA_MAX);

			// Initialising
			if (LoadAlpha > 0)
			{
				if (AutoLoadPhase == 0)
				{
					AutoLoadPhase = 1;
					NextAutoProgressBarStartTime = GetWorld()->GetTimeSeconds() + 0.4f;
				}

				float Fill = FMath::Clamp(LoadAlpha, 0.f, 1.f);
				TextInitialising->SetText(FText::FromString(AddLoadingBar(TEXT_INITIALIZING, Fill)));
			}

			// Connecting to server
			if (LoadAlpha > 1)
			{
				if (AutoLoadPhase == 1)
				{
					AutoLoadPhase = 2;
					NextAutoProgressBarStartTime = GetWorld()->GetTimeSeconds() + 0.4f;
				}

				float Fill = FMath::Clamp(LoadAlpha - 1, 0.f, 1.f);
				TextConnecting->SetText(FText::FromString(AddLoadingBar(TEXT_CONNECTING, Fill)));
			}

			// Preparing match
			if (LoadAlpha > 2)
			{
				if (AutoLoadPhase == 2)
				{
					AutoLoadPhase = 3;
					NextAutoProgressBarStartTime = GetWorld()->GetTimeSeconds() + 0.4f;
				}

				float Fill = FMath::Clamp(LoadAlpha - 2, 0.f, 1.f);
				TextPreparingMatch->SetText(FText::FromString(AddLoadingBar(TEXT_PREPARING_MATCH, Fill)));
			}

			// Starting match
			if (LoadAlpha > 3)
			{
				float Fill = FMath::Clamp((LoadAlpha - 3), 0.f, 2.f);
				TextStartingMatch->SetText(FText::FromString(AddSpinner(TEXT_STARTING_MATCH, Fill)));
			}
		}
	}
}

FString ULoadingScreen::AddLoadingBar(const FString& Source, float Progress) const
{
	// Console width - source stringth width - 1 character for a space - 2 characters for start and end block for the progress bar
	const uint32 BarSize = CONSOLE_WIDTH - Source.Len() - 3;
	const uint32 BarFillSize = FMath::FloorToInt(Progress * static_cast<float>(BarSize));
	const uint32 BarEmptySize = BarSize - BarFillSize;

	// The filled part of the bar
	const FString BarFillCharacters = FString::ChrN(BarFillSize, LOADING_BAR_FILL);

	// The empty part of the bar
	const FString BarEmptyCharacters = FString::ChrN(BarEmptySize, LOADING_BAR_SPACE);

	FString LoadingBar = FString(BarFillCharacters + BarEmptyCharacters);
	LoadingBar.InsertAt(0, LOADING_BAR_END_LEFT);
	LoadingBar.AppendChar(LOADING_BAR_END_RIGHT);

	return FString::Printf(TEXT("%s %s"), *Source, *LoadingBar);
}

FString ULoadingScreen::AddSpinner(const FString& Source, float Progress) const
{
	uint32 CharIndex = FMath::FloorToInt(STARTING_MATCH_LOADING_CHARS.Num() * Progress) % STARTING_MATCH_LOADING_CHARS.Num();
	FString SpinnerChar = STARTING_MATCH_LOADING_CHARS[CharIndex];

	return FString::Printf(TEXT("%s %s"), *Source, *SpinnerChar);
}
