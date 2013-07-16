#include "Logger.h"
#include "Bot.h"
#include "GUIHelper.h"

const int xGUI_CHAMPION_ID = 101;
const int xGUI_MESSAGE_ID = 102;
const int xGUI_STEPONE_ID = 103;
const int xGUI_STEPTWO_ID = 104;
const int xGUI_STEPTHREE_ID = 105;
const int xGUI_START_ID = 201;
const int xGUI_STOP_ID = 202;

const char g_szClassName[] = "mainWindow";
const LPCWSTR appTitle = L"Lol Auto Pick - SPARTA Release";
int windowWidth = 400, windowHeight = 250;

Bot* bot;
HWND hwndMessage;
HWND hwndChampion;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
		case WM_CREATE:
			Logger::log("WM_CREATE message. Adding some flavors to the mix.");
			GUIHelper::createLabel(hwnd, L"Champion name:", 10, 10, 150, 20, 0);
			hwndChampion = GUIHelper::createEdit(hwnd, 10, 30, 150, 25, xGUI_CHAMPION_ID);
			GUIHelper::createLabel(hwnd, L"Message:", 10, 55, 150, 20, 0);
			hwndMessage = GUIHelper::createEdit(hwnd, 10, 75, 150, 25, xGUI_MESSAGE_ID);
			GUIHelper::createCheckbox(hwnd, L"Accept-game", 10, 100, 150, 25, xGUI_STEPONE_ID);
			CheckDlgButton(hwnd, xGUI_STEPONE_ID, BST_CHECKED);
			GUIHelper::createCheckbox(hwnd, L"Call role", 10, 125, 150, 25, xGUI_STEPTWO_ID);
			CheckDlgButton(hwnd, xGUI_STEPTWO_ID, BST_CHECKED);
			GUIHelper::createCheckbox(hwnd, L"Pick champion", 10, 150, 150, 25, xGUI_STEPTHREE_ID);
			CheckDlgButton(hwnd, xGUI_STEPTHREE_ID, BST_CHECKED);
			GUIHelper::createButton(hwnd, L"Start", 200, 10, 50, 100, xGUI_START_ID); 
			GUIHelper::createButton(hwnd, L"Stop", 310, 10, 50, 100, xGUI_STOP_ID);
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case xGUI_STEPONE_ID:
					if(IsDlgButtonChecked(hwnd, xGUI_STEPONE_ID) == BST_CHECKED) {
						bot->doStepOne = false;
						CheckDlgButton(hwnd, xGUI_STEPONE_ID, BST_UNCHECKED);
					} else {
						bot->doStepOne = true;
						CheckDlgButton(hwnd, xGUI_STEPONE_ID, BST_CHECKED);
					}
					break;
				case xGUI_STEPTWO_ID:
					if(IsDlgButtonChecked(hwnd, xGUI_STEPTWO_ID) == BST_CHECKED) {
						bot->doStepTwo = false;
						CheckDlgButton(hwnd, xGUI_STEPTWO_ID, BST_UNCHECKED);
					} else {
						bot->doStepTwo = true;
						CheckDlgButton(hwnd, xGUI_STEPTWO_ID, BST_CHECKED);
					}
					break;
				case xGUI_STEPTHREE_ID:
					if(IsDlgButtonChecked(hwnd, xGUI_STEPTHREE_ID) == BST_CHECKED) {
						bot->doStepThree = false;
						CheckDlgButton(hwnd, xGUI_STEPTHREE_ID, BST_UNCHECKED);
					} else {
						bot->doStepThree = true;
						CheckDlgButton(hwnd, xGUI_STEPTHREE_ID, BST_CHECKED);
					}
					break;
				case xGUI_START_ID:
					bot->setChampion(GUIHelper::getText(hwndChampion));
					bot->setMessage(GUIHelper::getText(hwndMessage));
					bot->startThread();
					break;
				case xGUI_STOP_ID:
					bot->requestStop();
					break;
			}
			break;
		case WM_KEYUP:
			switch(wParam) {
				case VK_TAB:
					MessageBox(NULL, L"ok", L"ok", NULL);
				break;
			}
			break;
		case WM_CLOSE:
			Logger::log("WM_CLOSE message. Hide in the shadow!");
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			Logger::log("WM_DESTROY message. Goodbye.");
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	bot = new Bot();

	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = (LPCWSTR) g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		(LPCWSTR) g_szClassName,
		appTitle,
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight,
		NULL, NULL, hInstance, NULL
	);

	if(hwnd == NULL) {
		MessageBox(NULL, L"Window Creation Failed!", L"Error!",
		MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}


	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);


	while(GetMessage(&Msg, NULL, 0, 0) > 0)	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;

}