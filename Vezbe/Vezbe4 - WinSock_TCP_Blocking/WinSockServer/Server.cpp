#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27016"
#define MAX_CLIENT_SIZE 10

bool InitializeWindowsSockets();

int  main(void) 
{
	int connectedClients = 0;
	
    // Socket used for listening for new clients 
    SOCKET listenSocket = INVALID_SOCKET;
    // Socket used for communication with client
	SOCKET acceptedSockets[MAX_CLIENT_SIZE];

	for (int i = 0; i < MAX_CLIENT_SIZE; i++) {
		acceptedSockets[i] = INVALID_SOCKET;
	}

    // variable used to store function return value
    int iResult;
    // Buffer used for storing incoming data
    char recvbuf[DEFAULT_BUFLEN];
    
    if(InitializeWindowsSockets() == false)
    {
		// we won't log anything since it will be logged
		// by InitializeWindowsSockets() function
		return 1;
    }
    
    // Prepare address information structures
    addrinfo *resultingAddress = NULL;
    addrinfo hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // IPv4 address
    hints.ai_socktype = SOCK_STREAM; // Provide reliable data streaming
    hints.ai_protocol = IPPROTO_TCP; // Use TCP protocol
    hints.ai_flags = AI_PASSIVE;     // 

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &resultingAddress);
    if ( iResult != 0 )
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    listenSocket = socket(AF_INET,      // IPv4 address famly
                          SOCK_STREAM,  // stream socket
                          IPPROTO_TCP); // TCP

    if (listenSocket == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(resultingAddress);
        WSACleanup();
        return 1;
    }

	unsigned long mode = 1;
	iResult = ioctlsocket(listenSocket, FIONBIO, &mode);

    // Setup the TCP listening socket - bind port number and local address 
    // to socket
    iResult = bind( listenSocket, resultingAddress->ai_addr, (int)resultingAddress->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(resultingAddress);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Since we don't need resultingAddress any more, free it
    freeaddrinfo(resultingAddress);

    // Set listenSocket in listening mode
    iResult = listen(listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

	printf("Server initialized, waiting for clients.\n");

	FD_SET readfds;
	FD_ZERO(&readfds);

	timeval timeVal;
	timeVal.tv_sec = 1;
	timeVal.tv_usec = 0;

    do
    {
		if (connectedClients < MAX_CLIENT_SIZE) {
			FD_SET(listenSocket, &readfds);
		}

		for (int i = 0; i < connectedClients; i++) {
			FD_SET(acceptedSockets[i], &readfds);
		}

		int result = select(0, &readfds, NULL, NULL, &timeVal);

		if (result == 0) {
			// vreme za cekanje je isteklo
			continue;
		}
		else if (result == SOCKET_ERROR) {
			//desila se greska prilikom poziva funkcije
			printf("select failed with error: %d\n", WSAGetLastError());
			break;
		}
		else {
			// rezultat je jednak broju soketa koji su zadovoljili uslov
			if (FD_ISSET(listenSocket, &readfds)) {
				acceptedSockets[connectedClients] = accept(listenSocket, NULL, NULL);

				if (acceptedSockets[connectedClients] == INVALID_SOCKET)
				{
					printf("accept failed with error: %d\n", WSAGetLastError());
					closesocket(listenSocket);
					WSACleanup();
					return 1;
				}

				connectedClients++;

			}
			else {
				for (int i = 0; i < connectedClients; i++) {
					if (FD_ISSET(acceptedSockets[i], &readfds)) {

						iResult = recv(acceptedSockets[i], recvbuf, DEFAULT_BUFLEN, 0);
						if (iResult > 0)
						{
							printf("Message received from client: %s.\n", recvbuf);
						}
						else if (iResult == 0)
						{
							// connection was closed gracefully
							printf("Connection with client closed.\n");
							closesocket(acceptedSockets[i]);

							for (int j = i; j < connectedClients; j++) {
								acceptedSockets[j] = acceptedSockets[j + 1];
							}
							connectedClients--;
						}
						else
						{
							// there was an error during recv
							printf("recv failed with error: %d\n", WSAGetLastError());
							closesocket(acceptedSockets[i]);

							for (int j = i; j < connectedClients; j++) {
								acceptedSockets[j] = acceptedSockets[j + 1];
							}
							connectedClients--;
						}

						FD_CLR(acceptedSockets[i], &readfds);
					}
				}
			}
		}
    } while (1);

    // shutdown the connection since we're done
	for (int i = 0; i < connectedClients; i++) {
		iResult = shutdown(acceptedSockets[i], SD_SEND);
		if (iResult == SOCKET_ERROR)
		{
			printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(acceptedSockets[i]);
			WSACleanup();
			return 1;
		}
		closesocket(acceptedSockets[i]);
	}
    // cleanup
    closesocket(listenSocket);
    WSACleanup();

    return 0;
}

bool InitializeWindowsSockets()
{
    WSADATA wsaData;
	// Initialize windows sockets library for this process
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        return false;
    }
	return true;
}
