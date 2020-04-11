#pragma once

#include "CoreMinimal.h"

#include "B2UI/EffectWidget.h"

#include "EffectWidgetForce.generated.h"

UCLASS()
class BLADEIIGAME_API UEffectWidgetForce : public UEffectWidget
{
	GENERATED_BODY()

private:

	/* Run the animation for this widget */
	virtual void RunAnimation() override;
};