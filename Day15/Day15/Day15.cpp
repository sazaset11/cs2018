// Day15.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"

CRITICAL_SECTION g_cs;

DWORD WINAPI MyThreadFuction(LPVOID lpParam) {
	HANDLE hdout = GetStdHandle(STD_OUTPUT_HANDLE);
	double _tick = 0;

	while (1)
	{
		_tick += 0.001;

		EnterCriticalSection(&g_cs);

		SetConsoleCursorPosition(hdout, { 35, 1 });
		printf("%5.3lf", _tick);

		LeaveCriticalSection(&g_cs);
		Sleep(1);
	}

	return 0;
}


int main()
{
	HANDLE hdin = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hdout = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD _oldInputMode;
	DWORD _NumRead;
	INPUT_RECORD irInBuf[128];
	DWORD dwThreadid;
	HANDLE hThreed = CreateThread(NULL, 0, MyThreadFuction, NULL, 0, &dwThreadid);
	bool _loop = true;
	double _tick = 0;

	GetConsoleMode(hdin, &_oldInputMode);
	SetConsoleMode(hdin, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	InitializeCriticalSection(&g_cs);

	while (_loop) {
		ReadConsoleInput(hdin, irInBuf, 128, &_NumRead);

		for (DWORD i = 0; i < _NumRead; i++) {
			if (irInBuf[i].EventType == KEY_EVENT) {
				if (irInBuf[i].Event.KeyEvent.bKeyDown) {
					EnterCriticalSection(&g_cs);
/*
					SetConsoleCursorPosition(hdout, { 0, 10 });
					printf("								");*/
					SetConsoleCursorPosition(hdout, { 0, 10 });
					printf("%5d", irInBuf[i].Event.KeyEvent.wVirtualKeyCode);

					LeaveCriticalSection(&g_cs);
				}
			}
		}

		//_tick += 0.001;

		//SetConsoleCursorPosition(hdout, { 35, 1 });
		//printf("%5lf", _tick);
	}

	SetConsoleMode(hdin, _oldInputMode);

    return 0;
}

