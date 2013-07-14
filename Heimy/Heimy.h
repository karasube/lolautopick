#include <Windows.h>
#include <string>
#include "Structs.h"
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the HEIMY_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// HEIMY_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef HEIMY_EXPORTS
#define HEIMY_API __declspec(dllexport)
#else
#define HEIMY_API __declspec(dllimport)
#endif


HEIMY_API bool searchForPixelColor(HDC dc, TARGETRECT rect, int xInc, int yInc, COLORREF _color, COLORREF _color2, COLORREF _color3);
HEIMY_API bool checkPixelColor(HDC dc, COLORREF color, POINT pos);
HEIMY_API void keyPress(int vk, BOOL bExtended);
HEIMY_API void say(char* text, bool send);
HEIMY_API void winActivate(HWND hwnd);
HEIMY_API void moveMouse(POINT pos);
HEIMY_API void click();
HEIMY_API void moveAndClick(POINT pos);
HEIMY_API POINT getMousePos();
HEIMY_API void drawCross(HDC dc, POINT pos, COLORREF _color, COLORREF _cross, int wide);
HEIMY_API bool checkForChanges(WindowProps &props);
HEIMY_API WindowProps getWindowProps(WindowProps &props);
HEIMY_API void buildTarget(const WindowProps &props, int xPercent, int yPercent, TARGET &target);
HEIMY_API void translateTarget(const WindowProps &props, int originalX, int originalY, TARGET &target);
HEIMY_API void translateTargetRect(const WindowProps &props, int oStartX, int oStartY, int oEndX, int oEndY, TARGETRECT &targetRect);
HEIMY_API void testTarget(HDC _dc, HDC lolDC, const TARGET &target);