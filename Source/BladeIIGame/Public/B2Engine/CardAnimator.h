#pragma once

#include "CoreMinimal.h"

#include "Containers/Queue.h"

#include "B2Game/Card.h"
#include "B2Engine/Transition.h"

#include "CardAnimator.generated.h"


USTRUCT()
struct FB2CardAnimation
{
	GENERATED_BODY()

	UPROPERTY()
	ACard* Card;

	B2Transition Transition;
};

USTRUCT()
struct FB2CardAnimationGroup
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FB2CardAnimation> Group;
};

UCLASS()
class UB2CardAnimator : public UObject
{
	GENERATED_BODY()

public:
	void AddGroup(FB2CardAnimationGroup& Group);

	void InsertIntoLatestGroup(FB2CardAnimationGroup& Group);

	void Tick(float DeltaTime);

private:

	TQueue<FB2CardAnimationGroup> AnimationQueue;
};

