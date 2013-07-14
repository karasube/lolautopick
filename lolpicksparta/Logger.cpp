#include "Logger.h"


Logger::Logger(void)
{
}

void Logger::attachConsole() {
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);
	printf("[LoL AutoPick] SPARTA Release\n* Console has been attached.\n");
}

void Logger::log(string msg) {
	printf("* %s\n", msg.c_str());
}

Logger::~Logger(void)
{
}