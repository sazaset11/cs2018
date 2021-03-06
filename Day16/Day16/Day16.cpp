#include "stdafx.h"
#include "../../engine/TGE.h"
#include "tge_sprite.h"
#include "gameobject.h"
//#include "gameobject.h"
int main()
{

	HANDLE hStdout = NULL;
	TGE::startTGE(&hStdout);

	CHAR_INFO *pBakBuffer = TGE::CreateScreenBuffer();
	CHAR_INFO *pMapDataBuffer = TGE::CreateScreenBuffer();

	//리소스 로딩
	TGE::loadBufferBinary(pMapDataBuffer, "../../res/map1");

	tge_sprite::S_SPRITE_OBJECT sprMainInWhite;
	tge_sprite::Init(&sprMainInWhite);
	tge_sprite::load(&sprMainInWhite, "../../res/obj1.spr");

	tge_sprite::S_SPRITE_OBJECT sprAilen;
	tge_sprite::Init(&sprAilen);
	tge_sprite::load(&sprAilen, "../../res/ailen.spr");

	tge_sprite::S_SPRITE_OBJECT sprClaw;
	tge_sprite::Init(&sprClaw);
	tge_sprite::load(&sprClaw, "../../res/claw.spr");

	tge_sprite::S_SPRITE_OBJECT sprFire;
	tge_sprite::Init(&sprFire);
	tge_sprite::load(&sprFire, "../../res/fire.spr");

	//게임 오브젝트 생성
	cs2018prj::playerObject::S_GAMEOBJECT playerObj;
	cs2018prj::ailenObject::S_GAMEOBJECT ailenObj;
	cs2018prj::clawObject::S_GAMEOBJECT clawObj;
	cs2018prj::fireObject::S_GAMEOBJECT fireObj;

	cs2018prj::playerObject::Init(&playerObj, 10, 7, 10, &sprMainInWhite);
	playerObj.m_translation = irr::core::vector2df(-1, -2);
	playerObj.m_pFire = &fireObj;

	cs2018prj::ailenObject::Init(&ailenObj, 35, 10, 7, &sprAilen);
	ailenObj.m_translation = irr::core::vector2df(-1, -2);
	ailenObj.m_pTargetObj = &playerObj;
	ailenObj.m_pAttack = &clawObj;

	cs2018prj::clawObject::Init(&clawObj, 0, 0, 0, &sprClaw);
	clawObj.m_translation = irr::core::vector2df(0, -1);
	clawObj.m_pTargetObj = &playerObj;

	cs2018prj::fireObject::Init(&fireObj, 0, 0, 30, &sprFire);
	//fireObj.m_translation = irr::core::vector2df(0, -1);
	fireObj.m_pTargetObj = &ailenObj;

	bool _bLoop = true;
	static int _nFSM = 0;
	UINT64 _oldTick = TGE::util::GetTimeMs64();

	while (_bLoop)
	{
		UINT64 _curTick = TGE::util::GetTimeMs64();
		UINT64 _deltaTick = _curTick - _oldTick;
		_oldTick = _curTick;

		double _dbDeltaTick = (double)(_deltaTick) / 1000.0;

		//SetConsoleCursorPosition(hStdout,{ 40, 1 });

		static char szCmdBuf[128];
		//입력 처리 
		switch (_nFSM) {
		case 0:
			if (TGE::input::g_KeyTable[VK_RETURN]) {
				_nFSM = 1;
				TGE::input::setEditMode();
			}
			break;
		case 1: //동기  모드
			TGE::setCursor(hStdout, 0, 0);
			TGE::setCursor(hStdout, 0, 26);
			printf_s("                                                 ]");
			TGE::setCursor(hStdout, 0, 26);
			printf_s("[cmd >");
			gets_s(szCmdBuf, sizeof(szCmdBuf));
			_nFSM = 0;
			TGE::input::setNormalMode();
			TGE::input::g_KeyTable[VK_RETURN] = 0;
			_oldTick = TGE::util::GetTimeMs64();
			break;
		}
		//커멘드 처리..
		{
			char szTokenBuf[8][64];
			if (TGE::doTokenize(szCmdBuf, szTokenBuf) > 0) {
				if (!strcmp(szTokenBuf[0], "quit")) {
					_bLoop = false;
				}
				else if (!strcmp(szTokenBuf[0], "claw")) {
					clawObj.m_nFSM = 10;
					/*clawObj.m_bActive = true;*/
					clawObj.m_posx = atoi(szTokenBuf[1]);
					clawObj.m_posy = atoi(szTokenBuf[2]);
				}
				else if (!strcmp(szTokenBuf[0], "fire")) {
					fireObj.m_nFSM = 10;
					fireObj.m_posx = playerObj.m_posx + 2;
					fireObj.m_posy = playerObj.m_posy - 1;
				}
			}
			szCmdBuf[0] = 0x00;
		}

		//ai
		cs2018prj::playerObject::Apply(&playerObj, _dbDeltaTick);
		cs2018prj::ailenObject::Apply(&ailenObj, _dbDeltaTick);
		cs2018prj::clawObject::Apply(&clawObj, _dbDeltaTick);
		cs2018prj::fireObject::Apply(&fireObj, _dbDeltaTick);

		//랜더링 전처리
		//TGE::clearScreenBuffer(pBakBuffer, 0x20, 0x0007);
		//cs2018prj::Render(&testObj, pBakBuffer);
		TGE::copyScreenBuffer(pBakBuffer, pMapDataBuffer);
		cs2018prj::playerObject::Render(&playerObj, pBakBuffer);
		cs2018prj::ailenObject::Render(&ailenObj, pBakBuffer);
		cs2018prj::clawObject::Render(&clawObj, pBakBuffer);
		cs2018prj::fireObject::Render(&fireObj, pBakBuffer);

		//랜더 (화면 갱신)
		TGE::copyScreenBuffer(TGE::g_chiBuffer, pBakBuffer);
		TGE::updateBuffer(hStdout, TGE::g_chiBuffer);
	}

	free(pBakBuffer);
	free(pMapDataBuffer);
	tge_sprite::Release(&sprMainInWhite);
	tge_sprite::Release(&sprAilen);
	tge_sprite::Release(&sprClaw);
	tge_sprite::Release(&sprFire);
	/*free(sprMainInWhite.m_pSpriteBuf);
	free(sprAilen.m_pSpriteBuf);
	free(sprClaw.m_pSpriteBuf);*/
	TGE::endTGE();

	return 0;

}