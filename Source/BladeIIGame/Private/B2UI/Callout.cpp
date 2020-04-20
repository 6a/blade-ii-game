#include "B2UI/Callout.h"

#include "B2Utility/Log.h"

#include "TimerManager.h"

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

float UCallout::SetText(const FString& NewText)
{
	if (GetWorld()->GetTimerManager().TimerExists(TextAnimationHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(TextAnimationHandle);
	}

	TargetText = NewText;
	TargetTextIndex = 0;
	SetRenderOpacity(1);
	CurrentFadeAlpha = 1;

	// Empty the text field, replacing it with a string of spaces the same length 
	CalloutText->SetText(FText::FromString(""));

	// Fill the sizing text with the incoming text to set the size of the callout
	SizeText->SetText(FText::FromString(TargetText));

	GetWorld()->GetTimerManager().SetTimer(TextAnimationHandle, this, &UCallout::ProgressTextAnimation, TEXT_ANIMATION_TICK, true, 0);

	return TargetText.Len() * TEXT_ANIMATION_TICK + TEXT_ANIMATION_POST_WAIT + TEXT_FADE_DURATION;
}

void UCallout::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (bIsFadingOut && GetWorld()->GetTimeSeconds() >= FadeStartTime)
	{
		float Step = LERP_MAX / (TEXT_FADE_DURATION / InDeltaTime);

		CurrentFadeAlpha = FMath::Clamp(CurrentFadeAlpha - Step, 0.f, LERP_MAX);

		float NewOpacity = FMath::InterpEaseInOut(0.f, LERP_MAX, CurrentFadeAlpha, EASE_EXP);

		SetRenderOpacity(NewOpacity);

		if (NewOpacity <= 0)
		{
			bIsFadingOut = false;
		}
	}
}

void UCallout::ProgressTextAnimation()
{
	bool bFirstCharacterAdded = false;

	while (!bFirstCharacterAdded || TargetText.Mid(TargetTextIndex, 1).Compare(TEXT(" ")) == 0)
	{
		bFirstCharacterAdded = true;

		if (++TargetTextIndex > static_cast<uint32>(TargetText.Len()))
		{
			if (GetWorld()->GetTimerManager().TimerExists(TextAnimationHandle))
			{
				GetWorld()->GetTimerManager().ClearTimer(TextAnimationHandle);
				bIsFadingOut = true;
				FadeStartTime = GetWorld()->GetTimeSeconds() + TEXT_ANIMATION_POST_WAIT;
				return;
			}
		}
	}

	FString CurrentString = FString(TargetTextIndex, *TargetText);

	CalloutText->SetText(FText::FromString(CurrentString));
}