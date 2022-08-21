#include "PacketHandler.h"
#include <stdio.h>
#include "PacketTypes.h"
#include "Config.h"
#include <iostream>
#include "DumpHex.h"
#include <string>
//#include <nlohmann/json.hpp>
#pragma execution_character_set("utf-8")
//using json = nlohmann::json;


void PacketHandler::ReceivePacket(uint8_t* buffer, uint16_t offset, SOCKET& clientSocket)
{
	std::cout << "------- Reading new Packet...-------" << std::endl;

	uint8_t state = 0;
	char* responseBuffer[BUFFER_SIZE];
	ZeroMemory(responseBuffer, BUFFER_SIZE);
	uint16_t responseBufferSize = 0;

	int bufferSize = BUFFER_SIZE;
	while (offset < bufferSize)
	{
		int packetLength = PacketTypes::readVarInt(buffer, offset);
		int packetId = PacketTypes::readVarInt(buffer, offset);
		printf("Packet Length: %d\n", packetLength);
		printf("Packet ID: 0x%x\n", packetId);

		if (state == 0)
		{
			switch (packetId)
			{
				case 0x00: 
				{

					uint32_t protocolVersion = PacketTypes::readVarUInt(buffer, offset);
					char* ipAddr = PacketTypes::readString(buffer, offset);
					uint16_t port = PacketTypes::readUShort(buffer, offset);
					int nextState = PacketTypes::readVarInt(buffer, offset);
					printf("Protocol Version: %d\n", protocolVersion);
					printf("IP Address: %s\n", ipAddr);
					printf("Port: %d\n", port);
					printf("Next State: %x\n", nextState);
					state = nextState;
					break;
				}
				default:
					offset += packetLength - 1;
					break;
			}
		}
		else if (state == 1)
		{
			switch(packetId)
			{
				case 0x00:
					{
						/*json response = {
							{"version", {
								{"name", "1.18.2"},
								{"protocol", 758},

							}},
							{"players", {
								{"max", 128},
								{"online", 123},
								{"sample", {} },
							}},
							{"description", {
								{"text", "Hello!"},
							}},
							{"favicon", "data:image/png;base64;iVBORw0KGgoAAAANSUhEUgAAAEAAAABACAYAAACqaXHeAAAAYklEQVR42u3QAQ0AAAgDIN8/9M3hhAhk2s5jESBAgAABAgQIECBAgAABAgQIECBAgAABAgQIECBAgAABAgQIECBAgAABAgQIECBAgAABAgQIECBAgAABAgQIECBAgAABAu5bgEm/gfv5CP0AAAAASUVORK5CYII="},
							{"previewsChat", true},
						};*/
						// TODO send string
						std::string json = "{\"description\":{\"text\":\"A c++ Minecraft Server\"},\"players\":{\"max\":2147483647,\"online\":9999},\"version\":{\"name\":\"1.18.2\",\"protocol\":758}}";
						char* jsonBuffer = const_cast<char*>(json.c_str());
						const uint16_t jsonBufferLength = strlen(jsonBuffer);
						printf("STRLEN=%d", jsonBufferLength);
						char* stringBuffer = new char[jsonBufferLength];
						uint16_t stringBufferSize = 0;
						ZeroMemory(stringBuffer, jsonBufferLength);
						PacketTypes::writeString(jsonBuffer, jsonBufferLength, stringBuffer, stringBufferSize);
						printf("\n--------- STRING BUFFER: ---------\n");
						DumpHex(stringBuffer, stringBufferSize);
						PacketHandler::SendPacket(0x00, stringBuffer, stringBufferSize, clientSocket);
						break;
					}
				default:
					offset += packetLength - 1;
					break;
			}
		}

		if (buffer[offset] == 0) {
			break;
		}

		PacketHandler::ReceivePacket(buffer, offset, clientSocket);
	}
}

void PacketHandler::SendPacket(uint32_t packetId, char* &buffer, uint16_t &bufferLength, SOCKET& clientSocket)
{
	// The function getVarIntLength might be wrong!
	// Length of the packet contents itself
	uint32_t packetLength = PacketTypes::getVarIntLength(packetId) + sizeof(char) * bufferLength;
	// length of the packet-length prefix itself
	uint32_t packetLengthLength = PacketTypes::getVarIntLength(packetLength);
	// total packet length to be sent
	uint32_t totalPacketLength = packetLength + packetLengthLength;

	printf("Sending Data: %s", buffer);

	char* sendBuffer = new char[totalPacketLength];
	uint16_t sendBufferSize = 0;
	ZeroMemory(sendBuffer, totalPacketLength);

	// send packet length first
	PacketTypes::writeVarInt(packetLength, sendBuffer, sendBufferSize);
	// send packet id next
	
	PacketTypes::writeVarInt(packetId, sendBuffer, sendBufferSize);

	// send packet buffer
	for (uint32_t i = 0; i < bufferLength; i++)
	{
		//printf("Sending buffer index %d at original index %d (MAX: %d)\n", sendBufferSize, (1 + sendBufferSize), bufferLength);
		sendBuffer[sendBufferSize] = buffer[sendBufferSize-3];
		sendBufferSize++;
	}

	std::cout << "[Server -> Client] \n";
	DumpHex(sendBuffer, sendBufferSize);

	// dispatch
	send(clientSocket, sendBuffer, sendBufferSize, 0x00);
	// reset buffer for next outbound traffic
	ZeroMemory(buffer, BUFFER_SIZE);
	bufferLength = 0;
}