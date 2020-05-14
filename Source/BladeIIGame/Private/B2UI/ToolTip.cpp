#include "B2UI/ToolTip.h"

void UToolTip::SetText(const FText& Header, const FText& Information)
{
	if (HeaderText)
	{
		HeaderText->SetText(Header);
	}

	if (InformationText)
	{
		InformationText->SetText(Information);
	}
}

void UToolTip::SetActive(bool bNewVisible)
{
	SetVisibility(bNewVisible ? ESlateVisibility::HitTestInvisible: ESlateVisibility::Hidden);
}
