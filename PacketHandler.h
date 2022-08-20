#pragma once
class PacketHandler
{
public:
	PacketHandler();
	~PacketHandler();
	static void ReceivePacket(uint8_t* buffer);
};

