#include "B2UI/ToolTipPanel.h"


#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"
#include "TimerManager.h"

#include "B2Utility/Log.h"

const FVector2D ALIGNMENT_TOP_LEFT = FVector2D(0, 0);
const FVector2D ALIGNMENT_BOTTOM_LEFT = FVector2D(0, 1);

const float TOOL_TIP_DELAY = 0.5f;
const float CURSOR_HEIGHT = 52.f;

const FString LOC_KEY_HEADER_PREFIX = "UI_LABEL_";
const FString LOC_KEY_INFO_PREFIX = "UI_INFORMATION_";
const FString LOC_KEY_HEADER_FACEDOWN = "UI_LABEL_FACEDOWN_001";
const FString LOC_KEY_INFO_FACEDOWN = "UI_INFORMATION_FACEDOWN_001";

const FTextKey LOC_KEY_UI_NAMESPACE = "LOC_UI";

const uint8 BOLTED_CARD_OFFSET = 11;

UToolTipPanel::UToolTipPanel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	LastMouseMoveTime = 1000000;
	bToolTipActivationPending = false;
	bToolTipTextRequiresUpdate = true;
	bActive = true;
}

void UToolTipPanel::SetLocalPlayerInput(ALocalPlayerInput* InLocalPlayerInput)
{
	LocalPlayerInput = InLocalPlayerInput;
}

void UToolTipPanel::SetActive(bool bNewActive)
{
	bActive = bNewActive;
	SetVisibility(bNewActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UToolTipPanel::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UWorld* World = GetWorld();

	if (bActive && LocalPlayerInput && ToolTip)
	{
		if (ShouldShowToolTip())
		{
			if (!bToolTipActivationPending && World)
			{
				bToolTipActivationPending = true;
				World->GetTimerManager().SetTimerForNextTick(this, &UToolTipPanel::SetToolTipVisibleDelayed);
			}

			APlayerController* LocalPlayerController = static_cast<APlayerController*>(LocalPlayerInput->GetController());
			if (LocalPlayerController)
			{
				FVector2D MousePosition = LocalPlayerInput->GetCurrentMousePosition();

				float DPIScale = GetUIScale(LocalPlayerController);
				UCanvasPanelSlot* ToolTipSlot = static_cast<UCanvasPanelSlot*>(ToolTip->Slot);

				ToolTipSlot->SetAlignment(GetAlignment(LocalPlayerController, MousePosition, ToolTipSlot));
				ToolTipSlot->SetPosition(MousePosition / DPIScale);

				if (bToolTipTextRequiresUpdate)
				{
					bToolTipTextRequiresUpdate = false;

					ECard TypeToCheck = CurrentHoveredCard->Type;
					ToolTipTextPair TextPair;

					if (!CurrentHoveredCard->IsActive())
					{
						TypeToCheck = static_cast<ECard>(static_cast<uint8>(TypeToCheck) + BOLTED_CARD_OFFSET);
						TextPair = GetTextPair(TypeToCheck);
					}
					else if (CurrentHoveredCard->IsActive() && CurrentHoveredCard->IsFaceDown())
					{
						TextPair = GetFaceDownCardTextPair();
					}
					else
					{
						TextPair = GetTextPair(TypeToCheck);
					}

					ToolTip->SetText(TextPair.Header, TextPair.Information);
				}
			}
		}
		else
		{
			ToolTip->SetActive(false);
		}

		if (!LocalPlayerInput->MouseIsStationary())
		{
			if (World)
			{
				LastMouseMoveTime = World->GetRealTimeSeconds();
				
				ACard* HoveredCard = LocalPlayerInput->GetHoveredCard();
				if (HoveredCard)
				{
					if (!CurrentHoveredCard || HoveredCard->Type != CurrentHoveredCard->Type)
					{
						CurrentHoveredCard = HoveredCard;
						bToolTipTextRequiresUpdate = true;
					}
				}
			}
		}
	}
}

float UToolTipPanel::GetUIScale(const APlayerController* LocalPlayerController) const
{
	// Ref: https://gist.github.com/JonathanADaley/151f26b145981336371b73def45209e7

	FIntVector ViewportSize;

	LocalPlayerController->GetViewportSize(ViewportSize.X, ViewportSize.Y);

	return GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));
}

