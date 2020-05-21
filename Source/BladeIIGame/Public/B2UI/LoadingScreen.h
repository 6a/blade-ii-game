#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

#include "LoadingScreen.generated.h"

UCLASS()
class BLADEIIGAME_API ULoadingScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// Helper enums for setting loading bar values
	enum class LoadingBar
	{
		Connecting,
		PreparingMatch,
		StartingMatch,
	};

	/**
	 * Initialise the loading screen
	 * @param bIsVersusAI - Whether or not this game is vs the AI (if true, we do a fake load)
	 */
	void Initialise(bool bIsVersusAI);

	/**
	 * Set the progress for the specified loading bar. Note that, setting the progress for a bar will
	 * auto-fill all the bars prior, if not already full:
	 * 1. Connecting
	 * 2. PreparingMatch
	 * 3. StartingMatch
	 * @param Target - The loading bar to set
	 * @param Value - The new progress value (0-1)
	 */
	void SetProgress(LoadingBar Target, float Value);

protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	/* For building string loading bars */
	const TCHAR LOADING_BAR_END_LEFT = L'[';
	const TCHAR LOADING_BAR_END_RIGHT = L']';
	const TCHAR LOADING_BAR_FILL = L'#';
	const TCHAR LOADING_BAR_SPACE = L'-';
	const TCHAR CHAR_BLANK = L' ';

	/* For output text */
	const FString TEXT_CONNECTING = "Connecting to Server";
	const FString TEXT_PREPARING_MATCH = "Preparing Match";
	const FString TEXT_STARTING_MATCH = "Starting Match";
	const TArray<FString> STARTING_MATCH_LOADING_CHARS = { "|", "/", "-", "\\", "|", "/", "-", "\\" };

	/* Limits */
	const uint32 CONSOLE_WIDTH = 60;
	const float PROGRESS_MAX = 4;
	const float AUTO_LOAD_TIME = 4;

	/* For auto load */
	const float LOAD_TIME_PER_LINE = 1.f;
	float OverallProgress;
	float NextAutoProgressBarStartTime;
	uint32 LoadingPhase;

	/* For fading the screen out after load */
	const float FADE_ALPHA_MAX = 1.f;
	const float ALPHA_FADE_TIME = 1.f;
	const float FADE_EASE_EXP = 2.f;
	float FadeAlpha;

	/* Pointer to the game mode instance */
	class ABladeIIGameMode* GameModeInstance;

	/* Whether or not we should increment the loading progress display automatically (such as for an AI game or when the first two bars are full */
	bool bAutoLoad;

	/* Whether or not loading progress has changed, denoting that the UI should be re-drawn with new values */
	bool bUIRequiresUpdate;

	/* Version text widget */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* VersionText;

	/* Connecting to server text widget */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextConnecting;

	/* Preparing match text widget */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextPreparingMatch;

	/* Starting match text widget */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextStartingMatch;

	/**
	 * Returns the source + progress bar for the source string, fitted to the console width
	 * @param Source - The base string for this line
	 * @param Progress - Percentage (0 to 1) of the progress bar
	 */
	FString AddLoadingBar(const FString& Source, float Progress) const;

	/**
	 * Returns the source + spinner for the source string
	 * @param Source - The base string for this line
	 * @param Progress - Percentage (0 to 1) Rotation of the spinner (0 = 0 degrees, 0.5 = 180 degrees etc)
	 */
	FString AddSpinner(const FString& Source, float Progress) const;
};
