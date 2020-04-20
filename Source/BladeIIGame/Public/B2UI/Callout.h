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
	 * Sets the text for, and shows this callout
	 * @param NewText - The text to display
	 */
	void SetText(const FString& NewText);

	/* Returns true if the callout is currently animating text */
	bool IsAnimatingText() const;

	/* Returns true if the callout is currently active (animating text, waiting, or fading out) */
	bool IsActive() const;

private:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	/* Text widget */
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* CalloutText;

	/* Used to make the textblock the correct size when the animation starts */
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* SizeText;

	/* Text animation handle */
	FTimerHandle TextAnimationHandle;

	/* The string that we are currently setting for the text block */
	FString TargetText;

	/* The current Index for the text animation - as in, how many characters of the full message we are now displaying */
	uint32 TargetTextIndex;

	/* Set to true when fading the callout out */
	bool bIsFadingOut;

	float FadeStartTime;
	/* The time at which the fade out can be started */

	/* Alpha (lerp) value for the current fade out */
	float CurrentFadeAlpha;

	/* Function that adds one character to the text field each call, until completion */
	UFUNCTION()
	void ProgressTextAnimation();
};
