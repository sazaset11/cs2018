#include "stdafx.h"
#include "tge.h"
#include "tge_sprite.h"
#include "gameobject.h"

namespace cs2018prj {

	void _Init(S_GAMEOBJECT *pObj, irr::core::vector2df _pos, double _dbSpeed,
		tge_sprite::S_SPRITE_OBJECT *pSpr)
	{
		pObj->m_vPos = _pos;
		pObj->m_dbAngle = 0;
		pObj->m_dbSpeed = _dbSpeed;
		pObj->m_dbWorkTick = 0;
		pObj->m_nFSM = 0;
		pObj->m_pSprite = pSpr;
		pObj->m_bActive = false;
		pObj->m_pWepon = NULL;
	}

	namespace playerObject {
		void Init(S_GAMEOBJECT *pObj, irr::core::vector2df _pos, double _dbSpeed,
			tge_sprite::S_SPRITE_OBJECT *pSpr)
		{
			_Init(pObj, _pos, _dbSpeed, pSpr);
			pObj->m_fpApply = cs2018prj::playerObject::Apply;
			pObj->m_fpRender = cs2018prj::playerObject::Render;
		}

		void Apply(S_GAMEOBJECT *pObj, double _deltaTick)
		{
			switch (pObj->m_nFSM)
			{
			case 0:
				break;
			case 1:
				pObj->m_bActive = true;
				pObj->m_nFSM = 10;
				break;
			case 10: //정지
				pObj->m_nFSM++;
				break;
			case 11:
				if (TGE::input::g_KeyTable[VK_SPACE]) {
					pObj->m_nFSM = 20;
				}
				{
					if (TGE::input::g_KeyTable[VK_RIGHT]) {
						if (pObj->m_vPos.X >= 97) break;
						pObj->m_vPos.X += (pObj->m_dbSpeed * _deltaTick);
					}
					if (TGE::input::g_KeyTable[VK_LEFT]) {
						if (pObj->m_vPos.X <= 2) break;
						pObj->m_vPos.X -= (pObj->m_dbSpeed * _deltaTick);
					}
					if (TGE::input::g_KeyTable[VK_UP]) {
						if (pObj->m_vPos.Y <= 1) break;
						pObj->m_vPos.Y -= (pObj->m_dbSpeed * _deltaTick);
					}
					if (TGE::input::g_KeyTable[VK_DOWN]) {
						if (pObj->m_vPos.Y >= 21) break;
						pObj->m_vPos.Y += (pObj->m_dbSpeed * _deltaTick);
					}

					if (TGE::input::g_KeyTable['F']) {
						if (pObj->m_pWepon) {
							S_GAMEOBJECT *pWepon =
								(S_GAMEOBJECT *)pObj->m_pWepon;
							if (pWepon->m_nFSM == 0) { //슬립상태일때만...
								pWepon->m_nFSM = 10;
								pWepon->m_vPos.X = pObj->m_vPos.X;
								pWepon->m_vPos.Y = pObj->m_vPos.Y - 2;
							}
						}
					}
				}
				break;
			case 20: //move
			{

				irr::core::vector2df vTarget = irr::core::vector2df(TGE::input::g_cdMousePos.X,
					TGE::input::g_cdMousePos.Y);
				irr::core::vector2df vDir = vTarget - pObj->m_vPos;
				if (vDir.getLength() < 1) {
					pObj->m_nFSM = 10; //정지상태로...
				}
				else {
					vDir.normalize();
					pObj->m_vPos += irr::core::vector2df(
						vDir.X * _deltaTick * pObj->m_dbSpeed,
						vDir.Y * _deltaTick * pObj->m_dbSpeed);
				}
			}
			break;
			case 100:
				pObj->m_bActive = false;
				pObj->m_nFSM = 101;
				break;
			case 101: //ko 
				break;
			default:
				break;
			}
		}

		void Render(S_GAMEOBJECT *pObj, CHAR_INFO *pTargetBuf)
		{
			if (pObj->m_bActive) {
				tge_sprite::put(pObj->m_pSprite,
					irr::core::round32(pObj->m_vPos.X + pObj->m_translation.X),
					irr::core::round32(pObj->m_vPos.Y + pObj->m_translation.Y), pTargetBuf);
			}
		}

