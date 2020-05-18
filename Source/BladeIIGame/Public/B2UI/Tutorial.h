#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"

#include "Tutorial.generated.h"

UCLASS()
class BLADEIIGAME_API UTutorial : public UUserWidget
{
	GENERATED_BODY()
	

public:

	/**
	 * Set the tutorial UI phase.
	 * @param Phase - The current phase of the tutorial. For details see the BP_Tutorial blueprint implementation.
	 */
	void SetPhase(uint32 Phase);

	/**
	 * Toggle the visiblity for the tutorial overlay.
	 * @param bNewVisible - Whether or not the overlay will be visible after the function call.
	 */
	void SetVisible(bool bNewVisible);

private:

	/* For dealing with localisation */
	const FString CULTURE_NAME_EN = TEXT("English");
	const FString CULTURE_NAME_JP = TEXT("Japanese (Japan)");
	const uint8 TEXT_WIDGET_INDEX_JP = 0;
	const uint8 TEXT_WIDGET_INDEX_EN = 1;

	/* The last phase of the tutorial, after which we hide the tutorial overlay */
	const uint32 TotalPhases = 22;

	virtual void NativeOnInitialized() override;

	/* Handler for culture change events */
	void HandleCultureChange();

	/* Text field for the continue prompt */
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* ContinuePromptText;

	/* Overlay widget switcher */
	UPROPERTY(meta = (BindWidgetOptional))
	UWidgetSwitcher* OverlaySwitcher;

	/* Introduction background widget switcher */
	UPROPERTY(meta = (BindWidgetOptional))
	UWidgetSwitcher* IntroBackgroundSwitcher;

	/* Pointer to the instruction animation */
	UPROPERTY(meta = (BindWidgetAnimOptional))
	class UWidgetAnimation* InstructionAnim;
};
