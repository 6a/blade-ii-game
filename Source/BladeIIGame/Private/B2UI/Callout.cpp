#include "B2UI/Callout.h"

#include "B2Utility/Log.h"
#include "B2GameMode/BladeIIGameMode.h"

#include "TimerManager.h"

void UCallout::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GameModeInstance = Cast<ABladeIIGameMode>(GetWorld()->GetAuthGameMode());
}

void UCallout::SetText(const FString& NewText)
{
	if (GetWorld()->GetTimerManager().TimerExists(TextAnimationHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(TextAnimationHandle);
	}

	TargetText = NewText;
	TargetTextIndex = 0;
	SetRenderOpacity(1);
	CurrentFadeAlpha = 1;
	bIsFadingOut = false;

	// Empty the text field, replacing it with a string of spaces the same length 
	CalloutText->SetText(FText::FromString(""));

	// Fill the sizing text with the incoming text to set the size of the callout
	SizeText->SetText(FText::FromString(TargetText));

	GetWorld()->GetTimerManager().SetTimer(TextAnimationHandle, this, &UCallout::ProgressTextAnimation, TEXT_ANIMATION_TICK, true, 0);
}

bool UCallout::IsAnimatingText() const
{
	return GetWorld()->GetTimerManager().TimerExists(TextAnimationHandle);
}

bool UCallout::IsActive() const
{
	bool bIsWaitingForFadeOut = GetWorld()->GetTimeSeconds() < FadeStartTime;

	return (IsAnimatingText() || bIsWaitingForFadeOut || bIsFadingOut);
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

	GameModeInstance->GetGameSound()->PlaySFX(ESFX::TextPulse);
}