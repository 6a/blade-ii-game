#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

#include "StatusIndicator.generated.h"

UCLASS()
class BLADEIIGAME_API UStatusIndicator : public UUserWidget
{
	GENERATED_BODY()

public:

	// State is an enum that represents the different states that the status indicator can be in
	enum class State : uint8
	{
		Waiting,
		PlayerTurn,
		OpponentTurn,
		GameOver,
		DrawACard,
	};

	/**
	 * Set the state for the status indicator
	 * @param NewState - The state to set
	 */
	void SetState(State NewState);

protected:

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:

	/* Class consts */
	const TCHAR Waiting_PLACEHOLDER_CHAR = TCHAR('.');
	const uint32 WAITING_PLACEHOLDER_MAX_CHARS = 3;
	const float WAITING_PLACEHOLDER_ANIMATION_RATE = 0.5f;
	const float TIMER_MAX = 20.999f;
	const float WARNING_ANIM_START_TIME = 5.999f;

	/* Waiting animation */
	float WaitingAnimationTimer;
	uint32 WaitingAnimationPhase;

	/* The current state of the status indicator */
	State CurrentState;

	/* Current turn time remaining for the current turn */
	float TurnTimeRemaining;

	/* Timer text widget */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimerText;

	/* Switching animation --> To Waiting */
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* SwitchToWaitingAnim;

	/* Switching animation --> To player turn */
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* SwitchToPlayerTurnAnim;

	/* Switching animation --> To opponent turn */
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* SwitchToOpponentTurnAnim;

	/* Switching animation --> To game over state */
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* SwitchToGameOverAnim;

	/* Switching animation --> To draw a card state */
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* SwitchToDrawACardAnim;

	/* Timer warning animation */
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* WarningAnim;
};
