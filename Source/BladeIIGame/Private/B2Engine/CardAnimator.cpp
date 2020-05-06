#include "B2Engine/CardAnimator.h"

B2CardAnimator* B2CardAnimator::Instance;

B2CardAnimator::B2CardAnimator()
{
	if (!Instance)
	{
		Instance = this;
	}
}

void B2CardAnimator::AddGroup(const B2CardAnimationGroup& Group)
{
	Instance->AnimationQueue.Enqueue(Group);
}

void B2CardAnimator::Tick(float DeltaTime)
{
	if (!Instance->AnimationQueue.IsEmpty())
	{
		if (Instance->AnimationQueue.Peek()->Group.Num() > 0)
		{
			for (int i = Instance->AnimationQueue.Peek()->Group.Num() - 1; i >= 0 ; --i)
			{
				Instance->AnimationQueue.Peek()->Group[i].Transition.Tick(DeltaTime);
				FVector Location = Instance->AnimationQueue.Peek()->Group[i].Transition.CurrentPosition;
				FRotator Rotation = Instance->AnimationQueue.Peek()->Group[i].Transition.CurrentRotation;

				Instance->AnimationQueue.Peek()->Group[i].Card->SetActorLocationAndRotation(Location, Rotation);

				if (Instance->AnimationQueue.Peek()->Group[i].Transition.Done())
				{
					Instance->AnimationQueue.Peek()->Group.RemoveAt(i);
				}
			}
		}
		else
		{
			Instance->AnimationQueue.Pop();
		}
	}
}
