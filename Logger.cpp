#include "Logger.h"
#include <cstdint>
#include <cstdarg>
#include "DumpHex.h"

const char* Logger::PREFIX_INBOUND = "[Client -> Server] ";
const char* Logger::PREFIX_OUTBOUND = "[Server -> Client] ";

void Logger::printRaw(char* text, uint8_t logLevel,...)
{
	printf("%s [%s] ", Logger::getTime(), Logger::getLevel(logLevel));
	va_list args;
	va_start(args, text);
	vprintf(text, args);
	std::cout << std::endl;
	va_end(args);
}
void Logger::printRaw(const char* text, uint8_t logLevel, ...)
{
	printf("%s [%s] ", Logger::getTime(), Logger::getLevel(logLevel));
	va_list args;
	va_start(args, text);
	vprintf(text, args);
	std::cout << std::endl;
	va_end(args);
}

void Logger::logIncoming(uint8_t* buffer, uint16_t bufferLength)
{
	Logger::printRaw(Logger::PREFIX_INBOUND, LOG_LEVEL::DEBUG);
	DumpHex(buffer, bufferLength);
}
void Logger::logOutgoing(uint8_t* buffer, uint16_t bufferLength)
{
	Logger::printRaw(Logger::PREFIX_OUTBOUND, LOG_LEVEL::DEBUG);
	DumpHex(buffer, bufferLength);
}

const char* Logger::getLevel(uint8_t logLevel) 
{
	switch (logLevel)
	{
		case 0:
			return "DEBUG";
		case 1:
			return "INFO";
		case 2:
			return "WARNING";
		case 3:
			return "ERROR";
		case 4:
			return "CRITICAL";

		default:
			return "DEBUG";

	}
}
const char* Logger::getTime()
{
	return currentDateTime().c_str();
}