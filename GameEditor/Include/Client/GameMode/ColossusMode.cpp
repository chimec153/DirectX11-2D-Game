#include "ColossusMode.h"
#include "PathManager.h"
#include "../../tinyxml.h"
#include "Scene/Scene.h"
#include "Tile/TileMap.h"
#include "Resource/ResourceManager.h"
#include "../Object/Player.h"
#include "../Object/Colossus.h"
#include "SoundManager.h"
#include "Component/Sound.h"
#include "Camera/CameraManager.h"
#include "Component/Camera.h"
#include "../Object/Bullet.h"
#include "../Object/Effect.h"
#include "Component/SpriteComponent.h"
#include "../BossManager.h"
#include "../Object/TitanFont.h"
#include "Device.h"
#include "Resource/Material.h"

bool CColossusMode::m_bClear = false;

CColossusMode::CColossusMode()
{
	SetType(BOSS_TYPE::COLOSSUS);
}

CColossusMode::~CColossusMode()
{
}

bool CColossusMode::Init()
{
	if (!CTileMode::Init())
		return false;

	m_pPlayer->SetWorldPos(480.f, 416.f, 0.f);

	return true;
}

void CColossusMode::Start()
{
	CGameMode::Start();

	int iSlot = GET_SINGLE(CBossManager)->GetSlot();
	SLOTINFO tInfo = GET_SINGLE(CBossManager)->GetInfo(iSlot);

	if ((int)tInfo.eType & (int)BOSS_TYPE::COLOSSUS)
	{
		GET_SINGLE(CBossManager)->Load();
	}


	CObj* pObj = GET_SINGLE(CBossManager)->FindMonster("boss_colossus");

	if (pObj)
	{
		CObj* pPrevObj = m_pScene->FindLayer("Default")->FindObj("boss_colossus");

		if (pPrevObj)
		{
			pPrevObj->Destroy();

			pPrevObj->Release();
		}

		m_pScene->FindLayer("Default")->AddObj(pObj);

		pObj->Release();
	}
}

bool CColossusMode::LoadXml(const char* pFileName, const std::string& strPathKey)
{
	return CTileMode::LoadXml(pFileName, strPathKey);
}

void CColossusMode::searchXMLData(TiXmlElement* pElem)
{
	CTileMode::searchXMLData(pElem);
}

void CColossusMode::LoadResource()
{

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ColossusHeadIdle", "colossus");

	GET_SINGLE(CResourceManager)->AddFrame("ColossusHeadIdle", Vector2(0.f, 0.f), Vector2(64.f, 64.f));

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ColossusBodyIdle", "colossus");

	GET_SINGLE(CResourceManager)->AddFrame("ColossusBodyIdle", Vector2(0.f, 112.f), Vector2(96.f, 240.f));

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ColossusLeftHandIdle", "colossus");

	GET_SINGLE(CResourceManager)->AddFrame("ColossusLeftHandIdle", Vector2(407.f, 214.f), Vector2(407.f, 214.f)+ Vector2(48.f, 48.f));
}
