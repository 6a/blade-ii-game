#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

#include "Callout.generated.h"

UCLASS()
class BLADEIIGAME_API UCallout : public UUserWidget
{
	GENERATED_BODY()

public:

	/**
	 * Set the internal reference to the game mode instance
	 * @param GameModeInstance - A pointer to the game mode instance
	 */
	void SetGameModeInstance(class ABladeIIGameMode* GameMode);

	/**
	 * Sets the text for, and shows this callout
	 * @param NewText - The text to display
	 */
	void SetText(const FString& NewText);

	/* Returns true if the callout is currently animating text */
	bool IsAnimatingText() const;

	/* Returns true if the callout is currently active (animating text, waiting, or fading out) */
	bool IsActive() const;

private:
	/* Class consts */
	/* How long to wait between each character being added */
	const float TEXT_ANIMATION_TICK = 0.05f;

	/* How long to wait after the text has finished animating */
	const float TEXT_ANIMATION_POST_WAIT = 1.0f;

	/* How long the fade out takes */
	const float TEXT_FADE_DURATION = 0.5f;

	/* The maximum value that we can have when lerping between 0 and 1 */
	const float LERP_MAX = 1;

	/* Ease exponent for fading the callout out */
	const float EASE_EXP = 2.f;

	/* Text widget */
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* CalloutText;

	/* Used to make the textblock the correct size when the animation starts */
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* SizeText;

	/* Pointer to the game mode instance */
	class ABladeIIGameMode* GameModeInstance;

	/* Text animation handle */
	FTimerHandle TextAnimationHandle;

	/* The string that we are currently setting for the text block */
	FString TargetText;

	/* The current Index for the text animation - as in, how many characters of the full message we are now displaying */
	uint32 TargetTextIndex;

	/* Set to true when fading the callout out */
	bool bIsFadingOut;

	/* The time at which the fade out can be started */
	float FadeStartTime;

	/* Alpha (lerp) value for the current fade out */
	float CurrentFadeAlpha;

	/* Used to restrict how often the callout text sound is played */
	uint32 CurrentCalloutTextSoundIndex;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	/* Function that adds one character to the text field each call, until completion */
	UFUNCTION()
	void ProgressTextAnimation();
};
