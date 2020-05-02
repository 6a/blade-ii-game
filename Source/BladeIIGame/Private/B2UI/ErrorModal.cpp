#include "B2UI/ErrorModal.h"

#include "B2GameMode/BladeIIGameMode.h"

#include "B2Utility/Log.h"

const FTextKey LOC_KEY_UI_NAMESPACE(TEXT("LOC_UI"));

const FTextKey LOC_KEY_UNABLE_TO_CONNECT_MESSAGE(TEXT("UI_LABEL_UNABLETOCONNECT_000"));
const FTextKey LOC_KEY_DISCONNECTED_MESSAGE(TEXT("UI_LABEL_DISCONNECTED_000"));
const FTextKey LOC_KEY_OPPONENTFORFEIT_MESSAGE(TEXT("UI_LABEL_OPPONENTFORFEIT_000"));
const FTextKey LOC_KEY_MUTUAL_TIME_OUT_MESSAGE(TEXT("UI_LABEL_MUTUALTIMEOUT_000"));
const FTextKey LOC_KEY_TIME_OUT_MESSAGE(TEXT("UI_LABEL_TIMEOUT_000"));

const FTextKey LOC_KEY_CHECKNET_INFO(TEXT("UI_LABEL_CHECKNET_000"));
const FTextKey LOC_KEY_VICTORIOUS_INFO(TEXT("UI_LABEL_VICTORIOUS_000"));
const FTextKey LOC_KEY_MUTUAL_TIME_OUT_INFO(TEXT("UI_LABEL_MUTUALTIMEOUTINFO_000"));
const FTextKey LOC_KEY_TIME_OUT_INFO(TEXT("UI_LABEL_TIMEOUTINFO_000"));

UErrorModal::UErrorModal(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
	SetupLocalization();
}

void UErrorModal::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterEventListeners();

	GameModeInstance = Cast<ABladeIIGameMode>(GetWorld()->GetAuthGameMode());
}

void UErrorModal::SetErrorType(ErrorType Type)
{
	switch (Type)
	{
	case UErrorModal::ErrorType::UnableToConnect:
		PositiveButton->SetVisibility(ESlateVisibility::Visible);
		MessageText->SetText(LocalisedTextUnableToConnect);
		InfoText->SetText(LocalisedTextCheckNet);
		break;
	case UErrorModal::ErrorType::Disconnected:
		NegativeButton->SetVisibility(ESlateVisibility::Collapsed);
		MessageText->SetText(LocalisedTextDisconnected);
		InfoText->SetText(LocalisedTextCheckNet);
		break;
	case UErrorModal::ErrorType::OpponentForfeit:
		NegativeButton->SetVisibility(ESlateVisibility::Collapsed);
		MessageText->SetText(LocalisedTextOpponentForfeit);
		InfoText->SetText(LocalisedTextVictorious);
	case UErrorModal::ErrorType::MutualTimeOut:
		NegativeButton->SetVisibility(ESlateVisibility::Collapsed);
		MessageText->SetText(LocalisedTextMutualTimeOut);
		InfoText->SetText(LocalisedTextMutualTimeOutInfo);
		break;
	case UErrorModal::ErrorType::TimeOut:
		NegativeButton->SetVisibility(ESlateVisibility::Collapsed);
		MessageText->SetText(LocalisedTextTimeOut);
		InfoText->SetText(LocalisedTextTimeOutInfo);
		break;
	}
}

void UErrorModal::SetActive(bool bNewVisible)
{
	if (bNewVisible)
	{
		PlayAnimationForward(ShowHideAnimation);
	} 
	else
	{
		PlayAnimationReverse(ShowHideAnimation);
	}
}

bool UErrorModal::IsActive() const
{
	return ModalWrapper->GetVisibility() != ESlateVisibility::Hidden;
}

void UErrorModal::HandlePositiveButtonClicked()
{
	GameModeInstance->LocalQuit();
}

void UErrorModal::HandleNegativeButtonClicked()
{
	if (OnButtonPressed.IsBound()) OnButtonPressed.Broadcast();
	SetActive(false);
}

void UErrorModal::RegisterEventListeners()
{
	if (PositiveButton)
	{
		PositiveButton->OnClicked.AddDynamic(this, &UErrorModal::HandlePositiveButtonClicked);
	}

	if (NegativeButton)
	{
		NegativeButton->OnClicked.AddDynamic(this, &UErrorModal::HandleNegativeButtonClicked);
	}
}

void UErrorModal::SetupLocalization()
{
	TArray<FTextKey> Keys
	{
		LOC_KEY_UNABLE_TO_CONNECT_MESSAGE, 
		LOC_KEY_DISCONNECTED_MESSAGE, 
		LOC_KEY_CHECKNET_INFO, 
		LOC_KEY_VICTORIOUS_INFO, 
		LOC_KEY_OPPONENTFORFEIT_MESSAGE,

		LOC_KEY_MUTUAL_TIME_OUT_MESSAGE,
		LOC_KEY_MUTUAL_TIME_OUT_INFO,
		LOC_KEY_TIME_OUT_MESSAGE,
		LOC_KEY_TIME_OUT_INFO,
	};

	TArray<FText*> Texts
	{
		&LocalisedTextUnableToConnect, 
		&LocalisedTextDisconnected, 
		&LocalisedTextCheckNet, 
		&LocalisedTextVictorious, 
		&LocalisedTextOpponentForfeit,

		&LocalisedTextMutualTimeOut,
		&LocalisedTextMutualTimeOutInfo,
		&LocalisedTextTimeOut,
		&LocalisedTextTimeOutInfo,
	};

	for (size_t i = 0; i < Keys.Num(); i++)
	{
		if (!FindText(*Texts[i], Keys[i]))
		{
			B2Utility::LogWarning(FString::Printf(TEXT("Unable to locate localisation object for key [%s]"), Keys[i].GetChars()));
		}
	}
}

bool UErrorModal::FindText(FText& OutTarget, FTextKey Key)
{
	return FText::FindText(LOC_KEY_UI_NAMESPACE, Key, OutTarget);
}
