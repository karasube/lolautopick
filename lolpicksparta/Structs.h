#pragma once
#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <Windows.h>

struct WindowProps {
	HWND hwnd;
	HDC dc;
	POINT pos;
	int width;
	int height;

	bool operator== (const WindowProps& winProps) {
		if(pos.x == winProps.pos.x && pos.y == winProps.pos.y &&
				width == winProps.width && height == winProps.height) {
			return true;
		} else {
			return false;
		}
	}

	bool operator! () {
		if(hwnd == NULL || dc == NULL || width < 0 || height < 0) {
			return false;	
		} else {
			return true;
		}
	}
};

struct TARGET {
	POINT screen;
	POINT lol;
};

struct TARGETRECT {
	int startX;
	int endX;
	int startY;
	int endY;
};

#endif