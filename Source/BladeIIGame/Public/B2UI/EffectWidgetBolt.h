#pragma once

#include "CoreMinimal.h"

#include "B2UI/EffectWidget.h"

#include "EffectWidgetBolt.generated.h"

UCLASS()
class BLADEIIGAME_API UEffectWidgetBolt : public UEffectWidget
{
	GENERATED_BODY()

private:

	/* Run the animation for this widget */
	virtual void RunAnimation() override;
};
