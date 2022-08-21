#pragma once
#include <string>
#include <iostream>

enum LOG_LEVEL {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERR = 3,
    CRITICAL = 4
};

class Logger
{
public:
    static void printRaw(char* text, uint8_t logLevel = 0, ...);
    static void printRaw(const char* text, uint8_t logLevel = 0, ...);
    static void logIncoming(uint8_t* buffer, uint16_t bufferLength);
    static void logOutgoing(uint8_t* buffer, uint16_t bufferLength);
private:
	static const char* getTime();
	static const char* getLevel(uint8_t logLevel);
    static const char* PREFIX_INBOUND;
    static const char* PREFIX_OUTBOUND;
};

static std::string currentDateTime() {
    struct tm newtime;
    time_t now = time(0);
    char* buf = new char[80];
    localtime_s(&newtime, &now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &newtime);

    return buf;
}