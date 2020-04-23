#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ComboBoxString.h"

#include "B2UI/ComboBoxItem.h"

#include "OptionsMenu.generated.h"

UCLASS()
class BLADEIIGAME_API UOptionsMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeOnInitialized() override;

	UOptionsMenu(const FObjectInitializer& ObjectInitializer);

	/* Toggle the options menu between open and closed - does not interrupt animations, so will result in a noop if one is playing */
	void ToggleMenu();

private:

	/* Slider represents different slider controls, so that we can generalise value setters and getters */
	enum class Slider : uint8
	{
		MasterVolume,
		BGMVolume,
		SFXVolume,
	};

	/* State represents the state of the options menu, so we can track what we need to do in response to escape pressed etc */
	enum class State : uint8
	{
		MenuOpen,
		MenuClosed,
		ModalOpen,
	};

	/* Widgets */
	
	/* Master volume slider widget */
	UPROPERTY(meta = (BindWidget))
	USlider* MasterVolumeSlider;

	/* Master volume slider fill widget */
	UPROPERTY(meta = (BindWidget))
	UImage* MasterVolumeSliderFill;

	/* BGM volume slider widget */
	UPROPERTY(meta = (BindWidget))
	USlider* BGMVolumeSlider;

	/* BGM volume slider fill widget */
	UPROPERTY(meta = (BindWidget))
	UImage* BGMVolumeSliderFill;

	/* SFX volume slider widget */
	UPROPERTY(meta = (BindWidget))
	USlider* SFXVolumeSlider;

	/* SFX volume slider fill widget */
	UPROPERTY(meta = (BindWidget))
	UImage* SFXVolumeSliderFill;

	/* Language combo box widget */
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* LanguageComboBox;

	/* Forfeit button widget */
	UPROPERTY(meta = (BindWidget))
	UButton* ForfeitButton;

	/* Darkener image widget */
	UPROPERTY(meta = (BindWidget))
	UImage* Darkener;

	/* Confirm forfeit button widget */
	UPROPERTY(meta = (BindWidget))
	UButton* ModalConfirmForfeitButton;

	/* Cancel forfeit button widget */
	UPROPERTY(meta = (BindWidget))
	UButton* ModalCancelForfeitButton;

	/* Animations */

	/* Open Animation */
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* OpenAnimation;

	/* Close Animation */
	UPROPERTY (meta = (BindWidgetAnim))
	UWidgetAnimation* CloseAnimation;

	/* Open Modal Animation */
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* ModalOpenAnimation;

	/* Close Modal Animation */
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* ModalCloseAnimation;

	/* Combo box item class */
	TSubclassOf<UComboBoxItem> ComboBoxItemClass;

	/* Reference to the game mode instance */
	class ABladeIIGameMode* GameModeInstance;

	/* Whether or not the menu is in the open (visible) state or not */
	State State;

	/* Register the event listeners for this widget */
	void RegisterEventListeners();

	/* Callback handler for when the master volume slider's value changes */
	UFUNCTION()
	void OnMasterVolumeValueChanged(float NewValue);

	/* Callback handler for when the BGM volume slider's value changes */
	UFUNCTION()
	void OnBGMVolumeValueChanged(float NewValue);

	/* Callback handler for when the SFX volume slider's value changes */
	UFUNCTION()
	void OnSFXVolumeValueChanged(float NewValue);

	/* Callback handler for when the language combo box's value changes */
	UFUNCTION()
	void OnLanguageComboBoxValueChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	/* Callback handler for when the background is clicked */
	UFUNCTION()
	FEventReply OnBackgroundClicked(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	/* Callback handler for when the forfeit button is pressed */
	UFUNCTION()
	void OnForfeitButtonPressed();

	/* Callback handler for when the language combobox is constructed */
	UFUNCTION()
	UWidget* OnLanguageComboBoxConstructed(FString Item);

	/* Callback handler for when the forfeit confirm button is pressed */
	UFUNCTION()
	void OnForfeitConfirmButtonPressed();

	/* Callback handler for when the forfeit cancel button is pressed */
	UFUNCTION()
	void OnForfeitCancelButtonPressed();

	/**
	 * Set the value for a particular slider
	 * @param Slider - The slider for which the value will be changed
	 * @param Value - The new slider value (0-1)
	 */
	void SetSliderValue(Slider Slider, float Value);

	/* Reads the stored settings from the settings class and applies them to their respective controls */
	void LoadStoredValues();
};
