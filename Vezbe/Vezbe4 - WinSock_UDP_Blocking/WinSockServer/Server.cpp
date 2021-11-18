#include <winsock2.h>
#include <stdio.h>

#define SERVER_PORT1 15011
#define SERVER_PORT2 15012
#define SERVER_SLEEP_TIME 300
#define ACCESS_BUFFER_SIZE 1024
#define IP_ADDRESS_LEN 16
#define SERVER_IP_ADDERESS "127.0.0.1"


// Initializes WinSock2 library
// Returns true if succeeded, false otherwise.
bool InitializeWindowsSockets();

int main(int argc,char* argv[])
{
    // Server address
    sockaddr_in serverAddress1;
    sockaddr_in serverAddress2;
	// Server's socket
    int serverPort1 = SERVER_PORT1;
    int serverPort2 = SERVER_PORT2;
	// size of sockaddr structure
    int sockAddrLen=sizeof(struct sockaddr);
	// buffer we will use to receive client message
    char accessBuffer[ACCESS_BUFFER_SIZE];
	// variable used to store function return value
	int iResult;

    if(InitializeWindowsSockets() == false)
	{
		// we won't log anything since it will be logged
		// by InitializeWindowsSockets() function
		return 1;
	}

    // Initialize serverAddress structure used by bind
    memset((char*)&serverAddress1,0,sizeof(serverAddress1));
    serverAddress1.sin_family = AF_INET; /*set server address protocol family*/
    serverAddress1.sin_addr.s_addr = INADDR_ANY;
    serverAddress1.sin_port = htons(serverPort1);

    memset((char*)&serverAddress2, 0, sizeof(serverAddress2));
    serverAddress2.sin_family = AF_INET; /*set server address protocol family*/
    serverAddress2.sin_addr.s_addr = INADDR_ANY;
    serverAddress2.sin_port = htons(serverPort2);


    // create a socket
    SOCKET serverSocket1 = socket(AF_INET,      // IPv4 address famly
								 SOCK_DGRAM,   // datagram socket
								 IPPROTO_UDP); // UDP
    SOCKET serverSocket2 = socket(AF_INET,      // IPv4 address famly
                                SOCK_DGRAM,   // datagram socket
                                IPPROTO_UDP); // UDP

    unsigned long mode = 1;
    iResult = ioctlsocket(serverSocket1, FIONBIO, &mode);
	// check if socket creation succeeded
    if (serverSocket1 == INVALID_SOCKET)
    {
        printf("Creating socket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    iResult = ioctlsocket(serverSocket2, FIONBIO, &mode);
    if (serverSocket2 == INVALID_SOCKET)
    {
        printf("Creating socket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Bind port number and local address to socket
    iResult = bind(serverSocket1,(LPSOCKADDR)&serverAddress1,sizeof(serverAddress1));

    if (iResult == SOCKET_ERROR)
    {
        printf("Socket bind failed with error: %d\n", WSAGetLastError());
        closesocket(serverSocket1);
        WSACleanup();
        return 1;
    }

    iResult = bind(serverSocket2, (LPSOCKADDR)&serverAddress2, sizeof(serverAddress2));

    if (iResult == SOCKET_ERROR)
    {
        printf("Socket bind failed with error: %d\n", WSAGetLastError());
        closesocket(serverSocket2);
        WSACleanup();
        return 1;
    }
    
    printf("Simple UDP server started and waiting client messages.\n");

    // Main server loop

    while(1)
    {
        // clientAddress will be set from recvfrom
        sockaddr_in clientAddress;
		memset(&clientAddress, 0, sizeof(sockaddr_in));

		// set whole buffer to zero
        memset(accessBuffer, 0, ACCESS_BUFFER_SIZE);

        fd_set readfds, errorfds;
        FD_ZERO(&readfds);
        FD_ZERO(&errorfds);
        FD_SET(serverSocket1, &readfds);
        FD_SET(serverSocket2, &readfds);
        FD_SET(serverSocket1, &errorfds);
        FD_SET(serverSocket2, &errorfds);

        // maksimalni period cekanja select funkcije 
        timeval timeVal;
        timeVal.tv_sec = 1;
        timeVal.tv_usec = 0;
        // poziv select funkcije koja omogucava da se saceka definisani 
        // dogadjaj u period od 1 sekunde 
        int result = select(0, &readfds, NULL, NULL, &timeVal);
        int error = select(0, NULL, NULL, &errorfds, &timeVal);

        if (result == 0 || error != 0) {
            // vreme za cekanje je isteklo
            printf("Waiting...\n");
            Sleep(SERVER_SLEEP_TIME);
            continue;
        }
        
        if (result == SOCKET_ERROR || error == SOCKET_ERROR) {
            //desila se greska prilikom poziva funkcije
            printf("Error!\n");
            continue;
        }
        
        // rezultat je jednak broju soketa koji su zadovoljili uslov
        if (FD_ISSET(serverSocket1, &readfds)) {
            // izvrsenje operacije
            // receive client message
            iResult = recvfrom(serverSocket1,
                accessBuffer,
                ACCESS_BUFFER_SIZE,
                0,
                (LPSOCKADDR)&clientAddress,
                &sockAddrLen);

            if (iResult == SOCKET_ERROR)
            {
                printf("recvfrom failed with error: %d\n", WSAGetLastError());
                continue;
            }

            char ipAddress[IP_ADDRESS_LEN];
            // copy client ip to local char[]
            strcpy_s(ipAddress, sizeof(ipAddress), inet_ntoa(clientAddress.sin_addr));
            // convert port number from TCP/IP byte order to
            // little endian byte order
            int clientPort = ntohs((u_short)clientAddress.sin_port);

            printf("Client connected from ip: %s, port: %d, sent: %s.\n", ipAddress, clientPort, accessBuffer);

            // possible message processing logic could be placed here

            FD_CLR(serverSocket1, &readfds);

        }
        else if (FD_ISSET(serverSocket2, &readfds)) {
            // izvrsenje operacije
            // receive client message
            iResult = recvfrom(serverSocket2,
                accessBuffer,
                ACCESS_BUFFER_SIZE,
                0,
                (LPSOCKADDR)&clientAddress,
                &sockAddrLen);

            if (iResult == SOCKET_ERROR)
            {
                printf("recvfrom failed with error: %d\n", WSAGetLastError());
                continue;
            }

            char ipAddress[IP_ADDRESS_LEN];
            // copy client ip to local char[]
            strcpy_s(ipAddress, sizeof(ipAddress), inet_ntoa(clientAddress.sin_addr));
            // convert port number from TCP/IP byte order to
            // little endian byte order
            int clientPort = ntohs((u_short)clientAddress.sin_port);

            printf("Client connected from ip: %s, port: %d, sent: %s.\n", ipAddress, clientPort, accessBuffer);

            // possible message processing logic could be placed here

            FD_CLR(serverSocket2, &readfds);

        }
            
        if (FD_ISSET(serverSocket1, &errorfds)) {
            iResult = closesocket(serverSocket1);
            if (iResult == SOCKET_ERROR)
                printf("closesocket failed with error: %ld\n", WSAGetLastError());
        }

        if (FD_ISSET(serverSocket2, &errorfds)) {
            iResult = closesocket(serverSocket2);
            if (iResult == SOCKET_ERROR)
                printf("closesocket failed with error: %ld\n", WSAGetLastError());
        }
		
    }

    // if we are here, it means that server is shutting down
	// close socket and unintialize WinSock2 library
    iResult = closesocket(serverSocket1);
    if (iResult == SOCKET_ERROR)
    {
        printf("closesocket failed with error: %ld\n", WSAGetLastError());
        return 1;
    }

    iResult = closesocket(serverSocket2);
    if (iResult == SOCKET_ERROR)
    {
        printf("closesocket failed with error: %ld\n", WSAGetLastError());
        return 1;
    }

    iResult = WSACleanup();
    if (iResult == SOCKET_ERROR)
    {
        printf("WSACleanup failed with error: %ld\n", WSAGetLastError());
        return 1;
    }
	
	printf("Server successfully shut down.\n");
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
