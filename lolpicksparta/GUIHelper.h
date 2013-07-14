#pragma once
#ifndef _GUIHELPER_H_
#define _GUIHELPER_H_
#include <Windows.h>

class GUIHelper
{
public:

	GUIHelper(void)
	{
	}

	~GUIHelper(void)
	{
	}

	static char* GUIHelper::getText(HWND hwnd) {
		int len = SendMessage(hwnd, WM_GETTEXTLENGTH, 0, 0);
		char* buffer = new char[len + 1];
		GetWindowTextA(hwnd, buffer, len + 1);
		return buffer;
	}

	static HWND GUIHelper::createButton(HWND hwnd, LPCWSTR title, int x, int y, int width, int height, int identifier) {
		return CreateWindow(L"button", title, WS_CHILD | WS_VISIBLE, x, y, width, height, hwnd, (HMENU) identifier, GetModuleHandle(NULL), NULL);
	}
	static HWND GUIHelper::createLabel(HWND hwnd, LPCWSTR title, int x, int y, int width, int height, int identifier){
		return CreateWindow(L"static", title, WS_CHILD | WS_VISIBLE, x, y, width, height, hwnd, (HMENU) identifier, GetModuleHandle(NULL), NULL);
	}
	static HWND GUIHelper::createEdit(HWND hwnd, int x, int y, int width, int height, int identifier) {
		return CreateWindowEx(NULL, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, x, y, width, height, hwnd, (HMENU) identifier, GetModuleHandle(NULL), NULL);
	}
	static HWND GUIHelper::createList(HWND hwnd, int x, int y, int width, int height, int identifier) {
		return CreateWindow(L"listbox", L"", WS_CHILD | WS_VISIBLE | LBS_STANDARD, x, y, width, height, hwnd, (HMENU) identifier, GetModuleHandle(NULL), NULL);
	}
	static HWND GUIHelper::createCheckbox(HWND hwnd, LPCWSTR title, int x, int y, int width, int height, int identifier) {
		return CreateWindow(L"button", title, WS_VISIBLE | WS_CHILD | BS_CHECKBOX, x, y, width, height, hwnd, (HMENU) identifier, GetModuleHandle(NULL), NULL);
	}
};

#endif