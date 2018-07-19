//TGE skeleton 
#include "stdafx.h"
#include "tge.h"
#include "tge_sprite.h"
#include "gameobject.h"

void _loadSprite(tge_sprite::S_SPRITE_OBJECT *spr, const char *szFileName)
{
	tge_sprite::Init(spr);
	tge_sprite::load(spr, szFileName);
}

int main()
{

	HANDLE hStdout = NULL;
	TGE::startTGE(&hStdout);

	//리소스 로딩
	CHAR_INFO *pMapDataBuffer = TGE::CreateScreenBuffer();
	CHAR_INFO *pBakBuffer = TGE::CreateScreenBuffer();
	TGE::loadBufferBinary(pMapDataBuffer, "../../res/1.map");

	tge_sprite::S_SPRITE_OBJECT sprMiw, sprAilen, sprFire, sprkey, sprdoor1, sprdoor2/*, sprClaw*/;
	_loadSprite(&sprMiw, "../../res/obj1.spr");
	_loadSprite(&sprAilen, "../../res/ailen.spr");
	//_loadSprite(&sprClaw, "../../res/claw.spr");
	_loadSprite(&sprFire, "../../res/firey.spr");
	_loadSprite(&sprkey, "../../res/keyg.spr");
	_loadSprite(&sprdoor1, "../../res/doorg.spr");
	_loadSprite(&sprdoor2, "../../res/doorb.spr");

	cs2018prj::objMng::S_OBJECT_MNG objMng = { 0 };
	cs2018prj::S_GAMEOBJECT *objPlayer, *objAilen, *objFire, *objKey, *objDoor1, *objDoor2/*, *objClaw*/;

	objPlayer = (cs2018prj::S_GAMEOBJECT *)malloc(sizeof(cs2018prj::S_GAMEOBJECT));
	cs2018prj::playerObject::Init(objPlayer, irr::core::vector2df(2, 2), 10, &sprMiw);
	cs2018prj::playerObject::Activate(objPlayer);
	//objPlayer->m_bActive = true;
	objPlayer->m_translation = irr::core::vector2df(-1, 0);

	objFire = (cs2018prj::S_GAMEOBJECT *)malloc(sizeof(cs2018prj::S_GAMEOBJECT));
	cs2018prj::attackObject::fire::Init(objFire, irr::core::vector2df(0, 0), 20, &sprFire);
	objFire->m_translation = irr::core::vector2df(0, 2);

	objAilen = (cs2018prj::S_GAMEOBJECT *)malloc(sizeof(cs2018prj::S_GAMEOBJECT));
	cs2018prj::ailenObject::Init(objAilen, irr::core::vector2df(50, 18), 7, &sprAilen);
	objAilen->m_translation = irr::core::vector2df(-1, 0);
	cs2018prj::ailenObject::Activate(objAilen);
	
	//objAilen->m_nFSM = 10;
	objKey = (cs2018prj::S_GAMEOBJECT*)malloc(sizeof(cs2018prj::S_GAMEOBJECT));
	cs2018prj::itemObject::key::Init(objKey, irr::core::vector2df(0, 0), 0, &sprkey);
	objKey->m_translation = irr::core::vector2df(0, 0);

	objDoor1 = (cs2018prj::S_GAMEOBJECT*)malloc(sizeof(cs2018prj::S_GAMEOBJECT));
	cs2018prj::itemObject::door::Init(objDoor1, irr::core::vector2df(99, 13), 0, &sprdoor1);
	objDoor1->m_translation = irr::core::vector2df(0, 0);	
	cs2018prj::itemObject::door::Activate(objDoor1);
	//objDoor1->m_bActive = true;
	objDoor2 = (cs2018prj::S_GAMEOBJECT*)malloc(sizeof(cs2018prj::S_GAMEOBJECT));
	cs2018prj::itemObject::door::Init(objDoor2, irr::core::vector2df(50, 24), 0, &sprdoor2);
	objDoor2->m_translation = irr::core::vector2df(0, 0);
	cs2018prj::itemObject::door::Activate(objDoor2);

	objPlayer->m_pWepon = objFire;
	objAilen->m_pTargetObj = objPlayer;
	objAilen->m_pDropObj = objKey;
	objFire->m_pTargetObj = objAilen;
	objKey->m_pDropObj = objAilen;
	objKey->m_pTargetObj = objPlayer;

	cs2018prj::objMng::add(&objMng, objFire);
	cs2018prj::objMng::add(&objMng, objAilen);
	cs2018prj::objMng::add(&objMng, objKey);
	cs2018prj::objMng::add(&objMng, objDoor1);
	cs2018prj::objMng::add(&objMng, objDoor2);
	cs2018prj::objMng::add(&objMng, objPlayer);


	bool _bLoop = true;
	static int _nFSM = 0;
	UINT64 _oldTick = TGE::util::GetTimeMs64();
	TGE::setCursor(hStdout, 0, 25);

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
				else if (!strcmp(szTokenBuf[0], "fire")) {
					cs2018prj::S_GAMEOBJECT *pFireObj;
					pFireObj = (cs2018prj::S_GAMEOBJECT *)malloc(sizeof(cs2018prj::S_GAMEOBJECT));
					cs2018prj::attackObject::fire::Init(pFireObj, irr::core::vector2df(3, 15), 7, &sprFire);
					pFireObj->m_translation = irr::core::vector2df(-1, -1);
					//pFireObj->m_nFSM = 10; //활성화 시킨다.
					cs2018prj::attackObject::fire::Activate(pFireObj);
					cs2018prj::objMng::add(&objMng, pFireObj);
				}
			}
			szCmdBuf[0] = 0x00;
		}

		//ai
		cs2018prj::objMng::applyAll(&objMng, _dbDeltaTick);

		//랜더링 전처리
		TGE::copyScreenBuffer(pBakBuffer, pMapDataBuffer);

		//오브잭트 랜더링
		cs2018prj::objMng::renderAll(&objMng, pBakBuffer);

		//랜더 (화면 갱신)
		TGE::copyScreenBuffer(TGE::g_chiBuffer, pBakBuffer);
		TGE::updateBuffer(hStdout, TGE::g_chiBuffer);
	}

	cs2018prj::objMng::clearAll(&objMng);

	free(pBakBuffer);
	free(pMapDataBuffer);

	free(sprMiw.m_pSpriteBuf);
	free(sprAilen.m_pSpriteBuf);
	free(sprFire.m_pSpriteBuf);

	TGE::endTGE();

	return 0;

}