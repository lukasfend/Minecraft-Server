#pragma once
#include <stdint.h>

class PacketTypes
{
public:
	static const int SEGMENT_BITS = 0x7F;
	static const int CONTINUE_BIT = 0x80;

	static int32_t readVarInt(uint8_t* buffer, uint16_t &offset);
	static int32_t readVarUInt(uint8_t* buffer, uint16_t &offset);
	static int64_t readVarLong(uint8_t* buffer, uint16_t &offset);
	static int64_t readVarULong(uint8_t* buffer, uint16_t &offset);
};

