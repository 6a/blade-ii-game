#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Overlay.h"

#include "B2Enum/ResponseTypeEnum.h"

#include "ErrorModal.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FModalCloseDelegate);

UCLASS()
class BLADEIIGAME_API UErrorModal : public UUserWidget
{
	GENERATED_BODY()

public:
	enum class ErrorType : uint8
	{
		UnableToConnect,
		Disconnected,
		OpponentForfeit,
		MutualTimeOut,
		TimeOut,
	};

	/* Event for when the non-quit button is pressed (quit button handled internally) */
	FModalCloseDelegate OnButtonPressed;

	UErrorModal(const FObjectInitializer& ObjectInitializer);

	/**
	 * Sets the modal to the specified error type state
	 * @param Mode - The new button mode
	 */
	void SetErrorType(ErrorType Error);

	/**
	 * Shows or hides the error modal
	 * @param bNewVisible - Whether the modal will be visible or not after the function call
	 */
	void SetActive(bool bNewVisible);

	/* Returns true if the error modal is currently active */
	bool IsActive() const;

protected:

	virtual void NativeOnInitialized() override;

private:

	/* The ftext objects for the UI */
	FText LocalisedTextUnableToConnect;
	FText LocalisedTextDisconnected;
	FText LocalisedTextOpponentForfeit;
	FText LocalisedTextMutualTimeOut;
	FText LocalisedTextTimeOut;

	FText LocalisedTextCheckNet;
	FText LocalisedTextVictorious;
	FText LocalisedTextMutualTimeOutInfo;
	FText LocalisedTextTimeOutInfo;

	/* Reference to the game mode instance */
	class ABladeIIGameMode* GameModeInstance;

	UPROPERTY(meta = (BindWidget))
	UOverlay* ModalWrapper;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MessageText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* InfoText;

	UPROPERTY(meta = (BindWidget))
	UButton* PositiveButton;

	UPROPERTY(meta = (BindWidget))
	UButton* NegativeButton;

	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* ShowHideAnimation;

	/* Event handler for positive button click */
	UFUNCTION()
	void HandlePositiveButtonClicked();

	/* Event handler for negative button click */
	UFUNCTION()
	void HandleNegativeButtonClicked();

	/* Helper function for hooking up event listeners */
	void RegisterEventListeners();

	/* Helper function for loading in all the localisation text objects */
	void SetupLocalization();

	/**
	 * Attempts to find the localised text object specified by the key, and writes it to OutTarget
	 * @param OutTarget - The ftext object that will be written to if successfully found
	 * @param Key - The localisation key to search
	 * @return True if found
	 */
	bool FindText(FText& OutTarget, FTextKey Key);
};
