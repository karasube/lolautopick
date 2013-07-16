// Heimy.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Heimy.h"

HEIMY_API bool searchForPixelColor(HDC dc, TARGETRECT rect, int xInc, int yInc, COLORREF _color, COLORREF _color2, COLORREF _color3){
	POINT p;
	for(p.y = rect.startY; p.y < rect.endY; p.y = p.y + yInc) {
		for(p.x = rect.startX; p.x < rect.endX; p.x = p.x + xInc) {
			if(_color != NULL) {
				if(checkPixelColor(dc, _color, p)) {
					return true;
				}
			}

			if(_color2 != NULL) {
				if(checkPixelColor(dc, _color2, p)) {
					return true;
				}
			}

			if(_color3 != NULL) {
				if(checkPixelColor(dc, _color3, p)) {
					return true;
				}
			}
		}
	}
	return false;
}

HEIMY_API bool checkPixelColor(HDC dc, COLORREF color, POINT pos){
	COLORREF _color = GetPixel(dc, pos.x, pos.y);
	if(color == _color) {
		return true;
	} else {
		return false;
	}
}

HEIMY_API void keyPress(int vk, BOOL bExtended){
	KEYBDINPUT  kb = {0};
    INPUT       Input = {0};

    if (bExtended) { kb.dwFlags  = KEYEVENTF_EXTENDEDKEY; }
    kb.wVk  = vk;
    Input.type  = INPUT_KEYBOARD;
    Input.ki  = kb;
    SendInput(1, &Input, sizeof(Input));

    ZeroMemory(&kb, sizeof(KEYBDINPUT));
    ZeroMemory(&Input, sizeof(INPUT));
    kb.dwFlags  =  KEYEVENTF_KEYUP;
    if (bExtended) { kb.dwFlags |= KEYEVENTF_EXTENDEDKEY; }
    kb.wVk = vk;
    Input.type = INPUT_KEYBOARD;
    Input.ki = kb;
    SendInput(1, &Input, sizeof(Input));
}

HEIMY_API bool SendText(LPCSTR lpctszText) {
	std::vector<INPUT> EventQueue;

	char Buff[120] = {0};
	//GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ILANGUAGE, (LPWSTR) Buff, sizeof(Buff));
	//HKL hKeyBoardLayout = LoadKeyboardLayout((LPWSTR) Buff, KLF_ACTIVATE);

	const int Len = strlen(lpctszText);

	for(int Index = 0; Index < Len; ++Index) {
		INPUT Event = {0};

		const SHORT Vk = VkKeyScanEx(lpctszText[Index], NULL);
		const UINT VKey = MapVirtualKey(LOBYTE(Vk), 0);

		if(HIBYTE(Vk) == 1) {
			ZeroMemory(&Event, sizeof(Event));
			Event.type = INPUT_KEYBOARD;
			Event.ki.dwFlags = KEYEVENTF_SCANCODE;
			Event.ki.wScan = MapVirtualKey(VK_LSHIFT, 0);
			EventQueue.push_back(Event);
		}

		ZeroMemory(&Event, sizeof(Event));
		Event.type = INPUT_KEYBOARD;
		Event.ki.dwFlags = KEYEVENTF_SCANCODE;
		Event.ki.wScan = VKey;
		EventQueue.push_back(Event);

		ZeroMemory(&Event, sizeof(Event));
		Event.type = INPUT_KEYBOARD;
		Event.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
		Event.ki.wScan = VKey;
		EventQueue.push_back(Event);

		if(HIBYTE(Vk) == 1) {
			ZeroMemory(&Event, sizeof(Event));
			Event.type = INPUT_KEYBOARD;
			Event.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
			Event.ki.wScan = MapVirtualKey(VK_LSHIFT, 0);
			EventQueue.push_back(Event);
		}
	}
	/*if(hKeyBoardLayout) {
		UnloadKeyboardLayout(hKeyBoardLayout);
	}*/

	return SendInput(EventQueue.size(), &EventQueue[0], sizeof(INPUT));
}

HEIMY_API void say(char* &text, bool send){
	for(int i = 0; i < (int) strlen(text); i++){
		keyPress(toupper(text[i]), FALSE);
		Sleep(1);
	}
	if(send) {
		keyPress(0x0D, FALSE);
	}
}