		void Activate(S_GAMEOBJECT *pObj)
		{
			pObj->m_nFSM = 1;
		}
	}

	namespace ailenObject {
		void Init(S_GAMEOBJECT *pObj, irr::core::vector2df _pos, double _dbSpeed,
			tge_sprite::S_SPRITE_OBJECT *pSpr)
		{
			_Init(pObj, _pos, _dbSpeed, pSpr);
			pObj->m_pTargetObj = NULL;
			pObj->m_fpApply = cs2018prj::ailenObject::Apply;
			pObj->m_fpRender = cs2018prj::playerObject::Render;
		}

		void Apply(S_GAMEOBJECT *pObj, double _deltaTick)
		{

			pObj->m_dbWorkTick += _deltaTick;

			switch (pObj->m_nFSM)
			{
			case 0: //
				break;
			case 10: //감시
			{
				pObj->m_bActive = true;
				pObj->m_nFSM++;
				pObj->m_dbWorkTick = 0;
			}
			break;
			case 11:
			{
				irr::core::vector2df _vdir(1, 0);
				_vdir *= pObj->m_dbSpeed;
				_vdir *= _deltaTick;
				_vdir.rotateBy(pObj->m_dbAngle);
				pObj->m_vPos += _vdir;
				if (pObj->m_dbWorkTick > 5) {
					pObj->m_dbAngle += 180;
					pObj->m_dbWorkTick = 0;
				}
				if (pObj->m_pTargetObj) {
					irr::core::vector2df a = pObj->m_vPos;
					irr::core::vector2df b = pObj->m_pTargetObj->m_vPos;
					double fDist = a.getDistanceFrom(b);
					if (fDist < 10) {
						pObj->m_nFSM = 20;
					}
				}
			}
			break;
			case 20: //추적모드 
			{
				irr::core::vector2df a = pObj->m_vPos;
				irr::core::vector2df b = pObj->m_pTargetObj->m_vPos;
				irr::core::vector2df c = b - a;
				double fDist = a.getDistanceFrom(b);
				c.normalize();
				pObj->m_vPos.X += c.X * pObj->m_dbSpeed * _deltaTick;
				pObj->m_vPos.Y += c.Y * pObj->m_dbSpeed * _deltaTick;

				/*if (a.getDistanceFrom(b) < 2.5) {
					if (pObj->m_pWepon) {
						cs2018prj::attackObject_claw::S_GAMEOBJECT *pWepon =
							(cs2018prj::attackObject_claw::S_GAMEOBJECT *)pObj->m_pWepon;
						pWepon->m_posx = pObj->m_pTargetObj->m_posx;
						pWepon->m_posy = pObj->m_pTargetObj->m_posy;
						pWepon->m_nFSM = 10;
						pObj->m_nFSM = 30;
					}
				}*/
				if (fDist > 10) {
					pObj->m_nFSM = 10;
				}
			}
				break;
			case 100:
				pObj->m_bActive = false;
				pObj->m_pDropObj->m_nFSM = 10;
				pObj->m_nFSM = 0;
				break;
			default:
				break;
			}
		}

		void Activate(S_GAMEOBJECT *pObj)
		{
			pObj->m_nFSM = 10;
		}


	}

	namespace attackObject {
		namespace claw {

		}
		namespace fire {
			void Init(S_GAMEOBJECT *pObj, irr::core::vector2df _pos, double _dbSpeed,
				tge_sprite::S_SPRITE_OBJECT *pSpr)
			{
				_Init(pObj, _pos, _dbSpeed, pSpr);
				pObj->m_fpApply = cs2018prj::attackObject::fire::Apply;
				pObj->m_fpRender = cs2018prj::playerObject::Render;

			}
			void Apply(S_GAMEOBJECT *pObj, double _deltaTick)
			{
				pObj->m_dbWorkTick += _deltaTick;
				switch (pObj->m_nFSM)
				{
				case 0:
					break;
				case 10:
					pObj->m_bActive = true;
					pObj->m_nFSM++;
					pObj->m_dbWorkTick = 0;
					break;
				case 11:
				{
					if (pObj->m_pTargetObj->m_bActive) {
						irr::core::vector2df a = pObj->m_vPos;
						irr::core::vector2df b = pObj->m_pTargetObj->m_vPos;
						double fDist = a.getDistanceFrom(b);

						if (fDist < 1.5) {
							pObj->m_nFSM = 0;
							pObj->m_bActive = false;
							pObj->m_pTargetObj->m_nFSM = 100;
						}
					}
					if (pObj->m_dbWorkTick > 2) {
						pObj->m_nFSM = 100;
					}
					else {
						irr::core::vector2df _vdir(1, 0);
						_vdir *= pObj->m_dbSpeed;
						_vdir *= _deltaTick;
						pObj->m_vPos += _vdir;
					}
				}
				break;
				case 100:
					pObj->m_bActive = false;
					pObj->m_nFSM = 0;
					break;
				default:
					break;
				}

			}