FVector2D UToolTipPanel::GetAlignment(const APlayerController* LocalPlayerController, const FVector2D& MousePosition, const UCanvasPanelSlot* CanvasPanelSlot) const
{
	FIntVector ViewportSize;

	LocalPlayerController->GetViewportSize(ViewportSize.X, ViewportSize.Y);

	FVector2D OutVector;

	// If Y is greater, then its on the bottom half of the screen.
	// If X is greater, then its on the right side of the screen.
	if (MousePosition.Y > ViewportSize.Y / 2)
	{
		OutVector = ALIGNMENT_BOTTOM_LEFT;
	}
	else
	{
		float CurrentToolTipHeight = ToolTip->GetDesiredSize().Y;
		float UIScale = GetUIScale(LocalPlayerController);

		float VerticalOffsetAdjustment = CURSOR_HEIGHT / (CurrentToolTipHeight * UIScale);

		B2Utility::LogInfo(FString::Printf(TEXT("CurrentToolTipHeight: %f | CURSOR_HEIGHT: %f => %f"), CurrentToolTipHeight * UIScale, CURSOR_HEIGHT, VerticalOffsetAdjustment));

		OutVector = ALIGNMENT_TOP_LEFT;

		OutVector.Y -= VerticalOffsetAdjustment;
	}

	return OutVector;
}

bool UToolTipPanel::ShouldShowToolTip() const
{
	UWorld* World = GetWorld();
	if (LocalPlayerInput->GetHoveredCard() && World)
	{
		return World->GetRealTimeSeconds() - LastMouseMoveTime > TOOL_TIP_DELAY;
	}

	return false;
}

