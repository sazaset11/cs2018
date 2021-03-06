// Day12.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
#include "stdafx.h"
#include "../../engine/TGE.h"
#include "maptool.h"
using namespace tge;

enum { MODE_BRUSH_DRAW = 0, MODE_CMD_INPUT = 1, MODE_BRUSH_MOVE = 2 };

int main()
{
	HANDLE hdout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD fdwSaveOldMode;
	static char szCmdBuf[32];
	int FSM = MODE_BRUSH_DRAW;
	static DWORD cNumRead;
	static INPUT_RECORD irInputBuf[128];

	GetConsoleMode(hdin, &fdwSaveOldMode);
	SetConsoleMode(hdin, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	initMapTool();
	clearScreenBuffer(0x2e, 0x0007);

	while (true) {
		updateBuffer(hdout);
		setCursor(hdout, 0, 0);
		setCursor(hdout, 0, 25);
		switch (FSM)
		{
		case MODE_BRUSH_DRAW:
			//비동기 키입력 처리
		{
			printf("							");
			setCursor(hdout, 0, 26);
			ReadConsoleInput(hdin, irInputBuf, 128, &cNumRead);

			for (DWORD i = 0; i < cNumRead; i++) {
				if (irInputBuf[i].EventType == KEY_EVENT) {
					if (irInputBuf[i].Event.KeyEvent.bKeyDown) {
						printf("%d", irInputBuf[i].Event.KeyEvent.wVirtualKeyCode);

						switch (irInputBuf[i].Event.KeyEvent.wVirtualKeyCode)
						{
						case 13:
							SetConsoleMode(hdin, fdwSaveOldMode);
							FSM = MODE_CMD_INPUT;
							break;
						case 37:
							sprintf_s(szCmdBuf, 32, "set %d %d %4x %4x", --g_cdCurrentCursorPos.X, g_cdCurrentCursorPos.Y, g_wcCurrentBrushCode, g_wCurrentBrushAttr);
							//g_cdCurrentCursorPos.X -= 1;
							break;
						case 38:
							sprintf_s(szCmdBuf, 32, "set %d %d %x %x", g_cdCurrentCursorPos.X, --g_cdCurrentCursorPos.Y, g_wcCurrentBrushCode, g_wCurrentBrushAttr);
							//g_cdCurrentCursorPos.Y -= 1;
							break;
						case 39:
							sprintf_s(szCmdBuf, 32, "set %d %d %x %x", ++g_cdCurrentCursorPos.X, g_cdCurrentCursorPos.Y, g_wcCurrentBrushCode, g_wCurrentBrushAttr);
							//g_cdCurrentCursorPos.X += 1;
							break;
						case 40:
							sprintf_s(szCmdBuf, 32, "set %d %d %x %x", g_cdCurrentCursorPos.X, ++g_cdCurrentCursorPos.Y, g_wcCurrentBrushCode, g_wCurrentBrushAttr);
							//g_cdCurrentCursorPos.Y += 1;
							break;
						case 77:
							SetConsoleMode(hdin, fdwSaveOldMode);
							FSM = MODE_BRUSH_MOVE;
							break;
						default:
							break;
						}
					}
				}
			}

		}
		break;
		case MODE_CMD_INPUT:
			//스크립트 입력처리
			printf("						]");
			setCursor(hdout, 0, 25);
			printf("[cmd > ");
			gets_s(szCmdBuf, sizeof(szCmdBuf));
			SetConsoleMode(hdin, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
			FSM = MODE_BRUSH_DRAW;

			break;
		case MODE_BRUSH_MOVE:
			printf("							");
			setCursor(hdout, 0, 26);
			ReadConsoleInput(hdin, irInputBuf, 128, &cNumRead);

			for (DWORD i = 0; i < cNumRead; i++) {
				if (irInputBuf[i].EventType == KEY_EVENT) {
					if (irInputBuf[i].Event.KeyEvent.bKeyDown) {
						printf("%d", irInputBuf[i].Event.KeyEvent.wVirtualKeyCode);

						switch (irInputBuf[i].Event.KeyEvent.wVirtualKeyCode)
						{
						case 13:
							SetConsoleMode(hdin, fdwSaveOldMode);
							FSM = MODE_CMD_INPUT;
							break;
						case 37:
							sprintf_s(szCmdBuf, 32, "cursor %d %d %4x %4x", --g_cdCurrentCursorPos.X, g_cdCurrentCursorPos.Y, g_wcCurrentBrushCode, g_wCurrentBrushAttr);
							//g_cdCurrentCursorPos.X -= 1;
							break;
						case 38:
							sprintf_s(szCmdBuf, 32, "cursor %d %d %4x %4x", g_cdCurrentCursorPos.X, --g_cdCurrentCursorPos.Y, g_wcCurrentBrushCode, g_wCurrentBrushAttr);
							//g_cdCurrentCursorPos.Y -= 1;
							break;
						case 39:
							sprintf_s(szCmdBuf, 32, "cursor %d %d %4x %4x", ++g_cdCurrentCursorPos.X, g_cdCurrentCursorPos.Y, g_wcCurrentBrushCode, g_wCurrentBrushAttr);
							//g_cdCurrentCursorPos.X += 1;
							break;
						case 40:
							sprintf_s(szCmdBuf, 32, "cursor %d %d %4x %4x", g_cdCurrentCursorPos.X, ++g_cdCurrentCursorPos.Y, g_wcCurrentBrushCode, g_wCurrentBrushAttr);
							//g_cdCurrentCursorPos.Y += 1;
							break;
						case 68:
							_oldPos.X = -1;
							FSM = MODE_BRUSH_DRAW;
							break;
						default:
							break;
						}
					}
				}
			}
		default:
			break;
		}

		if (!parseCmd(szCmdBuf)) return 0;
		//setCharacter(g_cdCurrentCursorPos.X, g_cdCurrentCursorPos.Y, 0x20, 0x00e0);
	}

	return 0;
}

