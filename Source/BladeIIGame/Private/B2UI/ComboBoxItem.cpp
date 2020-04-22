#include "B2UI/ComboBoxItem.h"

void UComboBoxItem::SetText(FText Value)
{
	if (Text)
	{
		Text->SetText(Value);
	}
}