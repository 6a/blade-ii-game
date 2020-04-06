#include "B2UI/EffectWidgetBlast.h"

void UEffectWidgetBlast::Play(float StartDelay, float InPostDelay)
{
	Super::Play(StartDelay, InPostDelay);


}


void UEffectWidgetBlast::Play(const FVector& TargetWorldPosition, float StartDelay, float InPostDelay)
{
	Super::Play(TargetWorldPosition, StartDelay, InPostDelay);


}