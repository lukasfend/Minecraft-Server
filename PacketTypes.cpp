#include "PacketTypes.h"
#include <iostream>
#include <intrin.h>
#include "Config.h"
#include "Logger.h"

int32_t PacketTypes::readVarInt(uint8_t* buffer, uint16_t &offset)
{
	int32_t value = 0;
	int position = 0;
	int8_t currentByte;

	while (true) {
		currentByte = buffer[offset++];
		value |= ((uint8_t)currentByte & PacketTypes::SEGMENT_BITS) << position;
		if ((currentByte & PacketTypes::CONTINUE_BIT) == 0) break;
		position += 7;

		if (position >= 32) {
			Logger::printRaw("Error: VarInt is too big.", LOG_LEVEL::ERR);
		}
	}

	return value;
}

uint32_t PacketTypes::readVarUInt(uint8_t* buffer, uint16_t &offset)
{
	uint32_t value = 0;
	int position = 0;
	uint8_t currentByte;

	while (true) {
		currentByte = buffer[offset++];
		value |= (currentByte & PacketTypes::SEGMENT_BITS) << position;
		if ((currentByte & PacketTypes::CONTINUE_BIT) == 0) break;
		position += 7;

		if (position >= 32) {
			Logger::printRaw("Error: VarInt is too big.", LOG_LEVEL::ERR);
		}
	}

	return value;
}

char* PacketTypes::readString(uint8_t* buffer, uint16_t &offset)
{
	uint16_t stringLength = PacketTypes::readVarInt(buffer, offset);
	char* data = new char[stringLength];
	std::fill(data, data + stringLength, 0);
	for (uint16_t i = 0; i < stringLength; i++) {
		data[i] = buffer[offset++];
	}
	return data;
}

int16_t PacketTypes::readShort(uint8_t* buffer, uint16_t& offset)
{
	offset++;
	uint16_t val = PacketTypes::swapEndianness((uint16_t)((buffer[offset] << 8) & 0xff00) | (buffer[offset - 1] & 0x00ff));
	offset++;
	return val;
}

uint16_t PacketTypes::readUShort(uint8_t* buffer, uint16_t& offset)
{
	offset++;
	uint16_t val = PacketTypes::swapEndianness((buffer[offset] << 8) | buffer[offset-1]);
	offset++;
	return val;
}
int64_t PacketTypes::readLong(uint8_t* buffer, uint16_t& offset)
{
	int64_t value = 0;
	for (int i = 0; i < 8; i++) {
		value <<= 8;
		value |= buffer[offset++];
	}
	return value;
}
uint64_t PacketTypes::readULong(uint8_t* buffer, uint16_t& offset)
{
	return (uint64_t) PacketTypes::readLong(buffer, offset);
}

// TODO: well....
uint16_t PacketTypes::swapEndianness(uint16_t value)
{
	return
		(value << 8 & 0b1000000000000000) |
		(value << 8 & 0b0100000000000000) |
		(value << 8 & 0b0010000000000000) |
		(value << 8 & 0b0001000000000000) |
		(value << 8 & 0b0000100000000000) |
		(value << 8 & 0b0000010000000000) |
		(value << 8 & 0b0000001000000000) |
		(value << 8 & 0b0000000100000000) |
		(value >> 8 & 0b0000000010000000) |
		(value >> 8 & 0b0000000001000000) |
		(value >> 8 & 0b0000000000100000) |
		(value >> 8 & 0b0000000000010000) |
		(value >> 8 & 0b0000000000001000) |
		(value >> 8 & 0b0000000000000100) |
		(value >> 8 & 0b0000000000000010) |
		(value >> 8 & 0b0000000000000001);
}

void PacketTypes::writeVarInt(int32_t value, char*& buffer, uint16_t& bufferSize)
{
	while (true)
	{
		if ((value & ~SEGMENT_BITS) == 0)
		{
			buffer[bufferSize++] = value;
			return;
		}

		buffer[bufferSize++] = (value & SEGMENT_BITS) | CONTINUE_BIT;

		value >>= 7;
	}
}

uint32_t PacketTypes::getVarIntLength(int32_t value)
{
	int32_t* virtualBuffer = new int32_t[BUFFER_SIZE];
	uint16_t virtualBufferSize = 0;
	while (true)
	{
		if ((value & ~SEGMENT_BITS) == 0)
		{
			virtualBuffer[virtualBufferSize++] = value;
			return virtualBufferSize;
		}

		virtualBuffer[virtualBufferSize++] = (value & SEGMENT_BITS) | CONTINUE_BIT;

		value >>= 7;
	}
}

void PacketTypes::writeString(char* value, uint16_t byteLength, char*& buffer, uint16_t& bufferSize)
{
	writeVarInt(byteLength, buffer, bufferSize);
	for (uint16_t i = 0; i < byteLength; i++)
	{
		buffer[bufferSize++] = value[i];
	}
}