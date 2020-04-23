#include "B2Engine/NetServer.h"

#include "B2Engine/WebSocketPacket.h"
#include "B2Enum/WSPacketTypeEnum.h"

#include "B2Utility/Log.h"

const uint32 MAX_CONNECTION_ATTEMPTS = 3;
const FString WEBSOCKET_URL = TEXT("ws://localhost:80/matchmaking");

bool UB2NetServer::Initialise(const FString& InPublicID, const FString& InAuthToken, uint64 InMatchID)
{
	PublicID = InPublicID;
	AuthToken = InAuthToken;
	MatchID = InMatchID;

	ConnectionAttempts = 0;
	bConnected = false;

	B2Utility::LogInfo("Attempting to initialise connection to game server");

	bool bInitialised = SetupWSConnection();

	if (!bInitialised)
	{
		B2Utility::LogWarning("Unable to initialise websocket");
	}
	else
	{
		B2Utility::LogWarning("Initialised websocket");
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
			FB2WebSocketPacket WSPacket{};

			if (static_cast<uint8>(MessageToServer.Update) >= 12)
			{
				// Translate the instruction to a packet type
				switch (MessageToServer.Update)
				{
				case EServerUpdate::InstructionForfeit:
					WSPacket.Code = static_cast<uint8>(EWSPacketType::Forfeit);
					break;
				case EServerUpdate::InstructionMessage:
					WSPacket.Code = static_cast<uint8>(EWSPacketType::Message);
					break;
				}
			}
			else
			{
				WSPacket.Code = static_cast<uint8>(EWSPacketType::Move);
				WSPacket.Message = MessageToServer.GetSerialised();
			}

			WebSocket->SendText(WSPacket.GetSerialised());
		}
	}
}

const FB2ServerUpdate UB2NetServer::GetNextUpdate()
{
	return FB2ServerUpdate();
}

bool UB2NetServer::SetupWSConnection()
{
	// Add any other headers, auth or whatever to the default headers
	TArray<FWebSocketHeaderPair> Headers(DefaultHeaders);
	//Headers.Add(FWebSocketHeaderPair());

	// Attempt to connect
	bool bConnectionFailed = true;

	WebSocket = UWebSocketBlueprintLibrary::ConnectWithHeader(WEBSOCKET_URL, Headers, bConnectionFailed);

	SetupEventListeners();

	return !bConnectionFailed;
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
	// If we arent connected, try to connect
	if (!bConnected)
	{
		if (++ConnectionAttempts <= MAX_CONNECTION_ATTEMPTS)
		{
			B2Utility::LogWarning(FString::Printf(TEXT("Could not connect to websocket [ %s ] - Retrying (retry #%d)"), *WEBSOCKET_URL, ConnectionAttempts));
			SetupWSConnection();
			return;
		}
		else
		{
			// Handle - couldnt connect after MAX_CONNECTION_ATTEMPTS tries
		}
	}

	// TODO how to handle this?
	bConnected = false;

	B2Utility::LogInfo(FString("Connection errored: ").Append(Error));
}

void UB2NetServer::HandleMessageReceivedEvent(const FString& Data)
{
	FB2WebSocketPacket WebSocketPacket = FB2WebSocketPacket::FromJSONString(Data);
	B2Utility::LogInfo(FString::Printf(TEXT("WS packet received: Code: %d, Message: [ %s ]"), WebSocketPacket.Code, *WebSocketPacket.Message));

	FB2ServerUpdate Update = FB2ServerUpdate::FromJSONString(WebSocketPacket.Message);
	B2Utility::LogInfo(FString::Printf(TEXT("WS update parsed: Update: %d | Metadata: [ %s ]"), Update.Update, *Update.Metadata));
}
