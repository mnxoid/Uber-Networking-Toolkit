#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

//#define VERBOSE

void Getch()
{
	while (getchar() != '\n');
}

int TCP_cl(string ipStr, string port)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	char *sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Validate the parameters
// 	if (argc != 2) {
// 		printf("usage: %s server-name\n", argv[0]);
// 		return 1;
// 	}
	#ifdef VERBOSE
cout << "WSAStartup" << endl;
#endif
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(ipStr.c_str(), port.c_str(), &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		#ifdef VERBOSE
cout << "socket" << endl;
#endif
		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}
		#ifdef VERBOSE
cout << "connect" << endl;
#endif
		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	while (true)
	{
		string mess;
		cout << "Enter your message: ";
		cin >> mess;
		Getch();
		#ifdef VERBOSE
cout << "send" << endl;
#endif
		// Send an initial buffer
			iResult = send(ConnectSocket, mess.c_str(), (int)strlen(mess.c_str())+1, 0);
			if (iResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ConnectSocket);
				WSACleanup();
				return 1;
			}
	
			printf("Bytes Sent: %ld\n", iResult);
	
			// shutdown the connection since no more data will be sent
		
	
			// Receive until the peer closes the connection
			//do {
				#ifdef VERBOSE
cout << "recv" << endl;
#endif
				iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
				if (iResult > 0) {
					printf("Bytes received: %d\n", iResult);
					cout << recvbuf << endl;
				}
				else if (iResult == 0)
					printf("Connection closed\n");
				else
					printf("recv failed with error: %d\n", WSAGetLastError());
	
			//} while (iResult > 0);
				int i;
				cout << "Input 1 to continue or 0 to exit: ";
				cin >> i;
				Getch();
				if (!i)break;
	}
	#ifdef VERBOSE
cout << "shutdown" << endl;
#endif
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	do {
	#ifdef VERBOSE
cout << "recv" << endl;
#endif
	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0) {
		printf("Bytes received: %d\n", iResult);
		cout << recvbuf << endl;
	}
	else if (iResult == 0)
		printf("Connection closed\n");
	else
		printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);
	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();
}

int main(int argc, char ** argv)
{
	string ipStr;
	string port;
	if (argc > 1)
	{
		ipStr = string(argv[1]);
		port = string(argv[2]);
	}
	else 
	{
		cout << "Usage:\n-------------------------\n     tcp2std ip port\n-------------------------\n";
		cout << "Input server address: ";
		cin >> ipStr;
		Getch();
		cout << "Input port number: ";
		cin >> port;
		Getch();
	}
	cout << endl;
	cout << "######################################" << endl;
	cout << "#                                    #" << endl;
	cout << "# UBER CLIENT by Mnxoid x Icebreaker #" << endl;
	cout << "#                                    #" << endl;
	cout << "######################################" << endl;
	cout << "Connecting to " << ipStr << ":" << port << "..." << endl;
	if (true)
	{
		TCP_cl(ipStr, port);
	}
	Getch();
	return 0;
}