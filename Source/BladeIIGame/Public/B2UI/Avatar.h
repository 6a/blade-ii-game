#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "B2UI/Callout.h"
#include "B2Enum/OpponentMessageEnum.h"

#include "Avatar.generated.h"

UCLASS()
class BLADEIIGAME_API UAvatar : public UUserWidget
{
	GENERATED_BODY()

public:

	/**
	 * Sets the opponent callout to the specifed text
	 * @param Message - The message to play
	 * @return how long the animation will take (including the time it will stay visible once the animation has finished
	 */
	float SetOpponentMessage(EOpponentMessage Message);
	
private:

	/* Render image widget */
	UPROPERTY(meta = (BindWidget))
	UImage* RenderImage;

	/* Nameplate text widget */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameplateText;

	/* Callout widget */
	UPROPERTY(meta = (BindWidget))
	UCallout* Callout;
};
