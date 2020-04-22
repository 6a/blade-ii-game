#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

#include "ComboBoxItem.generated.h"

UCLASS()
class BLADEIIGAME_API UComboBoxItem : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/**
	 * Set the text for this item
	 * @param Text - The text to set
	 */
	void SetText(FText Value);

private:

	/* Text field for this item */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text;
};
