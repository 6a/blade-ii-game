#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

#include "B2Enum/CardEnum.h"
#include "B2Engine/Transition.h"

#include "Card.generated.h"

UCLASS()
class ACard : public AActor
{
	GENERATED_BODY()
	
public:
	/* The mesh for this card actor, with read access for blueprints */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	/* The type of this card */
	ECard Type;

	const static uint32 MAX_CARD_SCORE = 7;

	ACard();

	/**
	 * Add a transition to the transition queue, which will play after any preceeding transitions have finished playing.
	 * @param Transition - A transition helper object
	 */
	void QueueTransition(B2Transition& Transition);

	/**
	 * Fade the card in over (Duration) seconds
	 * @param Duration - How long the fade will take
	 */
	void FadeIn(float Duration);

	/**
	 * Fade the card out over (Duration) seconds
	 * @param Duration - How long the fade will take
	 */
	void FadeOut(float Duration);

	/* Returns true if this card is currently transitioning */
	bool IsTransitioning() const;

	/* Returns the ID for this card */
	const FString GetID() const;

	/**
	 * Change the active state for this card.
	 * @param bNewActive - The new State
	 */
	void SetActive(bool bNewActive);

	/* Returns true if this card is currently active */
	bool IsActive() const;

	/* Returns true if the card is face-down (such as when its in the opponents hand) */
	bool IsFaceDown() const;

	static uint32 TypeToValue(ECard CardType);

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

private:
	// Used to describe the current fade state
	enum EFadeState
	{
		None,
		FadingIn,
		FadingOut
	};

	/* The queue of transitions for this card */
	TQueue<B2Transition> Transitions;

	/* The randomly generated ID for this card */
	FString ID;

	/* Whether this card is active (face up) or not - only really applies when on the field */
	bool bIsActive;

	/* The original scale of the card, so that we can revert back to it if required */
	FVector OriginalScale;

	/* For "opacity" fading (its actually dithered temporal AA) */
	float OpacityTransitionAlpha;
	float FadeDuration;
	EFadeState FadeState;

	// Class consts
	const float LERP_MIN = 0.f;
	const float LERP_MAX = 1.f;
	const float EASE_EXPONENT = 2.f;
	const FName OPACITY_PARAMETER = TEXT("Opacity");

	/* Reference to the game sound class */
	class AGameSound* GameSound;
};
