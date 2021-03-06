#include "stdafx.h"
#include "../../engine2/TGE.h"
//주인공 오브젝트
COORD g_cdPlayerPos;

int main() {
	HANDLE hdout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD fdwOldMode;
	int nW = 8;
	int nH = 4;
	CHAR_INFO *pBuf = (CHAR_INFO*)malloc(nW*nH * sizeof(CHAR_INFO));

	GetConsoleMode(hdin, &fdwOldMode);
	SetConsoleMode(hdin, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	TGE::clearScreenBuffer(0x20, 0x00e0);
	//TGE::loadBinary(TGE::g_chiBuffer, "../../res/map1");

	for (int i = 0; i < nW*nH; i++) {
		pBuf[i].Attributes = 0x00f0;
		pBuf[i].Char.UnicodeChar = 0x20;
	}
	pBuf[3].Attributes = 0x0000;
	pBuf[12].Attributes = 0x0000;
	pBuf[15].Attributes = 0x0000;
	
	//g_cdPlayerPos.X = 15;
	//g_cdPlayerPos.Y = 5;

	bool _bLoop = true;
	static int _nFsm = 0;
	
	while (_bLoop) {
		TGE::updateBuffer(hdout, TGE::g_chiBuffer);
		TGE::setCursor(hdout, 0, 0);
		TGE::setCursor(hdout, 0, 25);
		static char szCmdBuf[128];
		//입력 처리
		switch (_nFsm) {
		case 0://비동기 모드
			static DWORD cNumRead;
			static INPUT_RECORD irInputBuf[128];

			printf("							");
			TGE::setCursor(hdout, 0, 26);
			ReadConsoleInput(hdin, irInputBuf, 128, &cNumRead);

			for (DWORD i = 0; i < cNumRead; i++) {
				if (irInputBuf[i].EventType == KEY_EVENT) {
					if (irInputBuf[i].Event.KeyEvent.bKeyDown) {
						printf("%d", irInputBuf[i].Event.KeyEvent.wVirtualScanCode);

						switch (irInputBuf[i].Event.KeyEvent.wVirtualScanCode) {
						case 28: //커멘드 입력 모드 전환 
							SetConsoleMode(hdin, fdwOldMode);
							_nFsm = 1;

							break;
						default:
							break;
						}
					}
				}
			}

			break;
		case 1://동기 모드
			printf("						]");
			TGE::setCursor(hdout, 0, 25);
			printf("[cmd > ");
			gets_s(szCmdBuf, sizeof(szCmdBuf));

			SetConsoleMode(hdin, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
			_nFsm = 0;

			break;
		default:
			break;
		}
		//커멘드 처리
		{
			char szTokenBuf[8][64];

			if (TGE::doTokenize(szCmdBuf, szTokenBuf) > 0) {

				if (!strcmp(szTokenBuf[0], "quit")) {
					//return 0;
					_bLoop = false;
				}
				else if (!strcmp(szTokenBuf[0], "move")) {
					int _dx = atoi(szTokenBuf[1]);
					int _dy = atoi(szTokenBuf[2]);

					g_cdPlayerPos.X += _dx;
					g_cdPlayerPos.Y += _dy;
				}
			}
			szCmdBuf[0] = 0x00;
		}
		TGE::putSprite(3, 3, 8, 4, TGE::g_chiBuffer, pBuf);

		//오브잭트 처리
		//TGE::setCharacter(TGE::g_chiBuffer, g_cdPlayerPos.X, g_cdPlayerPos.Y, 0x41, 0x000f);
		//렌더(화면 갱신)
		//TGE::updateBuffer(hdout);
	}

	return 0;
}