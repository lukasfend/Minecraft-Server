#include "PacketHandler.h"
#include <stdio.h>
#include "PacketTypes.h"

void PacketHandler::ReceivePacket(uint8_t* buffer)
{
	int bufferSize = sizeof(buffer);
	for (uint16_t i = 0; i < bufferSize; i++)
	{
		printf("Val: %d ", (uint8_t)buffer[i]);
		int packetLength = PacketTypes::readVarInt(buffer, &i);
		printf("Packet Length: %d\n", packetLength);
	}
}