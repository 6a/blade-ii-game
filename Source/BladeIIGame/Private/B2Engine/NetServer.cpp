#include "B2Engine/NetServer.h"

#include "B2Utility/Log.h"

const uint32 MAX_CONNECTION_ATTEMPTS = 3;
const FString WEBSOCKET_URL = TEXT("wss://echo.websocket.org");

bool UB2NetServer::Initialise(const FString& PublicID, const FString& AuthToken, uint64 MatchID)
{
	ConnectionAttempts = 0;

	B2Utility::LogInfo("Attempting to initialise connection to game server");

	OutBoundQueue.Enqueue(FB2ServerUpdate{ EServerUpdate::InstructionMessage, TEXT("Hello, World!") });

	bool bInitialised = Connect(PublicID, AuthToken, MatchID);

	if (!bInitialised)
	{
		B2Utility::LogWarning("Unable to initialise websocket");
	}
	else
	{
		B2Utility::LogWarning("Initialised websocket");
		SetupEventListeners();
	}

	return bInitialised;
}

void UB2NetServer::Tick(float DeltaSeconds)
{
	if (bConnected)
	{
		// Handle messages to the server
		FB2ServerUpdate MessageToServer;
		while (OutBoundQueue.Dequeue(MessageToServer))
		{
			FString MessageToServerJSON = MessageToServer.GetSerialised();
			WebSocket->SendText(MessageToServerJSON);
		}
	}
}

const FB2ServerUpdate UB2NetServer::GetNextUpdate()
{
	return FB2ServerUpdate();
}

bool UB2NetServer::Connect(const FString& PublicID, const FString& AuthToken, uint64 MatchID)
{
	// Add any other headers, auth or whatever to the default headers
	TArray<FWebSocketHeaderPair> Headers(DefaultHeaders);
	//Headers.Add(FWebSocketHeaderPair());

	// Attempt to connect
	bool bConnectionFailed = true;
	WebSocket = UWebSocketBlueprintLibrary::ConnectWithHeader(WEBSOCKET_URL, Headers, bConnectionFailed);

	// Try again until the retry limit is reached, then exit and clean up + report back
	if (bConnectionFailed)
	{
		if (++ConnectionAttempts < MAX_CONNECTION_ATTEMPTS)
		{
			B2Utility::LogWarning(FString::Printf(TEXT("Could not connect to websocket [ %s ] - Retrying (retry #%d)"), *WEBSOCKET_URL, ConnectionAttempts));
			return Connect(PublicID, AuthToken, MatchID);
		}
		else
		{
			return false;
		}
	}

	return true;
}

void UB2NetServer::SetupEventListeners()
{
	WebSocket->OnConnectComplete.AddDynamic(this, &UB2NetServer::HandleConnectionEvent);
	WebSocket->OnConnectError.AddDynamic(this, &UB2NetServer::HandleConnectionErrorEvent);
	WebSocket->OnClosed.AddDynamic(this, &UB2NetServer::HandleConnectionClosedEvent);
	WebSocket->OnReceiveData.AddDynamic(this, &UB2NetServer::HandleMessageReceivedEvent);
}

void UB2NetServer::HandleConnectionEvent()
{
	bConnected = true;

	B2Utility::LogInfo("Connection opened");
}

void UB2NetServer::HandleConnectionClosedEvent()
{
	bConnected = false;

	B2Utility::LogInfo("Connection closed");
}

void UB2NetServer::HandleConnectionErrorEvent(const FString& Error)
{
	// TODO how to handle this?
	bConnected = false;

	B2Utility::LogInfo(FString("Connection errored: ").Append(Error));
}

void UB2NetServer::HandleMessageReceivedEvent(const FString& Data)
{
	FB2ServerUpdate Update = FB2ServerUpdate::FromJSONString(Data);

	B2Utility::LogInfo(FString::Printf(TEXT("Connection received data: Update: %d | Metadata: [ %s ]"), Update.Update, *Update.Metadata));
}
