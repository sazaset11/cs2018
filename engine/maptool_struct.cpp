#include "stdafx.h"
#include "TGE.h"
#include "maptool_struct.h"
#include "tge_sprite.h"

using namespace TGE;

tge_sprite::S_SPRITE_OBJECT g_WorkSprObject;

void initMapTool(S_TGE_MAPTOOL *pObj) {
	pObj->m_cdCurrentCursorPos.X = 40;
	pObj->m_cdCurrentCursorPos.Y = 12;
	pObj->m_wcCurrentBrushCode = 0x20;
	pObj->m_wCurrentBrushAttr = 0x00e0;
	pObj->m_wCurrentCursorSizeX = 1;
	pObj->m_wCurrentCursorSizeY = 1;
	tge_sprite::Init(&g_WorkSprObject);

}

int parseCmd(S_TGE_MAPTOOL *pObj, char *pzBuf) {
	static char szTokenBuf[8][MAX_TOKEN_SIZE];
	//토큰처리
	doTokenize(pzBuf, szTokenBuf);

	if (!strcmp(szTokenBuf[0], "quit"))	return 0;
	else if (!strcmp(szTokenBuf[0], "set")) {
		int _xpos = atoi(szTokenBuf[1]);
		int _ypos = atoi(szTokenBuf[2]);
		WCHAR _code = (WCHAR)strtol(szTokenBuf[3], NULL, 16);
		WORD _attr = (WORD)strtol(szTokenBuf[4], NULL, 16);
		setCharacter(g_chiBuffer, _xpos, _ypos, _code, _attr);

		return 1;
	}
	else if (!strcmp(szTokenBuf[0], "box")) {
		int _xpos = atoi(szTokenBuf[1]);
		int _ypos = atoi(szTokenBuf[2]);
		int _width = atoi(szTokenBuf[3]);
		int _height = atoi(szTokenBuf[4]);
		WCHAR _code = (WCHAR)strtol(szTokenBuf[5], NULL, 16);
		WORD _attr = (WORD)strtol(szTokenBuf[6], NULL, 16);
		drawBox(g_chiBuffer, _xpos, _ypos, _width, _height, _code, _attr);

		return 1;
	}
	else if (!strcmp(szTokenBuf[0], "move")) {
		pObj->m_cdCurrentCursorPos.X = atoi(szTokenBuf[1]);
		pObj->m_cdCurrentCursorPos.Y = atoi(szTokenBuf[2]);

		return 1;
	}
	else if (!strcmp(szTokenBuf[0], "cursor") || !strcmp(szTokenBuf[0], "cursorend")) {
		static COORD _oldPos = { -1,-1 };
		static WCHAR _oldCode = -1;
		static WORD _oldAttr = -1;
		if (!strcmp(szTokenBuf[0], "cursorend")) {
			_oldPos.X = -1;
		}
		if (_oldPos.X >= 0) {
			setCharacter(g_chiBuffer, _oldPos.X, _oldPos.Y, _oldCode, _oldAttr);
		}
		//이전값 대피
		_oldPos.X = pObj->m_cdCurrentCursorPos.X;
		_oldPos.Y = pObj->m_cdCurrentCursorPos.Y;
		CHAR_INFO *oldInfo = getCharacter(g_chiBuffer, _oldPos.X, _oldPos.Y);
		_oldCode = oldInfo->Char.UnicodeChar;
		_oldAttr = oldInfo->Attributes;

		int _xpos = atoi(szTokenBuf[1]);
		int _ypos = atoi(szTokenBuf[2]);
		WCHAR _code = (WCHAR)strtol(szTokenBuf[3], NULL, 16);
		WORD _attr = (WORD)strtol(szTokenBuf[4], NULL, 16);
		setCharacter(g_chiBuffer, _xpos, _ypos, _code, _attr);

		return 1;
	}
	else if (!strcmp(szTokenBuf[0], "char")) {
		pObj->m_wcCurrentBrushCode = (WCHAR)strtol(szTokenBuf[1], NULL, 16);
		pObj->m_wCurrentBrushAttr = (WORD)strtol(szTokenBuf[2], NULL, 16);

		return 1;
	}
	else if (!strcmp(szTokenBuf[0], "clear")) {
		clearScreenBuffer(TGE::g_chiBuffer, 0x2e, 0x0007);

		return 1;
	}
	else if (!strcmp(szTokenBuf[0], "save")) {
		return saveBinary(g_chiBuffer, szTokenBuf[1]);
	}
	else if (!strcmp(szTokenBuf[0], "load")) {
		return loadBinary(g_chiBuffer, szTokenBuf[1]);
	}
	else if (!strcmp(szTokenBuf[0], "getsprite")) {
		int _xpos = atoi(szTokenBuf[1]);
		int _ypos = atoi(szTokenBuf[2]);
		int _width = atoi(szTokenBuf[3]);
		int _height = atoi(szTokenBuf[4]);

		tge_sprite::get(&g_WorkSprObject, _xpos, _ypos, _width, _height);

		return 1;
	}
	else if (!strcmp(szTokenBuf[0], "putsprite")) {
		int _xpos = atoi(szTokenBuf[1]);
		int _ypos = atoi(szTokenBuf[2]);
		if (g_WorkSprObject.m_pSpriteBuf != NULL) {
			tge_sprite::put(&g_WorkSprObject, _xpos, _ypos);
		}

		return 1;
	}
	else if (!strcmp(szTokenBuf[0], "savesprite")) {
		return tge_sprite::save(&g_WorkSprObject, szTokenBuf[1]);
	}
	else if (!strcmp(szTokenBuf[0], "loadsprite")) {
		return tge_sprite::load(&g_WorkSprObject, szTokenBuf[1]);
	}
	else return 1;
}