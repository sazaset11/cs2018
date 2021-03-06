#include "stdafx.h"
#include "../../engine/TGE.h"
#include "../../engine/TGE_sprite.h"
#include "../../engine/PlayerObject.h"

//주인공 오브젝트
//COORD g_cdPlayerPos;
struct S_GAME_OBJECT {
	COORD m_cdPosition;
	tge_sprite::S_SPRITE_OBJECT m_pSpr;
	int m_nFsm;
};

int Apply(S_GAME_OBJECT *pObj) {
	static int _x = 0, _y = 0;
	static int count = 1;

	Sleep(20);

	switch (pObj->m_nFsm)
	{
	case 0:
		if (pObj->m_cdPosition.X < 77 - _x) {
			pObj->m_cdPosition.X += 1;
		}
		else {
			if (pObj->m_cdPosition.Y >= 22 - _y) {
				pObj->m_nFsm = 6;
			}
			else {
				pObj->m_nFsm = 1;
			}
		}

		break;
	case 1:
		if (pObj->m_cdPosition.Y < 22 - _y) {
			pObj->m_cdPosition.Y += 1;
		}
		else {
			pObj->m_nFsm = 2;
		}

		break;
	case 2:
		if (pObj->m_cdPosition.X > _x) {
			pObj->m_cdPosition.X -= 1;
		}
		else {
			if (pObj->m_cdPosition.Y <= _y) {
				pObj->m_nFsm = 4;
			}
			else {
				pObj->m_nFsm = 3;
				_y += 1;
				if (_y > 13) _y = 13;
			}
		}

		break;
	case 3:
		if (pObj->m_cdPosition.Y > _y) {
			pObj->m_cdPosition.Y -= 1;
		}
		else {
			_x += 3;
			if (_x > 40) _x = 40;
			count++;
			pObj->m_nFsm = 0;
		}
		break;
	case 4:
		if (pObj->m_cdPosition.X < 77 - _x) {
			pObj->m_cdPosition.X += 1;
		}
		else {
			pObj->m_nFsm = 7;
		}

		break;
	case 5:
		if (pObj->m_cdPosition.Y < 22 - _y) {
			pObj->m_cdPosition.Y += 1;
		}
		else {
			pObj->m_nFsm = 4;
		}

		break;
	case 6:
		if (pObj->m_cdPosition.X > _x) {
			pObj->m_cdPosition.X -= 1;
		}
		else {
			if (pObj->m_cdPosition.X == 0) {
				pObj->m_nFsm = 0;
			}
			else {
				_y -= 1;
				if (_y < 0) _y = 0;
				pObj->m_nFsm = 5;
			}
		}

		break;
	case 7:
		if (pObj->m_cdPosition.Y > _y) {
			pObj->m_cdPosition.Y -= 1;
		}
		else {
			_x -= 3;
			if (_x < 0) _x = 0;
			count--;
			pObj->m_nFsm = 6;
		}
		break;

	default:
		break;
	}
	return count;
}


int main() {
	HANDLE hdout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD fdwOldMode;
	CHAR_INFO *pBackBuff = TGE::CreateScreenBuffer();
	CHAR_INFO *pBackDataBuff = TGE::CreateScreenBuffer();
	S_GAME_OBJECT crossObj = { 0 };
	playerobject::S_PlayerObject g_player;
	playerobject::Init(&g_player, 15, 5);

	GetConsoleMode(hdin, &fdwOldMode);
	SetConsoleMode(hdin, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	//TGE::loadBinary(pBackDataBuff, "../../res/map1");
	TGE::clearScreenBuffer(pBackDataBuff, 0x20, 0x0090);
	tge_sprite::load(&crossObj.m_pSpr, "../../res/sprite2");


	bool _bLoop = true;
	static int _nFsm = 0;

	while (_bLoop) {
		TGE::updateBuffer(hdout, TGE::g_chiBuffer);
		TGE::setCursor(hdout, 0, 0);
		TGE::setCursor(hdout, 0, 25);
		static char szCmdBuf[128];
		//입력 처리
		//switch (_nFsm) {
		//case 0://비동기 모드
		//	static DWORD cNumRead;
		//	static INPUT_RECORD irInputBuf[128];

		//	printf("							");
		//	TGE::setCursor(hdout, 0, 26);
		//	ReadConsoleInput(hdin, irInputBuf, 128, &cNumRead);

		//	for (DWORD i = 0; i < cNumRead; i++) {
		//		if (irInputBuf[i].EventType == KEY_EVENT) {
		//			if (irInputBuf[i].Event.KeyEvent.bKeyDown) {
		//				printf("%d", irInputBuf[i].Event.KeyEvent.wVirtualScanCode);

		//				switch (irInputBuf[i].Event.KeyEvent.wVirtualScanCode) {
		//				case 28: //커멘드 입력 모드 전환 
		//					SetConsoleMode(hdin, fdwOldMode);
		//					_nFsm = 1;

		//					break;
		//				default:
		//					break;
		//				}
		//			}
		//		}
		//	}

		//	break;
		//case 1://동기 모드
		//	printf("						]");
		//	TGE::setCursor(hdout, 0, 25);
		//	printf("[cmd > ");
		//	gets_s(szCmdBuf, sizeof(szCmdBuf));

		//	SetConsoleMode(hdin, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
		//	_nFsm = 0;

		//	break;
		//default:
		//	break;
		//}
		////커멘드 처리
		//{
		//	char szTokenBuf[8][MAX_TOKEN_SIZE];

		//	if (TGE::doTokenize(szCmdBuf, szTokenBuf) > 0) {

		//		if (!strcmp(szTokenBuf[0], "quit")) {
		//			_bLoop = false;
		//		}
		//		else if (!strcmp(szTokenBuf[0], "move")) {
		//			int _dx = atoi(szTokenBuf[1]);
		//			int _dy = atoi(szTokenBuf[2]);

		//			g_player.m_Position.X += _dx;
		//			g_player.m_Position.Y += _dy;
		//		}
		//	}
		//	szCmdBuf[0] = 0x00;
		//}

		printf("%d\n", Apply(&crossObj));
		//TGE::clearScreenBuffer(0x20, 0x0090);
		memcpy_s(pBackBuff, SCREEN_BUF_SIZE * sizeof(CHAR_INFO), pBackDataBuff, SCREEN_BUF_SIZE * sizeof(CHAR_INFO));
		//오브잭트 처리
		TGE::setCharacter(pBackBuff, g_player.m_Position.X, g_player.m_Position.Y, 0x41, 0x000f);
		tge_sprite::put(&(crossObj.m_pSpr), crossObj.m_cdPosition.X, crossObj.m_cdPosition.Y, pBackBuff);
		//렌더(화면 갱신)
		memcpy_s(TGE::g_chiBuffer, SCREEN_BUF_SIZE * sizeof(CHAR_INFO), pBackBuff, SCREEN_BUF_SIZE * sizeof(CHAR_INFO));
	}

	return 0;
}
