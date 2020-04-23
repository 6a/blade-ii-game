#include "B2UI/OptionsMenu.h"

#include "UObject/ConstructorHelpers.h"

#include "B2GameMode/BladeIIGameMode.h"

#include "B2Utility/Log.h"

void UOptionsMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GameModeInstance = Cast<ABladeIIGameMode>(GetWorld()->GetAuthGameMode());

	RegisterEventListeners();

	LoadStoredValues();

	bIsOpen = false;
}

UOptionsMenu::UOptionsMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UComboBoxItem> ComboBoxItem(TEXT("WidgetBlueprint'/Game/BladeIIGame/Blueprints/UI/BP_ComboBoxItemItem'"));
	if (ComboBoxItem.Succeeded())
	{
		ComboBoxItemClass = ComboBoxItem.Class;
	}
}

void UOptionsMenu::ToggleMenu()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if (!IsAnyAnimationPlaying())
	{
		if (bIsOpen)
		{
			PlayAnimation(CloseAnimation);

			if (PlayerController)
			{
				PlayerController->bShowMouseCursor = false;
				PlayerController->bEnableMouseOverEvents = false;

				FInputModeGameOnly InputMode;
				PlayerController->SetInputMode(InputMode);
			}
		}
		else
		{
			PlayAnimation(OpenAnimation);

			if (PlayerController)
			{
				PlayerController->bShowMouseCursor = true;
				PlayerController->bEnableMouseOverEvents = true;

				FInputModeGameAndUI InputMode;
				InputMode.SetWidgetToFocus(nullptr);
				InputMode.SetHideCursorDuringCapture(false);
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PlayerController->SetInputMode(InputMode);
			}
		}

		bIsOpen = !bIsOpen;
	}
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

}

UWidget* UOptionsMenu::OnLanguageComboBoxConstructed(FString Item)
{
	UComboBoxItem* ComboBoxItem = CreateWidget<UComboBoxItem>(this, ComboBoxItemClass);
	ComboBoxItem->SetText(FText::FromString(Item));

	return ComboBoxItem;
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
}

void UOptionsMenu::SetSliderValue(Slider Slider, float Value)
{
	// As we have custom fill behaviour, we set the value and then manuallycall the on change callback handler

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
