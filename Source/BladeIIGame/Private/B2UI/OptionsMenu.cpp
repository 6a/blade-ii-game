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

	bIsOpen = true;
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
	if (!IsAnyAnimationPlaying())
	{
		if (bIsOpen)
		{
			PlayAnimation(CloseAnimation);
		}
		else
		{
			PlayAnimation(OpenAnimation);
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
}

void UOptionsMenu::OnBGMVolumeValueChanged(float NewValue)
{
	if (BGMVolumeSliderFill)
	{
		BGMVolumeSliderFill->SetRenderScale(FVector2D(NewValue, 1));
	}
}

void UOptionsMenu::OnSFXVolumeValueChanged(float NewValue)
{
	if (SFXVolumeSliderFill)
	{
		SFXVolumeSliderFill->SetRenderScale(FVector2D(NewValue, 1));
	}
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
	if (MasterVolumeSlider)
	{
		SetSliderValue(Slider::MasterVolume, GameModeInstance->GetSettings()->GetFloatSetting(EFloatSetting::MasterVolume));
	}

	if (BGMVolumeSlider)
	{
		SetSliderValue(Slider::BGMVolume, GameModeInstance->GetSettings()->GetFloatSetting(EFloatSetting::BGMVolume));
	}

	if (SFXVolumeSlider)
	{
		SetSliderValue(Slider::SFXVolume, GameModeInstance->GetSettings()->GetFloatSetting(EFloatSetting::SFXVolume));
	}
}
