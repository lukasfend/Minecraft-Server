#include "PacketTypes.h"
#include <iostream>

int32_t PacketTypes::readVarInt(uint8_t* buffer, uint16_t& offset)
{
	int numRead = 0;
	int32_t result = 0;
	do
	{
		int16_t value = (buffer[offset] & SEGMENT_BITS);
		result |= (value << (7 * numRead));
	} while ((buffer[offset++] & CONTINUE_BIT) != 0);

	return result;
}