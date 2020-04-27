#include "B2UI/OptionsMenu.h"

#include "UObject/ConstructorHelpers.h"

#include "B2GameMode/BladeIIGameMode.h"

#include "B2Utility/Log.h"

const FString COMBOBOX_ITEM_BLUEPRINT_PATH = TEXT("WidgetBlueprint'/Game/BladeIIGame/Blueprints/UI/BP_ComboBoxItem'");

UOptionsMenu::UOptionsMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UComboBoxItem> ComboBoxItem(*COMBOBOX_ITEM_BLUEPRINT_PATH);
	if (ComboBoxItem.Succeeded())
	{
		ComboBoxItemClass = ComboBoxItem.Class;
	}

	State = State::MenuClosed;
}

void UOptionsMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterEventListeners();
	
	GameModeInstance = Cast<ABladeIIGameMode>(GetWorld()->GetAuthGameMode());

	LoadStoredValues();
}

void UOptionsMenu::ToggleMenu()
{
	if (State == State::MenuOpen)
	{
		PlayAnimationReverse(ShowHideMenuAnimation);
		State = State::MenuClosed;
	}
	else if (State == State::MenuClosed)
	{
		PlayAnimationForward(ShowHideMenuAnimation);
		State = State::MenuOpen;
	}
	else if (State == State::ModalOpen)
	{
		PlayAnimationReverse(ShowHideModalAnimation);
		State = State::MenuOpen;
	}

	LanguageComboBox->Clear();
}

void UOptionsMenu::ClearLanguageComboBoxFocus()
{
	LanguageComboBox->Clear();
}

void UOptionsMenu::OnMasterVolumeValueChanged(float NewValue)
{
	if (MasterVolumeSliderFill)
	{
		MasterVolumeSliderFill->SetRenderScale(FVector2D(NewValue, 1));
	}

	GameModeInstance->GetSettings()->SetFloatSetting(EFloatSetting::MasterVolume, NewValue);
}

void UOptionsMenu::OnBGMVolumeValueChanged(float NewValue)
{
	if (BGMVolumeSliderFill)
	{
		BGMVolumeSliderFill->SetRenderScale(FVector2D(NewValue, 1));
	}

	GameModeInstance->GetSettings()->SetFloatSetting(EFloatSetting::BGMVolume, NewValue);
}

void UOptionsMenu::OnSFXVolumeValueChanged(float NewValue)
{
	if (SFXVolumeSliderFill)
	{
		SFXVolumeSliderFill->SetRenderScale(FVector2D(NewValue, 1));
	}

	GameModeInstance->GetSettings()->SetFloatSetting(EFloatSetting::SFXVolume, NewValue);
}

void UOptionsMenu::OnLanguageComboBoxValueChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	GameModeInstance->GetSettings()->SetStringSetting(EStringSetting::Language, SelectedItem);
}

FEventReply UOptionsMenu::OnBackgroundClicked(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	ToggleMenu();

	return FEventReply(true);
}

void UOptionsMenu::OnForfeitButtonPressed()
{
	PlayAnimationForward(ShowHideModalAnimation);
	State = State::ModalOpen;

}

UWidget* UOptionsMenu::OnLanguageComboBoxConstructed(FString Item)
{
	UComboBoxItem* ComboBoxItem = CreateWidget<UComboBoxItem>(this, ComboBoxItemClass);
	ComboBoxItem->SetText(FText::FromString(Item));

	return ComboBoxItem;
}

void UOptionsMenu::OnForfeitConfirmButtonPressed()
{
	GameModeInstance->LocalQuit();
}

void UOptionsMenu::OnForfeitCancelButtonPressed()
{
	PlayAnimationReverse(ShowHideModalAnimation);
	State = State::MenuOpen;

}

void UOptionsMenu::RegisterEventListeners()
{
	if (MasterVolumeSlider)
	{
		MasterVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionsMenu::OnMasterVolumeValueChanged);
	}

	if (BGMVolumeSlider)
	{
		BGMVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionsMenu::OnBGMVolumeValueChanged);
	}

	if (SFXVolumeSlider)
	{
		SFXVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionsMenu::OnSFXVolumeValueChanged);
	}

	if (LanguageComboBox)
	{
		LanguageComboBox->OnGenerateWidgetEvent.BindDynamic(this, &UOptionsMenu::OnLanguageComboBoxConstructed);
		LanguageComboBox->OnSelectionChanged.AddDynamic(this, &UOptionsMenu::OnLanguageComboBoxValueChanged);
	}

	if (Darkener)
	{
		Darkener->OnMouseButtonDownEvent.BindDynamic(this, &UOptionsMenu::OnBackgroundClicked);
	}

	if (ForfeitButton)
	{
		ForfeitButton->OnClicked.AddDynamic(this, &UOptionsMenu::OnForfeitButtonPressed);
	}

	if (ModalConfirmForfeitButton)
	{
		ModalConfirmForfeitButton->OnClicked.AddDynamic(this, &UOptionsMenu::OnForfeitConfirmButtonPressed);
	}

	if (ModalCancelForfeitButton)
	{
		ModalCancelForfeitButton->OnClicked.AddDynamic(this, &UOptionsMenu::OnForfeitCancelButtonPressed);
	}
}

void UOptionsMenu::SetSliderValue(Slider Slider, float Value)
{
	// As we have custom fill behaviour, we set the value and then manually call the on change callback handler

	switch (Slider)
	{
	case UOptionsMenu::Slider::MasterVolume:
		MasterVolumeSlider->SetValue(Value);
		OnMasterVolumeValueChanged(Value);
		break;
	case UOptionsMenu::Slider::BGMVolume:
		BGMVolumeSlider->SetValue(Value);
		OnBGMVolumeValueChanged(Value);
		break;
	case UOptionsMenu::Slider::SFXVolume:
		SFXVolumeSlider->SetValue(Value);
		OnSFXVolumeValueChanged(Value);
		break;
	}
}

void UOptionsMenu::LoadStoredValues()
{
	// During init, we manually set the values so that we dont try to re-apply the settings that were just read

	if (MasterVolumeSlider && MasterVolumeSliderFill)
	{
		float Volume = GameModeInstance->GetSettings()->GetFloatSetting(EFloatSetting::MasterVolume);
		MasterVolumeSlider->SetValue(Volume);
		MasterVolumeSliderFill->SetRenderScale(FVector2D(Volume, 1));
	}

	if (BGMVolumeSlider && BGMVolumeSliderFill)
	{
		float Volume = GameModeInstance->GetSettings()->GetFloatSetting(EFloatSetting::BGMVolume);
		BGMVolumeSlider->SetValue(Volume);
		BGMVolumeSliderFill->SetRenderScale(FVector2D(Volume, 1));
	}

	if (SFXVolumeSlider && SFXVolumeSliderFill)
	{
		float Volume = GameModeInstance->GetSettings()->GetFloatSetting(EFloatSetting::SFXVolume);
		SFXVolumeSlider->SetValue(Volume);
		SFXVolumeSliderFill->SetRenderScale(FVector2D(Volume, 1));
	}

	if (LanguageComboBox)
	{
		LanguageComboBox->SetSelectedOption(GameModeInstance->GetSettings()->GetStringSetting(EStringSetting::Language));
	}
}