HEIMY_API void sayEnter() { keyPress(0x0D, FALSE); }

HEIMY_API void winActivate(HWND hwnd){
	ShowWindow(hwnd, SW_RESTORE);
	SetForegroundWindow(hwnd);
}

HEIMY_API void moveMouse(POINT pos){
	SetCursorPos(pos.x, pos.y);
}

HEIMY_API void click(){
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );
	mouse_event(MOUSEEVENTF_LEFTUP  , 0, 0, 0, 0 );
}

HEIMY_API void moveAndClick(POINT pos){
	moveMouse(pos);
	click();
}

HEIMY_API POINT getMousePos(){
	POINT p;
	GetCursorPos(&p);
	return p;
}

HEIMY_API void drawCross(HDC dc, POINT pos, COLORREF _color, COLORREF _cross, int wide){
	int extend = 1;
	extend = extend + wide;
	SetPixel(dc, pos.x + extend, pos.y + extend, _color);
	SetPixel(dc, pos.x - extend, pos.y - extend, _color);
	SetPixel(dc, pos.x + extend, pos.y - extend, _color);
	SetPixel(dc, pos.x - extend, pos.y + extend, _color);
	SetPixel(dc, pos.x, pos.y - extend, _color);
	SetPixel(dc, pos.x, pos.y - extend + 1, _cross);
	SetPixel(dc, pos.x, pos.y + extend, _color);
	SetPixel(dc, pos.x, pos.y + extend - 1, _cross);
	SetPixel(dc, pos.x - extend , pos.y, _color);
	SetPixel(dc, pos.x - extend + 1 , pos.y, _cross);
	SetPixel(dc, pos.x + extend , pos.y, _color);
	SetPixel(dc, pos.x + extend - 1 , pos.y, _cross);
}

HEIMY_API bool checkForChanges(WindowProps &props) {
	WindowProps currentProps = props;
	getWindowProps(currentProps);
	if(props == currentProps) {
		return false;
	} else {
		props = currentProps;
		return true;
	}
}

HEIMY_API WindowProps getWindowProps(WindowProps &props) {
	RECT WinPos, WinSize;
	GetWindowRect(props.hwnd, &WinPos);
	GetClientRect(props.hwnd, &WinSize);

	props.pos.x = WinPos.top;
	props.pos.y = WinPos.left;
	props.width = WinSize.right;
	props.height = WinSize.bottom;

	return props;
}

HEIMY_API void buildTarget(const WindowProps &props, int xPercent, int yPercent, TARGET &target) {
	target.lol.x = (props.width * xPercent) / 100;
	target.lol.y = (props.height * yPercent) / 100;
	target.screen = target.lol;
	ClientToScreen(props.hwnd, &target.screen);
}

HEIMY_API void translateTarget(const WindowProps &props, int originalX, int originalY, TARGET &target) {
	target.lol.x = (originalX * props.width) / 1024;
	target.lol.y = (originalY * props.height) / 640;
	target.screen = target.lol;
	ClientToScreen(props.hwnd, &target.screen);
}

HEIMY_API void translateTargetRect(const WindowProps &props, int oStartX, int oStartY, int oEndX, int oEndY, TARGETRECT &targetRect) {
	targetRect.startX = (oStartX * props.width) / 1024;
	targetRect.endX = (oEndX * props.height) / 640;
	targetRect.startY = (oStartY * props.width) / 1024;
	targetRect.endY = (oEndY * props.height) / 640;
}

HEIMY_API void testTarget(HDC _dc, HDC lolDC, const TARGET &target) {
	SetPixel(_dc, target.screen.x, target.screen.y, RGB(0, 0, 255));
	SetPixel(_dc, target.screen.x + 1, target.screen.y + 1, RGB(255, 0, 0));
	SetPixel(_dc, target.screen.x - 1, target.screen.y - 1, RGB(0, 255, 0));


	SetPixel(lolDC, target.lol.x, target.lol.y, RGB(255, 0, 0));
	SetPixel(lolDC, target.lol.x + 1, target.lol.y + 1, RGB(0, 255, 0));
	SetPixel(lolDC, target.lol.x - 1, target.lol.y - 1, RGB(0, 0, 255));
}