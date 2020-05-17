#include "B2UI/Tutorial.h"

#include "Internationalization/Internationalization.h"
#include "Internationalization/CulturePointer.h"
#include "Internationalization/Culture.h"

void UTutorial::SetVisible(bool bNewVisible)
{
	SetVisibility(bNewVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UTutorial::NativeOnInitialized()
{
	if (InstructionAnim)
	{
		PlayAnimation(InstructionAnim, 0, 0);
	}

	// Localization for the background image
	FInternationalization::Get().OnCultureChanged().AddLambda([this]() { HandleCultureChange(); });

	HandleCultureChange();
}

void UTutorial::SetPhase(uint32 Phase)
{
	if (OverlaySwitcher)
	{
		if (Phase < static_cast<uint32>(OverlaySwitcher->GetChildrenCount()))
		{
			OverlaySwitcher->SetActiveWidgetIndex(Phase);
		}
	}

	if (ContinuePromptText && IntroBackgroundSwitcher)
	{
		if (Phase == 21)
		{
			IntroBackgroundSwitcher->SetVisibility(ESlateVisibility::Hidden);
			ContinuePromptText->SetVisibility(ESlateVisibility::Hidden);
		}
		else if (Phase == 22)
		{
			SetVisible(false);
		}
		else
		{
			IntroBackgroundSwitcher->SetVisibility(ESlateVisibility::HitTestInvisible);
			ContinuePromptText->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}

void UTutorial::HandleCultureChange()
{
	FCultureRef CurrentCulture = FInternationalization::Get().GetCurrentCulture();
	FString CultureString = CurrentCulture.Get().GetEnglishName();

	// Note - I don't like having to use strings for things like this but I don't think its possible to get any statically typed data 
	// so we have to just pray that this remains constant

	// Also can't switch on strings so we have to use an if else booooo!
	if (CultureString == CULTURE_NAME_EN)
	{
		IntroBackgroundSwitcher->SetActiveWidgetIndex(TEXT_WIDGET_INDEX_EN);
	}
	else if (CultureString == CULTURE_NAME_JP)
	{
		IntroBackgroundSwitcher->SetActiveWidgetIndex(TEXT_WIDGET_INDEX_JP);
	}
}