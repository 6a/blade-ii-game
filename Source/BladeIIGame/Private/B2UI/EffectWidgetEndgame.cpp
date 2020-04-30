#include "B2UI/EffectWidgetEndgame.h"

#include "Internationalization/Internationalization.h"
#include "Internationalization/CulturePointer.h"
#include "Internationalization/Culture.h"

#include "B2GameMode/BladeIIGameMode.h"

void UEffectWidgetEndgame::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GameModeInstance = Cast<ABladeIIGameMode>(GetWorld()->GetAuthGameMode());

	FInternationalization::Get().OnCultureChanged().AddLambda([this]() { HandleCultureChange(); });

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UEffectWidgetEndgame::HandleQuitButtonClicked);
	}
}

void UEffectWidgetEndgame::RunAnimation()
{
	RunAnimationAtTargetScreenPosition();
}

void UEffectWidgetEndgame::HandleCultureChange()
{
	FCultureRef CurrentCulture = FInternationalization::Get().GetCurrentCulture();
	FString CultureString = CurrentCulture.Get().GetEnglishName();

	// Note - I don't like having to use strings for things like this but I don't think its possible to get any statically typed data 
	// so we have to just pray that this remains constant

	// Also can't switch on strings so we have to use an if else booooo!
	if (CultureString == CULTURE_NAME_EN)
	{
		Switcher->SetActiveWidgetIndex(TEXT_WIDGET_INDEX_EN);
	}
	else if (CultureString == CULTURE_NAME_JP)
	{
		Switcher->SetActiveWidgetIndex(TEXT_WIDGET_INDEX_JP);
	}
}

void UEffectWidgetEndgame::HandleQuitButtonClicked()
{
	if (QuitButton)
	{
		QuitButton->OnClicked.Clear();
		GameModeInstance->LocalQuit(false);
	}
}
