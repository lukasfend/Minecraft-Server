#include "PacketHandler.h"
#include <stdio.h>
#include "PacketTypes.h"
#include "Config.h"
#include <iostream>

void PacketHandler::ReceivePacket(uint8_t* buffer, uint16_t offset)
{
	std::cout << "Reading new Packet..." << std::endl;

	uint8_t state = 0;

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
			switch (packetId)
			{
				case 0x00:
					// TODO send string
					break;
				default:
					offset += packetLength - 1;
					break;
			}
		}

		if (buffer[offset] == 0) {
			break;
		}

		PacketHandler::ReceivePacket(buffer, offset);
	}
}