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

void UToolTip::SetVisible(bool bNewVisible)
{
	SetRenderOpacity(bNewVisible ? 1 : 0);
}
