#include "B2UI/LoadingScreen.h"

#include "B2GameMode/BladeIIGameMode.h"

// TODO set to zero for build
#define FAST_LOAD 0

void ULoadingScreen::Initialise(bool bIsVersusAI)
{
	GameModeInstance = Cast<ABladeIIGameMode>(GetWorld()->GetAuthGameMode());

	VersionText->SetText(FText::FromString(FString(TEXT_VERSION_PREFIX).Append(GameModeInstance->GetSettings()->GetStringSetting(EStringSetting::Version))));

	TextConnecting->SetText(FText());
	TextPreparingMatch->SetText(FText());
	TextStartingMatch->SetText(FText());

	if (FAST_LOAD)
	{
		SetRenderOpacity(0);
		SetVisibility(ESlateVisibility::Hidden);
		GameModeInstance->LoadingFinished();
	}
	else
	{
		OverallProgress = 0;
		FadeAlpha = 1;
		LoadingPhase = 0;
		NextAutoProgressBarStartTime = GetWorld()->GetTimeSeconds() + 0.4f;
		bUIRequiresUpdate = true;

		bAutoLoad = bIsVersusAI;
	}
}

void ULoadingScreen::SetProgress(LoadingBar Target, float Value)
{
	// Clamp value to prevent any incorrect values
	Value = FMath::Clamp(Value, 0.f, 1.f);

	float NewAlpha = 0;

	switch (Target)
	{
	case LoadingBar::Connecting:
		NewAlpha = Value;
		break;
	case LoadingBar::PreparingMatch:
		NewAlpha = 1 + Value;
		break;
	}

	if (NewAlpha > OverallProgress)
	{
		bUIRequiresUpdate = true;
		OverallProgress = NewAlpha;
	}
}

void ULoadingScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (!FAST_LOAD)
	{
		if (OverallProgress >= PROGRESS_MAX)
		{
			if (FadeAlpha <= 0)
			{
				bAutoLoad = false;
				SetVisibility(ESlateVisibility::Hidden);
				GameModeInstance->LoadingFinished();

			}
			else
			{
				float Step = FADE_ALPHA_MAX / ((ALPHA_FADE_TIME) / InDeltaTime);

				FadeAlpha = FMath::Clamp(FadeAlpha - Step, 0.f, FADE_ALPHA_MAX);

				float FadeAlphaEased = FMath::InterpEaseInOut(0.f, FADE_ALPHA_MAX, FadeAlpha, FADE_EASE_EXP);

				SetRenderOpacity(FadeAlphaEased);
			}
		}

		if (bAutoLoad || OverallProgress >= 2)
		{
			if (GetWorld()->GetTimeSeconds() >= NextAutoProgressBarStartTime)
			{
				float Step = PROGRESS_MAX / ((AUTO_LOAD_TIME) / InDeltaTime);
				OverallProgress = FMath::Clamp(OverallProgress + Step, 0.f, PROGRESS_MAX);
				bUIRequiresUpdate = true;
			}
		}

		if (bUIRequiresUpdate)
		{
			bUIRequiresUpdate = false;

			// Connecting to server
			if (OverallProgress > 0)
			{
				float Fill = FMath::Clamp(OverallProgress, 0.f, 1.f);
				TextConnecting->SetText(FText::FromString(AddLoadingBar(TEXT_CONNECTING, Fill)));
			}

			// Preparing match
			if (OverallProgress >= 1)
			{
				if (LoadingPhase == 0)
				{
					LoadingPhase = 1;
					if (bAutoLoad) NextAutoProgressBarStartTime = GetWorld()->GetTimeSeconds() + 0.4f;
				}

				float Fill = FMath::Clamp(OverallProgress - 1, 0.f, 1.f);
				TextPreparingMatch->SetText(FText::FromString(AddLoadingBar(TEXT_PREPARING_MATCH, Fill)));
			}

			// Starting match
			if (OverallProgress >= 2)
			{
				float Fill = FMath::Clamp((OverallProgress - 2), 0.f, 2.f);
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
