#include <windows.h>
#include <string>;
#include <iostream>

bool gameLoop = true;

struct ScreenInfo {
	void* bufferMemory;
	int screenWidth;
	int screenHeight;
	BITMAPINFO bitmapInfo;

	int clsBackgroundColor = 0x0d023d;
};
ScreenInfo si;

struct InputInfo {
	bool pressed;
};

enum {
	btnLeft,
	btnRight,
	btnEsc
};
const int buttonCount = 3;

struct Input {
	InputInfo buttons[buttonCount];
};

#define inputKeyA 0x41
#define inputKeyD 0x44
#define inputKeyEsc 0x1B

#include "Struct.cpp"
#include "Render.cpp";
#include "Simulate.cpp";
#include "Import.cpp"

LRESULT CALLBACK callBack(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	
	LRESULT result = 0;

	switch (uMsg) {

		case WM_DESTROY:
		case WM_CLOSE: {
			gameLoop = false;
		} break;
	
		case WM_SIZE: {
			
			RECT rect;
			GetClientRect(hwnd, &rect);
			si.screenWidth = rect.right - rect.left;
			si.screenHeight = rect.bottom - rect.top;

			int bufferSize = si.screenWidth * si.screenHeight * sizeof(unsigned int);
			if (si.bufferMemory) {
				VirtualFree(si.bufferMemory, 0, MEM_RELEASE);
			}
			si.bufferMemory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE,
				PAGE_READWRITE);

			BITMAPINFOHEADER* bmih = &si.bitmapInfo.bmiHeader;
			bmih->biSize = sizeof(si.bitmapInfo.bmiHeader);
			bmih->biWidth = si.screenWidth;
			bmih->biHeight = si.screenHeight;
			bmih->biPlanes = 1;
			bmih->biBitCount = 32;
			bmih->biCompression = BI_RGB;

		} break;

		default: {
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	
	ShowCursor(false);

	LevelInfo li;
	importLevelInfo(&li, bgi.level1);
	handleSpacing(&li);
	
	WNDCLASS wndClass = {CS_HREDRAW | CS_VREDRAW, callBack};
	wndClass.lpszClassName = L"GameWindow";
	RegisterClass(&wndClass);
	HWND window = CreateWindow(
		wndClass.lpszClassName, L"Breakout",
		WS_VISIBLE | WS_BORDER | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		1920, 1080, 0, 0, hInstance, 0
	);

	HDC hdc = GetDC(window);

	Input input = {};

	float frameTime = 1/60;
	float frameFrequency;
	LARGE_INTEGER CPUPower;
	QueryPerformanceFrequency(&CPUPower);
	frameFrequency = (float)CPUPower.QuadPart;

	LARGE_INTEGER frameStartPoint;
	LARGE_INTEGER frameEndPoint;
	QueryPerformanceCounter(&frameStartPoint);

	while (gameLoop) {

		MSG message;

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {

			switch (message.message) {

				case WM_KEYUP:
				case WM_KEYDOWN: {
					
					unsigned int keyIdent = (unsigned int) message.wParam;
					bool pressed = (message.lParam & (1 << 31)) == 0;

					switch (keyIdent) {
						
						case inputKeyA: {
							input.buttons[btnLeft].pressed = pressed;
						}
						case inputKeyD: {
							input.buttons[btnRight].pressed = pressed;
						} break;
						case inputKeyEsc: {
							return 0;
						} break;
					}

				} break;

				default: {
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}
		}

		rClearScreen();
		simulate(&input, frameTime, &li);

		if (pi.currentLifes <= -1) {
			playSoundSyn(pi.loseGameSound);
			return 0;
		}
		if (li.beaten) {
			bgi.level++;
			if (bgi.level == 2) {
				importLevelInfo(&li, bgi.level2);
				handleSpacing(&li);
				giveBackLifeIncreaseDiff();
				resetBallNPlayer();
			}
			else if (bgi.level == 3) {
				importLevelInfo(&li, bgi.level3);
				handleSpacing(&li);
				giveBackLifeIncreaseDiff();
				resetBallNPlayer();
			}
			else {
				playSoundSyn(pi.winGameSound);
				return 0;
			}
		}

		StretchDIBits(hdc, 0, 0, si.screenWidth, si.screenHeight,
			0, 0, si.screenWidth, si.screenHeight, si.bufferMemory,
			&si.bitmapInfo, DIB_RGB_COLORS, SRCCOPY
		);

		QueryPerformanceCounter(&frameEndPoint);
		frameTime = (float) (frameEndPoint.QuadPart - frameStartPoint.QuadPart) / frameFrequency;
		frameStartPoint = frameEndPoint;
	}
}