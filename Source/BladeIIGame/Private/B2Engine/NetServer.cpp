#include "B2Engine/NetServer.h"

#include "B2Enum/WSPacketTypeEnum.h"
#include "Engine/World.h"

#include "B2Utility/Log.h"

const uint32 MAX_CONNECTION_ATTEMPTS = 3;
const FString WEBSOCKET_URL = TEXT("ws://localhost:80/game");
//const FString WEBSOCKET_URL = TEXT("wss://echo.websocket.org");
const FString AUTH_DELIMITER = TEXT(":");

const float MAX_CONNECT_ATTEMPT_TIME = 4.f;

/* Codes for categorising websocket messages */
const uint16 WSC_AUTH = 200;
const uint16 WSC_AUTH_BAD_FORMAT = 201;
const uint16 WSC_AUTH_BAD_CREDENTIALS = 202;
const uint16 WSC_AUTH_RECEIVED = 205;
const uint16 WSC_AUTH_EXPECTED = 206;
const uint16 WSC_AUTH_SUCCESS = 208;

const uint16 WSC_MATCH_ID = 400;
const uint16 WSC_MATCH_ID_EXPECTED = 401;
const uint16 WSC_MATCH_ID_BAD_FORMAT = 402;
const uint16 WSC_MATCH_INVALID = 403;
const uint16 WSC_MATCH_EXPIRED = 404;
const uint16 WSC_MATCH_ID_RECEIVED = 405;
const uint16 WSC_MATCH_ID_NOT_RECEIVED = 406;
const uint16 WSC_MATCH_ID_CONFIRMED = 407;
const uint16 WSC_MATCH_MULTIPLE_CONNECTIONS = 408;
const uint16 WSC_MATCH_FULL = 409;
const uint16 WSC_MATCH_JOINED = 410;
const uint16 WSC_MATCH_ILLEGAL_MOVE = 411;
const uint16 WSC_MATCH_RELAY_MESSAGE = 412;
const uint16 WSC_MATCH_MOVE = 413;
const uint16 WSC_MATCH_DATA = 414;
const uint16 WSC_MATCH_FORFEIT = 415;
const uint16 WSC_MATCH_MUTUAL_TIMEOUT = 416;
const uint16 WSC_MATCH_TIME_OUT = 417;
const uint16 WSC_MATCH_WIN = 418;
const uint16 WSC_MATCH_DRAW = 419;
const uint16 WSC_MATCH_LOSS = 420;

/* The server events to expect when a connection is successfully made */
const TArray<uint16> SEQ_EVT_CONNECTION_SUCCESS
{
	WSC_AUTH_RECEIVED,
	WSC_AUTH_SUCCESS,
	WSC_MATCH_ID_RECEIVED,
	WSC_MATCH_ID_CONFIRMED,
	WSC_MATCH_JOINED,
};

/* Server events that indicate that an auth error occurred */
const TArray<uint16> SEQ_EVT_AUTH_ERROR
{
	WSC_AUTH_BAD_FORMAT,
	WSC_AUTH_BAD_CREDENTIALS,
	WSC_AUTH_EXPECTED,
};

/* Server events that indicate that an auth error occurred */
const TArray<uint16> SEQ_EVT_MATCH_CHECK_ERROR
{
	WSC_MATCH_ID_EXPECTED,
	WSC_MATCH_ID_BAD_FORMAT,
	WSC_MATCH_INVALID,
	WSC_MATCH_EXPIRED,
};

/* Server events that indicate that an match setup error occurred */
const TArray<uint16> SEQ_EVT_MATCH_SETUP_ERROR
{
	WSC_MATCH_FULL,
	WSC_MATCH_MULTIPLE_CONNECTIONS,
};

/* Server events that indicate that an match has ended */
const TArray<uint16> SEQ_EVT_MATCH_END
{
	WSC_MATCH_WIN,
	WSC_MATCH_DRAW,
	WSC_MATCH_LOSS,
};

UB2NetServer::UB2NetServer()
{
	bConnected = false;
	bConnectionMade = false;
	bEnforceConnectionTimeout = false;
	ConnectionStepsProcessed = 0;
	bIgnoreAllEvents = false;
}

void UB2NetServer::Initialise(const FString& InPublicID, const FString& InAuthToken, uint64 InMatchID)
{
	PublicID = InPublicID;
	AuthToken = InAuthToken;
	MatchID = InMatchID;

	bConnectionMade = false;

	Connect();
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

			if (WebSocket)
			{
				WebSocket->SendText(WSPacket.GetSerialised());
			}
		}
	} 
	else if (bEnforceConnectionTimeout)
	{
		TimeSinceConnectionStart += DeltaSeconds;
		if (TimeSinceConnectionStart >= MAX_CONNECT_ATTEMPT_TIME)
		{
			ConnectionAttempts = MAX_CONNECTION_ATTEMPTS;
			HandleConnectionErrorEvent("Connection attempt timed out");
		}
	}
}