ToolTipTextPair UToolTipPanel::GetTextPair(ECard Card) const
{
	ToolTipTextPair OutPair;
	FString LocKeyHeaderIdentifierString;
	FString LocKeyInfoIdentifierString;
	FTextKey LocTextHeaderKey;
	FTextKey LocTextInfoKey;

	switch (Card)
	{
	case ECard::ElliotsOrbalStaff:
		LocKeyHeaderIdentifierString = TEXT("ELLIOTSORBALSTAFF_001");
		break;
	case ECard::FiesTwinGunswords:
		LocKeyHeaderIdentifierString = TEXT("FIESTWINGUNSWORDS_001");
		break;
	case ECard::AlisasOrbalBow:
		LocKeyHeaderIdentifierString = TEXT("ALISASORBALBOW_001");
		break;
	case ECard::JusisSword:
		LocKeyHeaderIdentifierString = TEXT("JUSISSWORD_001");
		break;
	case ECard::MachiasOrbalShotgun:
		LocKeyHeaderIdentifierString = TEXT("MACHIASORBALSHOTGUN_001");
		break;
	case ECard::GaiusSpear:
		LocKeyHeaderIdentifierString = TEXT("GAIUSSPEAR_001");
		break;
	case ECard::LaurasGreatsword:
		LocKeyHeaderIdentifierString = TEXT("LAURASGREATSWORD_001");
		break;
	case ECard::Bolt:
		LocKeyHeaderIdentifierString = TEXT("BOLT_001");
		break;
	case ECard::Mirror:
		LocKeyHeaderIdentifierString = TEXT("MIRROR_001");
		break;
	case ECard::Blast:
		LocKeyHeaderIdentifierString = TEXT("BLAST_001");
		break;
	case ECard::Force:
		LocKeyHeaderIdentifierString = TEXT("FORCE_001");
		break;
	case ECard::InactiveElliotsOrbalStaff:
		LocKeyHeaderIdentifierString = TEXT("ELLIOTSORBALSTAFF_BOLTED_001");
		break;
	case ECard::InactiveFiesTwinGunswords:
		LocKeyHeaderIdentifierString = TEXT("FIESTWINGUNSWORDS_BOLTED_001");
		break;
	case ECard::InactiveAlisasOrbalBow:
		LocKeyHeaderIdentifierString = TEXT("ALISASORBALBOW_BOLTED_001");
		break;
	case ECard::InactiveJusisSword:
		LocKeyHeaderIdentifierString = TEXT("JUSISSWORD_BOLTED_001");
		break;
	case ECard::InactiveMachiasOrbalShotgun:
		LocKeyHeaderIdentifierString = TEXT("MACHIASORBALSHOTGUN_BOLTED_001");
		break;
	case ECard::InactiveGaiusSpear:
		LocKeyHeaderIdentifierString = TEXT("GAIUSSPEAR_BOLTED_001");
		break;
	case ECard::InactiveLaurasGreatsword:
		LocKeyHeaderIdentifierString = TEXT("LAURASGREATSWORD_BOLTED_001");
		break;
	case ECard::InactiveBolt:
		LocKeyHeaderIdentifierString = TEXT("BOLT_BOLTED_001");
		break;
	case ECard::InactiveMirror:
		LocKeyHeaderIdentifierString = TEXT("MIRROR_BOLTED_001");
		break;
	case ECard::InactiveBlast:
		LocKeyHeaderIdentifierString = TEXT("BLAST_BOLTED_001");
		break;
	case ECard::InactiveForce:
		LocKeyHeaderIdentifierString = TEXT("FORCE_BOLTED_001");
		break;
	}

	if (Card >= ECard::InactiveElliotsOrbalStaff)
	{
		LocKeyInfoIdentifierString = TEXT("BOLTED_CARD_001");
	}
	else
	{
		LocKeyInfoIdentifierString = LocKeyHeaderIdentifierString;
	}

	LocTextHeaderKey = FTextKey(FString::Printf(TEXT("%s%s"), *LOC_KEY_HEADER_PREFIX, *LocKeyHeaderIdentifierString));
	LocTextInfoKey = FTextKey(FString::Printf(TEXT("%s%s"), *LOC_KEY_INFO_PREFIX, *LocKeyInfoIdentifierString));

	FText OutHeader;
	bool bFoundHeader = FText::FindText(LOC_KEY_UI_NAMESPACE, LocTextHeaderKey, OutHeader);
	if (bFoundHeader)
	{
		OutPair.Header = OutHeader;
	}
	else
	{
		OutPair.Header = FText::FromString(TEXT("<unknown>"));
	}

	FText OutInfo;
	bool bFoundInfo = FText::FindText(LOC_KEY_UI_NAMESPACE, LocTextInfoKey, OutInfo);
	if (bFoundInfo)
	{
		OutPair.Information = OutInfo;
	}
	else
	{
		OutPair.Information = FText::FromString(TEXT("<unknown>"));
	}

	return OutPair;
}

ToolTipTextPair UToolTipPanel::GetFaceDownCardTextPair() const
{
	ToolTipTextPair OutPair;

	FText OutHeader;
	bool bFoundHeader = FText::FindText(LOC_KEY_UI_NAMESPACE, LOC_KEY_HEADER_FACEDOWN, OutHeader);
	if (bFoundHeader)
	{
		OutPair.Header = OutHeader;
	}
	else
	{
		OutPair.Header = FText::FromString(TEXT("<unknown>"));
	}

	FText OutInfo;
	bool bFoundInfo = FText::FindText(LOC_KEY_UI_NAMESPACE, LOC_KEY_INFO_FACEDOWN, OutInfo);
	if (bFoundInfo)
	{
		OutPair.Information = OutInfo;
	}
	else
	{
		OutPair.Information = FText::FromString(TEXT("<unknown>"));
	}

	return OutPair;
}

void UToolTipPanel::SetToolTipVisibleDelayed()
{
	bToolTipActivationPending = false;
	ToolTip->SetActive(true);
}