			void Activate(S_GAMEOBJECT *pObj) {
				pObj->m_nFSM = 10;
			}


		}
	}

	namespace itemObject {
		namespace key {
			void Init(S_GAMEOBJECT *pObj, irr::core::vector2df _pos, double _dbSpeed,
				tge_sprite::S_SPRITE_OBJECT *pSpr) {
				_Init(pObj, _pos, _dbSpeed, pSpr);
				pObj->m_fpApply = cs2018prj::itemObject::key::Apply;
				pObj->m_fpRender = cs2018prj::playerObject::Render;
			}
			void Apply(S_GAMEOBJECT *pObj, double _deltaTick) {
				pObj->m_dbWorkTick += _deltaTick;
				switch (pObj->m_nFSM)
				{
				case 0:
					break;
				case 10:
					pObj->m_vPos = pObj->m_pDropObj->m_vPos;
					pObj->m_bActive = true;
					if (pObj->m_pTargetObj->m_bActive) {
						irr::core::vector2df a = pObj->m_vPos;
						irr::core::vector2df b = pObj->m_pTargetObj->m_vPos;
						double fDist = a.getDistanceFrom(b);
						if (fDist < 1) {
							printf("초록색 키를 획득했습니다.");
							pObj->m_nFSM = 0;
							pObj->m_bActive = false;
						}
					}
					break;
				default:
					break;
				}
			}
			void Activate(S_GAMEOBJECT *pObj) {
				pObj->m_nFSM = 10;
			}
		}
		namespace door {
			void Init(S_GAMEOBJECT *pObj, irr::core::vector2df _pos, double _dbSpeed,
				tge_sprite::S_SPRITE_OBJECT *pSpr) {
				_Init(pObj, _pos, _dbSpeed, pSpr);
				pObj->m_fpApply = cs2018prj::itemObject::door::Apply;
				pObj->m_fpRender = cs2018prj::playerObject::Render;
			}
			void Apply(S_GAMEOBJECT *pObj, double _deltaTick) {
				pObj->m_dbWorkTick += _deltaTick;
				switch (pObj->m_nFSM)
				{
				case 0:
					break;
				case 10:
					pObj->m_bActive = true;
					break;
				default:
					break;
				}
			}
			void Activate(S_GAMEOBJECT *pObj) {
				pObj->m_nFSM = 10;
			}
		}
	}

	namespace objMng {

		void add(S_OBJECT_MNG *pObj, S_GAMEOBJECT *pGameObj) {
			pObj->m_pListObject[pObj->m_nIndex++] = pGameObj;
		}
		void applyAll(S_OBJECT_MNG *pObj, double _deltaTick) {
			for (int i = 0; i < pObj->m_nIndex; i++) {
				pObj->m_pListObject[i]->m_fpApply(pObj->m_pListObject[i], _deltaTick);
			}
		}
		void renderAll(S_OBJECT_MNG *pObj, CHAR_INFO *pBuf) {
			for (int i = 0; i < pObj->m_nIndex; i++) {
				pObj->m_pListObject[i]->m_fpRender(pObj->m_pListObject[i], pBuf);
			}
		}

		void clearAll(S_OBJECT_MNG *pObj)
		{
			for (int i = 0; i < pObj->m_nIndex; i++) {
				if (pObj->m_pListObject[i] != NULL) {
					free(pObj->m_pListObject[i]);
				}
			}
		}


	}



}