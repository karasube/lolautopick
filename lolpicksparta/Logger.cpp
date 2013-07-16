#include "Logger.h"


Logger::Logger(void)
{
}

void Logger::attachConsole() {
		AllocConsole();
		AttachConsole(GetCurrentProcessId());
		freopen("CON", "w", stdout);
}

void Logger::detachConsole() {
		FreeConsole();
}

void Logger::log(string msg) {
		printf("* %s\n", msg.c_str());
}

Logger::~Logger(void)
{
}