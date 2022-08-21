#include <iostream>
#include <WS2tcpip.h>
#include "main.h";
#include "Config.h"
#include "PacketHandler.h"

#pragma comment (lib, "ws2_32.lib")

//using namespace std;
const unsigned short SERVER_PORT = 25565;

int main(void*)
{
	std::cout << "[Minecraft Server] Starting up mcserver v1.18.2 by Lukas Fend" << std::endl;
	// Initialize winsock
	WSADATA wsData;
	WORD version = MAKEWORD(2, 2);

	int wsOk = WSAStartup(version, &wsData);
	if (wsOk != 0) 
	{
		std::cerr << "Failed initializing winsock" << std::endl;
		return EXIT_FAILURE;
	}

	// Create socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) 
	{
		std::cerr << "Failed initializing socket." << std::endl;
		return EXIT_FAILURE;
	}

	// Bind socket to address
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(SERVER_PORT); // inverts bit array (little/big endian)
	hint.sin_addr.S_un.S_addr = INADDR_ANY;
	
	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Start listening on socket
	listen(listening, SOMAXCONN);

	// Wait for connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	if (clientSocket == INVALID_SOCKET) 
	{
		std::cerr << "Error creating client socket." << std::endl;
		return EXIT_FAILURE;
	}

	char host[NI_MAXHOST];		// Client's remote name
	char service[NI_MAXSERV];	// Service port


	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0x0)) 
	{
		std::cout << host << " connected on port " << service << std::endl;
	}
	else 
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on port "
			<< ntohs(client.sin_port) << std::endl;
	}

	// Close listening socket
	closesocket(listening);

	// While loop - accept & echo message back to client
	uint8_t buffer[BUFFER_SIZE];

	while (true) 
	{
		ZeroMemory(buffer, BUFFER_SIZE);

		// Wait for data from client
		int bytesReceived = recv(clientSocket, (char*)buffer, BUFFER_SIZE, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			std::cerr << "Error in recv(). Aborting..." << std::endl;
			break;
		}
		if (bytesReceived == 0)
		{
			std::cout << "Client disconnected." << std::endl;
			break;
		}

		printf("[Client -> Server] \n");
		DumpHex(buffer, BUFFER_SIZE);
		PacketHandler::ReceivePacket(buffer, 0, &clientSocket);

		// Respond

		// Echo message back to client
		//send(clientSocket, (const char*)_responseBuffer, _responseBufferSize, 0);
	}
	
	// Close socket
	closesocket(clientSocket);

	// Shutdown/cleanup winsock
	WSACleanup();
}
