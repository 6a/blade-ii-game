#include "B2UI/ComboBoxStringWithOverride.h"

void UComboBoxStringWithOverride::Clear()
{
	if (ComboBoxContent.IsValid())
	{
		MyComboBox->SetSelectedItem(CurrentOptionPtr);
		UpdateOrGenerateWidget(CurrentOptionPtr);
	}
	else
	{
		HandleSelectionChanged(CurrentOptionPtr, ESelectInfo::Direct);
	}
}