#pragma once

#include "CoreMinimal.h"

#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"

#include "B2UI/EffectWidget.h"

#include "EffectWidgetEndgame.generated.h"

UCLASS()
class BLADEIIGAME_API UEffectWidgetEndgame : public UEffectWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;

private:

	/* For dealing with localisation */
	const FString CULTURE_NAME_EN = TEXT("English");
	const FString CULTURE_NAME_JP = TEXT("Japanese (Japan)");
	const uint8 TEXT_WIDGET_INDEX_EN = 0;
	const uint8 TEXT_WIDGET_INDEX_JP = 1;

	/* Pointer to the game mode */
	class ABladeIIGameMode* GameModeInstance;

	/* Text image switcher */
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* Switcher;

	/* Text image switcher */
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	/* Run the animation for this widget */
	virtual void RunAnimation() override;

	void HandleCultureChange();

	UFUNCTION()
	void HandleQuitButtonClicked();
};
