#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

#include "ToolTip.generated.h"

UCLASS()
class BLADEIIGAME_API UToolTip : public UUserWidget
{
	GENERATED_BODY()
	

public:
		
	/**
	 * Update the text values for the tooltip
	 * @param Header - Text to set for the tooltip's header
	 * @param Information - Text to set for the tooltip's information section
	 */
	void SetText(const FText& Header, const FText& Information);

	/**
	 * Show/Hide the tooltip
	 * @param bNewVisible - Whether the tooltip should be visible or not after the function call
	 */
	void SetActive(bool bNewVisible);

private:

	/* Header text */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HeaderText;

	/* Information text */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InformationText;
};
