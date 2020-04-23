#pragma once

#include "B2Engine/Server.h"

#include "WebSocketBlueprintLibrary.h"

#include "NetServer.generated.h"

UCLASS()
class UB2NetServer : public UB2Server
{
	GENERATED_BODY()

public:

	/**
	 * Get the next update from the server.
	 * @return The update
	 */
	bool Initialise(const FString& PublicID, const FString& AuthToken, uint64 MatchID);

	/* Tick the server */
	virtual void Tick(float DeltaSeconds);

	/**
	 * Get the next update from the server.
	 * @return The update
	 */
	virtual const FB2ServerUpdate GetNextUpdate() override;

private:

	/* The default headers to add to the connection */
	TArray<FWebSocketHeaderPair> DefaultHeaders;

	/* The websocket itself */
	UWebSocketBase* WebSocket;

	/* How many times we have tried to connect (or reconnect) */
	uint32 ConnectionAttempts;

	/* Whether or not the connection is currently active */
	bool bConnected;

	/**
	 * Attempt to connect to the game server
	 * @return True if the connection was made successfully, else false
	 */
	bool Connect(const FString& PublicID, const FString& AuthToken, uint64 MatchID);

	/* Set up the event listeners for the websocket connection */
	void SetupEventListeners();

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