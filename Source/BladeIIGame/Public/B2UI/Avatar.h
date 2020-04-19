#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "Avatar.generated.h"

UCLASS()
class BLADEIIGAME_API UAvatar : public UUserWidget
{
	GENERATED_BODY()
	
private:

	/* Render image widget */
	UPROPERTY(meta = (BindWidget))
	UImage* RenderImage;

	/* Nameplate text widget */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameplateText;
};