const FB2ServerUpdate UB2NetServer::GetNextUpdate()
{
	return Super::GetNextUpdate();
}

bool UB2NetServer::Connect()
{
	B2Utility::LogInfo("Attempting to initialise connection to game server");

	bConnected = false;
	ConnectionAttempts = 0;
	TimeSinceConnectionStart = 0;
	bEnforceConnectionTimeout = true;
	ConnectionStepsProcessed = 0;
	bIgnoreAllEvents = false;

	if (!bConnectionMade)
	{
		bool bInitialised = SetupWSConnection();

		if (!bInitialised)
		{
			B2Utility::LogWarning("Unable to initialise websocket");
			InBoundQueue.Enqueue(FB2ServerUpdate{ EServerUpdate::InstructionConnectionError });
		}
		else
		{
			B2Utility::LogWarning("Initialised websocket");
		}

		return bInitialised;
	}

	return false;
}

void UB2NetServer::Kill()
{
	UB2Server::Kill();

	if (WebSocket)
	{
		WebSocket->Close();
	}

	B2Utility::LogWarning(TEXT("NetServer was killed (永遠の安らぎがあらんことを)"));
}

bool UB2NetServer::SetupWSConnection()
{
	// Null out the websocket to ensure that it can receive no more events
	WebSocket = nullptr;

	// Add any other headers, auth or whatever to the default headers
	TArray<FWebSocketHeaderPair> Headers(DefaultHeaders);
	//Headers.Add(FWebSocketHeaderPair());

	// Attempt to connect
	bool bConnectionFailed = true;

	WebSocket = UWebSocketBlueprintLibrary::ConnectWithHeader(WEBSOCKET_URL, Headers, bConnectionFailed);

	SetupEventListeners();

	return !bConnectionFailed && WebSocket;
}

void UB2NetServer::SetupEventListeners()
{
	if (WebSocket)
	{
		WebSocket->OnConnectComplete.AddDynamic(this, &UB2NetServer::HandleConnectionEvent);
		WebSocket->OnConnectError.AddDynamic(this, &UB2NetServer::HandleConnectionErrorEvent);
		WebSocket->OnClosed.AddDynamic(this, &UB2NetServer::HandleConnectionClosedEvent);
		WebSocket->OnReceiveData.AddDynamic(this, &UB2NetServer::HandleMessageReceivedEvent);
	}
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
	// Early exit if we are ignoring events
	if (bIgnoreAllEvents) return;

	bConnected = true;

	// Auth
	FB2WebSocketPacket AuthPacket = MakeAuthPacket();
	FString SerialisedAuthString = AuthPacket.GetSerialised();
	WebSocket->SendText(SerialisedAuthString);

	// Match ID
	FB2WebSocketPacket MatchIDPacket = MakeMatchIDPacket();
	WebSocket->SendText(MatchIDPacket.GetSerialised());

	InBoundQueue.Enqueue(FB2ServerUpdate{ EServerUpdate::InstructionConnectionProgress });
}

void UB2NetServer::HandleConnectionClosedEvent()
{
	bConnected = false;

	// Early exit if we are ignoring events
	if (bIgnoreAllEvents) return;

	InBoundQueue.Enqueue(FB2ServerUpdate{ EServerUpdate::InstructionConnectionClosed });
	B2Utility::LogInfo("Connection closed");
}

void UB2NetServer::HandleConnectionErrorEvent(const FString& Error)
{
	// Early exit if we are ignoring events
	if (bIgnoreAllEvents) return;

	// If we arent connected, try to connect
	if (!bConnected)
	{
		if (++ConnectionAttempts <= MAX_CONNECTION_ATTEMPTS)
		{
			TimeSinceConnectionStart = 0;

			B2Utility::LogWarning(FString::Printf(TEXT("Could not connect to websocket [ %s ] - Retrying (retry #%d)"), *WEBSOCKET_URL, ConnectionAttempts));
			if (SetupWSConnection())
			{
				return;
			}

		}
	}

	bConnected = false;
	bEnforceConnectionTimeout = false;

	InBoundQueue.Enqueue(FB2ServerUpdate{ EServerUpdate::InstructionConnectionError });
	B2Utility::LogInfo(FString("Connection errored: ").Append(Error));
}

