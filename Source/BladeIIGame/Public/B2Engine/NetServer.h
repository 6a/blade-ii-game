#pragma once

#include "B2Engine/Server.h"

#include "WebSocketBlueprintLibrary.h"

#include "B2Engine/WebSocketPacket.h"

#include "NetServer.generated.h"

UCLASS()
class UB2NetServer : public UB2Server
{
	GENERATED_BODY()

public:

	UB2NetServer();

	/**
	 * Initialise this object, and attempt to connect to the game server
	 * @param PublicID - The public ID of the local player
	 * @param AuthToken - The auth token for the local player
	 * @param MatchID - The match ID for the match that the local player is attempting to join
	 */
	void Initialise(const FString& PublicID, const FString& AuthToken, uint64 MatchID);

	/* Tick the server */
	virtual void Tick(float DeltaSeconds);

	/**
	 * Get the next update from the server.
	 * @return The update
	 */
	virtual const FB2ServerUpdate GetNextUpdate() override;

	/**
	 * Attempt to connect to the game server again - only valid if the client has not yet connected
	 * @return false if the conditions are invalid (such as if the client has disconnected after being connected already)
	 */
	virtual bool Connect() override;

	/* Kill the server, so that it stops processing and sending/receiving messages */
	virtual void Kill();

private:

	/* The default headers to add to the connection */
	TArray<FWebSocketHeaderPair> DefaultHeaders;

	/* The websocket itself */
	UPROPERTY()
	UWebSocketBase* WebSocket;

	/* How many times we have tried to connect */
	uint32 ConnectionAttempts;

	/* Whether or not the connection is currently active */
	bool bConnected;
	
	/* Whether or not the connection has ever been active */
	bool bConnectionMade;

	/* For manually timing out the connection */
	float TimeSinceConnectionStart;
	bool bEnforceConnectionTimeout;

	/* For determining when to consider the connection as "made" */
	uint32 ConnectionStepsProcessed;

	/* Connection parameters */
	FString PublicID;
	FString AuthToken;
	uint64 MatchID;

	/**
	 * Attempt to initialise the websocket connection
	 * @return True if initialised
	 */
	bool SetupWSConnection();

	/* Set up the event listeners for the websocket connection */
	void SetupEventListeners();

	/**
	 * Creates a websocket packet containing the local players authentication information
	 * @return The packet
	 */
	FB2WebSocketPacket MakeAuthPacket() const;

	/**
	 * Creates a websocket packet containing match ID
	 * @return The packet
	 */
	FB2WebSocketPacket MakeMatchIDPacket() const;

	/* Handler for successful connection events */
	UFUNCTION()
	void HandleConnectionEvent();

	/* Handler for connection closed events */
	UFUNCTION()
	void HandleConnectionClosedEvent();

	/* Handler for connection error events */
	UFUNCTION()
	void HandleConnectionErrorEvent(const FString& Error);

	/* Handler for message received events */
	UFUNCTION()
	void HandleMessageReceivedEvent(const FString& Data);
};