#include "B2Engine/NetServer.h"

#include "B2Enum/WSPacketTypeEnum.h"
#include "Engine/World.h"

#include "B2Utility/Log.h"

const uint32 MAX_CONNECTION_ATTEMPTS = 3;
const FString WEBSOCKET_URL = TEXT("ws://localhost:80/game");
//const FString WEBSOCKET_URL = TEXT("wss://echo.websocket.org");
const FString AUTH_DELIMITER = TEXT(":");

const float TIME_BETWEEN_PINGS = 4.f;

/* Codes for categorising websocket messages */
const uint16 WSC_AUTH = 200;
const uint16 WSC_AUTH_RECEIVED = 205;
const uint16 WSC_AUTH_SUCCESS = 208;

const uint16 WSC_MATCH_ID = 400;
const uint16 WSC_MATCH_ID_EXPECTED = 401;
const uint16 WSC_MATCH_ID_BAD_FORMAT = 402;
const uint16 WSC_MATCH_INVALID = 403;
const uint16 WSC_MATCH_ID_RECEIVED = 404;
const uint16 WSC_MATCH_ID_NOT_RECEIVED = 405;
const uint16 WSC_MATCH_ID_CONFIRMED = 406;
const uint16 WSC_MATCH_MULTIPLE_CONNECTIONS = 407;
const uint16 WSC_MATCH_FULL = 408;
const uint16 WSC_MATCH_JOINED = 409;
const uint16 WSC_MATCH_ILLEGAL_MOVE = 410;
const uint16 WSC_MATCH_RELAY_MESSAGE = 411;
const uint16 WSC_MATCH_MOVE = 412;
const uint16 WSC_MATCH_DATA = 413;
const uint16 WSC_MATCH_FORFEIT = 414;
const uint16 WSC_MATCH_TIMED_OUT = 415;

/* The server events to expect when a connection is successfully made */
const TArray<uint16> SEQ_EVT_CONNECTION_SUCCESS
{
	WSC_AUTH_RECEIVED,
	WSC_AUTH_SUCCESS,
	WSC_MATCH_ID_RECEIVED,
	WSC_MATCH_ID_CONFIRMED,
	WSC_MATCH_JOINED,
};

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

			if (static_cast<uint8>(MessageToServer.Code) >= 12)
			{
				// Translate the instruction to a packet type
				switch (MessageToServer.Code)
				{
				case EServerUpdate::InstructionForfeit:
					WSPacket.Code = WSC_MATCH_FORFEIT;
					break;
				case EServerUpdate::InstructionMessage:
					WSPacket.Code = WSC_MATCH_RELAY_MESSAGE;
					WSPacket.Message = MessageToServer.GetSerialised();
					break;
				}
			}
			else
			{
				WSPacket.Code = WSC_MATCH_MOVE;
				WSPacket.Message = MessageToServer.GetSerialised();
			}

			WebSocket->SendText(WSPacket.GetSerialised());
		}
	}
}

const FB2ServerUpdate UB2NetServer::GetNextUpdate()
{
	return Super::GetNextUpdate();
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

FB2WebSocketPacket UB2NetServer::MakeAuthPacket() const
{
	FB2WebSocketPacket Packet{
		WSC_AUTH,
		FString::Printf(TEXT("%s%s%s"), *PublicID, *AUTH_DELIMITER, *AuthToken),
	};

	return Packet;
}

FB2WebSocketPacket UB2NetServer::MakeMatchIDPacket() const
{
	FB2WebSocketPacket Packet{
		WSC_MATCH_ID,
		FString::Printf(TEXT("%d"), MatchID),
	};

	return Packet;
}

void UB2NetServer::HandleConnectionEvent()
{
	bConnected = true;

	// Auth
	FB2WebSocketPacket AuthPacket = MakeAuthPacket();
	FString SerialisedAuthString = AuthPacket.GetSerialised();
	WebSocket->SendText(SerialisedAuthString);

	// Match ID
	FB2WebSocketPacket MatchIDPacket = MakeMatchIDPacket();
	WebSocket->SendText(MatchIDPacket.GetSerialised());

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

	FB2ServerUpdate OutUpdate{ EServerUpdate::None };

	// If the websocket packet code was parsed as 0, this was either a noop or an eror. 
	// Exit early if this is the case
	if (WebSocketPacket.Code != 0) 
	{		
		// Connecting to server
		if (SEQ_EVT_CONNECTION_SUCCESS.Contains(WebSocketPacket.Code)) {
			OutUpdate.Code = EServerUpdate::InstructionConnectionProgress;
		}
		else if (WebSocketPacket.Code == WSC_MATCH_DATA || WebSocketPacket.Code == WSC_MATCH_MOVE)
		{
			// Unpack the server update in the payload - exit silently if it was parsed as none
			OutUpdate = FB2ServerUpdate::UnSerialise(WebSocketPacket.Message);
			if (OutUpdate.Code != EServerUpdate::None) 
			{
				InBoundQueue.Enqueue(OutUpdate);
			}
		}
	}

	if (OutUpdate.Code != EServerUpdate::None)
	{
		B2Utility::LogInfo(FString::Printf(TEXT("WS update parsed: Code: %d | Payload: [ %s ]"), OutUpdate.Code, *OutUpdate.Payload));
	}
	else
	{
		B2Utility::LogInfo(FString::Printf(TEXT("WS packet received but could not be parsed: Code: %d, Payload: [ %s ]"), WebSocketPacket.Code, *WebSocketPacket.Message));
	}
}