void UB2NetServer::HandleMessageReceivedEvent(const FString& Data)
{
	// Early exit if we are ignoring events
	if (bIgnoreAllEvents) return;

	FB2WebSocketPacket WebSocketPacket = FB2WebSocketPacket::FromJSONString(Data);

	FB2ServerUpdate OutUpdate{ EServerUpdate::None };

	// If the websocket packet code was parsed as 0, this was either a noop or an eror. 
	// Exit early if this is the case
	if (WebSocketPacket.Code != 0) 
	{		
		// Connecting to server
		if (SEQ_EVT_CONNECTION_SUCCESS.Contains(WebSocketPacket.Code)) {
			OutUpdate.Code = EServerUpdate::InstructionConnectionProgress;

			if (int32(++ConnectionStepsProcessed) >= SEQ_EVT_CONNECTION_SUCCESS.Num())
			{
				bEnforceConnectionTimeout = false;
			}
		}
		// Receiving match data / moves
		else if (WebSocketPacket.Code == WSC_MATCH_DATA || WebSocketPacket.Code == WSC_MATCH_MOVE)
		{
			bConnectionMade = true;

			// Unpack the server update in the payload - exit silently if it was parsed as none
			OutUpdate = FB2ServerUpdate::UnSerialise(WebSocketPacket.Message);
		}
		// Opponent forfeit (or disconnect - though this is also handled as a forfeit server side)
		else if (WebSocketPacket.Code == WSC_MATCH_FORFEIT)
		{
			OutUpdate.Code = EServerUpdate::InstructionForfeit;
			B2Utility::LogWarning(TEXT("Opponent forfeited"));

			bEnforceConnectionTimeout = false;
			bIgnoreAllEvents = true;
		}
		// Booted out due to illegal move
		else if (WebSocketPacket.Code == WSC_MATCH_ILLEGAL_MOVE)
		{
			OutUpdate.Code = EServerUpdate::InstructionMatchIllegalMove;
			B2Utility::LogWarning(TEXT("Illegal Move played by the local player"));
			bIgnoreAllEvents = true;
		}
		// Mutual timeout - draw
		else if (WebSocketPacket.Code == WSC_MATCH_MUTUAL_TIMEOUT)
		{
			OutUpdate.Code = EServerUpdate::InstructionMatchMutualTimeOut;
			B2Utility::LogWarning(TEXT("Match ended in a draw due to a mutual timeout"));
			bIgnoreAllEvents = true;
		}
		// Booted out due to being timed out for a move
		else if (WebSocketPacket.Code == WSC_MATCH_TIME_OUT)
		{
			OutUpdate.Code = EServerUpdate::InstructionMatchTimeOut;
			B2Utility::LogWarning(TEXT("Match ended in a loss due to the local player being timed out"));
			bIgnoreAllEvents = true;
		}
		// Match end
		else if (SEQ_EVT_MATCH_END.Contains(WebSocketPacket.Code))
		{
			// No further processing - these are handled by the game state anyway, so no need to pass them along

			bEnforceConnectionTimeout = false;
			bIgnoreAllEvents = true;
		}
		// Auth errors
		else if (SEQ_EVT_AUTH_ERROR.Contains(WebSocketPacket.Code))
		{
			OutUpdate.Code = EServerUpdate::InstructionAuthError;
			B2Utility::LogWarning(FString::Printf(TEXT("Authentication error: %d"), WebSocketPacket.Code));

			bEnforceConnectionTimeout = false;
			bIgnoreAllEvents = true;
		}
		// Match Check errors
		else if (SEQ_EVT_MATCH_CHECK_ERROR.Contains(WebSocketPacket.Code))
		{
			OutUpdate.Code = EServerUpdate::InstructionMatchCheckError;
			B2Utility::LogWarning(FString::Printf(TEXT("Match check error: %d"), WebSocketPacket.Code));

			bEnforceConnectionTimeout = false;
			bIgnoreAllEvents = true;
		}
		// Match Setup errors
		else if (SEQ_EVT_MATCH_SETUP_ERROR.Contains(WebSocketPacket.Code))
		{
			OutUpdate.Code = EServerUpdate::InstructionMatchSetupError;
			B2Utility::LogWarning(FString::Printf(TEXT("Match setup error: %d"), WebSocketPacket.Code));

			bEnforceConnectionTimeout = false;
			bIgnoreAllEvents = true;
		}

		if (OutUpdate.Code != EServerUpdate::None)
		{
			InBoundQueue.Enqueue(OutUpdate);
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
