#pragma once
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <Windows.h>
#include <string.h>
#include <tchar.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

class Logger
{
public:
	Logger(void);
	~Logger(void);
	static void attachConsole();
	static void detachConsole();
	static void log(string msg);
};
#endif