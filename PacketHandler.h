#pragma once
#include <stdint.h>

class PacketHandler
{
public:
	PacketHandler();
	~PacketHandler();
	static void ReceivePacket(uint8_t* buffer, uint16_t offset);
};

