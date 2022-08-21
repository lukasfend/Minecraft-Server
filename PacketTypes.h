#pragma once
#include <stdint.h>
#include <WS2tcpip.h>

class PacketTypes
{
public:
	static const uint8_t SEGMENT_BITS = 0b01111111;
	static const uint8_t CONTINUE_BIT = 0b10000000;

	// Reading
	static int32_t readVarInt(uint8_t* buffer, uint16_t& offset);
	static uint32_t readVarUInt(uint8_t* buffer, uint16_t& offset);
	static int64_t readVarLong(uint8_t* buffer, uint16_t& offset);
	static uint64_t readVarULong(uint8_t* buffer, uint16_t& offset);
	static char* readString(uint8_t* buffer, uint16_t& offset);
	static int16_t readShort(uint8_t* buffer, uint16_t& offset);
	static uint16_t readUShort(uint8_t* buffer, uint16_t& offset);
	static int64_t readLong(uint8_t* buffer, uint16_t& offset);
	static uint64_t readULong(uint8_t* buffer, uint16_t& offset);

	// Writing
	static void writeVarInt(int32_t value, char* &buffer, uint16_t &bufferSize);
	static void writeString(char* value, uint16_t byteLength, char*& buffer, uint16_t& bufferSize);

	static uint32_t getVarIntLength(int32_t value);

	static uint16_t swapEndianness(uint16_t value);
};

