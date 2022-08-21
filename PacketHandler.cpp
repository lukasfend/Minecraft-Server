#include "PacketHandler.h"
#include <stdio.h>
#include "PacketTypes.h"
#include "Config.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


void PacketHandler::ReceivePacket(uint8_t* buffer, uint16_t offset, SOCKET& clientSocket)
{
	std::cout << "Reading new Packet..." << std::endl;

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
						json response = {
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
						};	
						// TODO send string
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

void PacketHandler::SendPacket(char* buffer, uint16_t bufferLength, SOCKET& clientSocket)
{

}