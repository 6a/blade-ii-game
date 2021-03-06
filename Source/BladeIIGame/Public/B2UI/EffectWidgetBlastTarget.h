#pragma once

#include "CoreMinimal.h"

#include "B2UI/EffectWidget.h"

#include "EffectWidgetBlastTarget.generated.h"

UCLASS()
class BLADEIIGAME_API UEffectWidgetBlastTarget : public UEffectWidget
{
	GENERATED_BODY()

private:

	/* Run the animation for this widget */
	virtual void RunAnimation() override;
};
