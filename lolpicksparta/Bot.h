#pragma once
#ifndef _BOT_H_
#define _BOT_H_

#include "Heimy.h"
#include "Logger.h"
#include "Structs.h"

class Bot
{
	volatile bool stop;
public:
	Bot(void);
	~Bot(void);
	void requestStop() { stop = true; };
	bool isRunning;
	void buildTargets();
	bool doAcceptGame();
	bool doCallRole();
	bool doPickChampion();
	void processChanges();
	DWORD Bot::startThread();
	DWORD Bot::threadProxy();
	static DWORD WINAPI Bot::BotThread(void* Param);
	char* message;
	char* champion;
	int currentStep;
	bool doStepOne, doStepTwo, doStepThree;
	void setChampion(char* champ);
	void setMessage(char* msg);
private:
	WindowProps lolProps;
	TARGETRECT chatRect;
	TARGET timer, accept, chat, search, select, tokenScreen, tokenChampion;
	TARGET _chat;
	HDC _dc;
};
#endif

