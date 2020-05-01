

#pragma once

#include "CoreMinimal.h"
#include "Components/ComboBoxString.h"
#include "ComboBoxStringWithOverride.generated.h"

/**
 * 
 */
UCLASS()
class BLADEIIGAME_API UComboBoxStringWithOverride : public UComboBoxString
{
	GENERATED_BODY()
	
public:

	/* Force reset this combobox (remove any selections and focus etc) */
	void Clear();
};
