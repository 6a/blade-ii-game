#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Components/Image.h"

#include "OptionsMenu.generated.h"

UCLASS()
class BLADEIIGAME_API UOptionsMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:

private:
	
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

	/* Forfeit button widget */
	UPROPERTY(meta = (BindWidget))
	UButton* ForfeitButton;
};
