#pragma once

#include "CoreMinimal.h"

#include "Containers/Queue.h"

#include "B2Game/Card.h"
#include "B2Engine/Transition.h"

struct B2CardAnimation
{
	UPROPERTY()
	ACard* Card;

	B2Transition Transition;
};

struct B2CardAnimationGroup
{
	TArray<B2CardAnimation> Group;
};

class B2CardAnimator
{

public:
	B2CardAnimator();

	static void AddGroup(const B2CardAnimationGroup& Group);

	static void Tick(float DeltaTime);

private:

	static B2CardAnimator* Instance;

	TQueue<B2CardAnimationGroup> AnimationQueue;
};

