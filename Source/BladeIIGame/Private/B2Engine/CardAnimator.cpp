#include "B2Engine/CardAnimator.h"

#include "B2Game/GameSound.h"

void UB2CardAnimator::AddGroup(FB2CardAnimationGroup& Group)
{
	for (size_t i = 0; i < Group.Group.Num(); i++)
	{
		Group.Group[i].Transition.SetGameSoundInstance(Group.Group[i].Card->GameSound);
	}

	AnimationQueue.Enqueue(Group);
}

void UB2CardAnimator::InsertIntoLatestGroup(FB2CardAnimationGroup& Group)
{
	for (size_t i = 0; i < Group.Group.Num(); i++)
	{
		Group.Group[i].Transition.SetGameSoundInstance(Group.Group[i].Card->GameSound);
	}

	if (!AnimationQueue.IsEmpty())
	{
		AnimationQueue.Peek()->Group.Append(Group.Group);
	}
	else
	{
		AnimationQueue.Enqueue(Group);
	}
}

void UB2CardAnimator::Tick(float DeltaTime)
{
	if (!AnimationQueue.IsEmpty())
	{
		if (AnimationQueue.Peek()->Group.Num() > 0)
		{
			TSet<FString> CardsHandled;
			TArray<int32> ToRemove;

			for (int32 i = 0; i < AnimationQueue.Peek()->Group.Num(); ++i)
			{
				if (AnimationQueue.Peek()->Group[i].Transition.Done())
				{
					ToRemove.Add(i);
				}
				else if (!CardsHandled.Contains(AnimationQueue.Peek()->Group[i].Card->GetID()))
				{
					CardsHandled.Add(AnimationQueue.Peek()->Group[i].Card->GetID());

					AnimationQueue.Peek()->Group[i].Transition.Tick(DeltaTime);
					FVector Location = AnimationQueue.Peek()->Group[i].Transition.CurrentPosition;
					FRotator Rotation = AnimationQueue.Peek()->Group[i].Transition.CurrentRotation;

					AnimationQueue.Peek()->Group[i].Card->SetActorLocationAndRotation(Location, Rotation);
				}
			}

			for (int32 i = ToRemove.Num() - 1; i >= 0; --i)
			{
				AnimationQueue.Peek()->Group.RemoveAt(ToRemove[i]);
			}
		}
		else
		{
			AnimationQueue.Pop();
		}
	}
}
