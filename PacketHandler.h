#pragma once
#include <stdint.h>
#include <WS2tcpip.h>

class PacketHandler
{
public:
	PacketHandler();
	~PacketHandler();
	static void ReceivePacket(uint8_t* buffer, uint16_t offset, SOCKET &clientSocket);
	static void SendPacket(uint32_t packetId, char* &buffer, uint16_t &bufferLength, SOCKET& clientSocket);
};

