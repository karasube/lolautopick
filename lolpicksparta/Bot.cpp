#include "Bot.h"


Bot::Bot(void)
{
	stop = false;
	isRunning = false;
	Logger::log("Starting bot");
	this->lolProps.hwnd = FindWindow(L"ApolloRuntimeContentWindow", NULL);
	if(!this->lolProps.hwnd) {
		MessageBox(NULL, L"Couldn't find LoL Window.", L"Damn!", NULL);
		PostQuitMessage(0);
	} else {
		Logger::log("Found LoL HWND.");
	}

	this->_dc = GetDC(NULL);
	if(!this->_dc) {
		MessageBox(NULL, L"Couldn't find DC.", L"Damn!", NULL);
		PostQuitMessage(0);
	} else {
		Logger::log("Found DC.");
	}

	this->lolProps.dc = GetDC(this->lolProps.hwnd);
	if(!this->lolProps.dc) {
		MessageBox(NULL, L"Couldn't find LoL DC.", L"Damn!", NULL);
		PostQuitMessage(0);
	} else {
		Logger::log("Found LoL DC.");
	}

	winActivate(this->lolProps.hwnd);
	getWindowProps(lolProps);
	this->buildTargets();

	Logger::log("Targets are in memory!");
	currentStep = 1;
	doStepOne = true;
	doStepTwo = true;
	doStepThree = true;
	//startThread();
}

void Bot::buildTargets() {
	translateTarget(lolProps, 512, 332, timer);
	translateTarget(lolProps, 430, 358, accept);
	translateTarget(lolProps, 665, 588, chat);
	translateTarget(lolProps, 747, 108, search);
	translateTarget(lolProps, 256, 166, select);
	translateTarget(lolProps, 512, 32, tokenScreen);
	translateTargetRect(lolProps, 225, 471, 245, 486, chatRect);	
}

bool Bot::doAcceptGame() {
	this->processChanges();
	if(checkPixelColor(lolProps.dc, RGB(255,255,255), timer.lol) || checkPixelColor(lolProps.dc, RGB(0,0,0), timer.lol)) {
		winActivate(lolProps.hwnd);
		this->processChanges();
		moveAndClick(accept.screen);
		return true;
	} else {
		return false;
	}
}

bool Bot::doCallRole() {
	this->processChanges();
	if(checkPixelColor(lolProps.dc, RGB(255,255,255), chat.lol)) {
		winActivate(lolProps.hwnd);
		this->processChanges();
		moveAndClick(chat.screen);
		moveAndClick(chat.screen);
		say(message, TRUE);

		while(!searchForPixelColor(lolProps.dc, chatRect, 2, 2, RGB(203, 254, 0), RGB(0, 194, 243), RGB(254, 254, 254))) {
			Sleep(1);
		}
		say(message, TRUE);

		return true;
	}
	
	return false;
}

bool Bot::doPickChampion() {
	if(!doStepTwo) {
		if(!checkPixelColor(lolProps.dc, RGB(255,255,255), chat.lol)) {
			return false;
		}
		
	}
	this->processChanges();
	winActivate(lolProps.hwnd);
	moveAndClick(search.screen);
	moveAndClick(search.screen);
	say(champion, FALSE);
	moveAndClick(select.screen);
	return true;
}



void Bot::processChanges() {
	if(checkForChanges(lolProps)) {
		this->buildTargets();
	}
}

DWORD Bot::startThread() {
	if(!isRunning) {
		if(stop) {
			stop = false;
		}
		DWORD ThreadID;
		CreateThread(NULL, 0, BotThread, (void*) this, 0, &ThreadID);
		return ThreadID;
	}
	return 0;
}

DWORD Bot::threadProxy() {
	isRunning = true;
	if(doStepThree && currentStep != 3) { currentStep = 3; }
	if(doStepTwo && currentStep != 2) { currentStep = 2; }
	if(doStepOne && currentStep != 1) { currentStep = 1; }

	if(doStepOne && currentStep == 1) {
		Logger::log("[BOT] Step 1 - auto-accept game.");
		while(!this->doAcceptGame()) {
			Sleep(1);
			if(stop) {
				currentStep = 0;
				isRunning = false;
				return 0;
			}
		}
		if(doStepTwo) {
			currentStep = 2;
		} else {
			currentStep = 3;
		}
	}
	
	if(doStepTwo && currentStep == 2) {
		Logger::log("[BOT] Step 2 - call role.");
		while(!this->doCallRole()) {
			Sleep(1);
			if(stop) {
				currentStep = 0;
				isRunning = false;
				return 0;
			}
		}
		if(doStepThree) {
			currentStep = 3;
		} else {
			currentStep = 0;
		}
	}

	if(doStepThree && currentStep == 3) {
		Logger::log("[BOT] Step 3 - pick champion.");
		while(!this->doPickChampion()) {
			Sleep(1);
			if(stop) {
				currentStep = 0;
				isRunning = false;
				return 0;
			}
		}
		currentStep = 0;
		isRunning = false;
	}
	return 0;
}

DWORD WINAPI Bot::BotThread(void* Param) {
	Bot* This = (Bot*) Param;
	return This->threadProxy();
}

void Bot::setChampion(char* champ) {
	champion = champ;
}
void Bot::setMessage(char* msg) {
	message = msg;
}

Bot::~Bot(void)
{
}
