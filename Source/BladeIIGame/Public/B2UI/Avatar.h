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
	 * @param Character - String representation of the character for which the corresponding message should be fetched
	 */
	void SetOpponentMessage(EOpponentMessage Message, const FString& Character);
	
	/* Returns true if the callout is currently animating text */
	bool IsAnimatingCalloutText() const;

	/* Returns true if the callout is currently active (animating text, waiting, or fading out) */
	bool IsCalloutActive() const;

private:

	/* Pointer to the game mode instance */
	class ABladeIIGameMode* GameModeInstance;

	/* Render image widget */
	UPROPERTY(meta = (BindWidget))
	UImage* RenderImage;

	/* Nameplate text widget */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameplateText;

	/* Callout widget */
	UPROPERTY(meta = (BindWidget))
	UCallout* Callout;

	/**
	 * Gets the corresponding message for the specified parameters as a text object, catering for localisation
	 * @param Message - The message to play
	 * @param Character - String representation of the character for which the corresponding message should be fetched
	 * @return the corresponding text object
	 */
	const FText GetTextForMessage(EOpponentMessage Message, const FString& Character) const;

	/**
	 * Returns the specified message as a string
	 * @param Message - The message type to convert
	 * @return The message type as a string
	 */
	FString MessageTypeToString(EOpponentMessage Message) const;
};
