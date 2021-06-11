#include "pch.h"
#include "EyeCubeMode.h"
#include "../Object/Eyecube.h"
#include "Scene/Scene.h"
#include "../BossManager.h"
#include "Camera/CameraManager.h"
#include "Component/Camera.h"

bool CEyeCubeMode::m_bClear = false;

CEyeCubeMode::CEyeCubeMode()
{
	SetType(BOSS_TYPE::EYECUBE);
}

CEyeCubeMode::~CEyeCubeMode()
{
}

bool CEyeCubeMode::Init()
{
	if (!CTileMode::Init())
		return false;

	m_pPlayer->SetWorldPos(472.f, 408.f, 0.f);

	SetFontText("EYECUBE");
	SetDescText("EYE~OF~THE~GUARDIAN");

	return true;
}

void CEyeCubeMode::Start()
{
	CTileMode::Start();

	int iSlot = GET_SINGLE(CBossManager)->GetSlot();
	SLOTINFO tInfo = GET_SINGLE(CBossManager)->GetInfo(iSlot);

	if ((int)tInfo.eType & (int)BOSS_TYPE::EYECUBE)
	{
		GET_SINGLE(CBossManager)->Load();
	}

	CObj* pObj = GET_SINGLE(CBossManager)->FindMonster("boss_eyecube");

	if (pObj)
	{
		CObj* pPrevObj = m_pScene->FindLayer("Default")->FindObj("boss_eyecube");

		if (pPrevObj)
		{
			pPrevObj->Destroy();

			pPrevObj->Release();
		}

		m_pScene->FindLayer("Default")->AddObj(pObj);

		pObj->Release();
	}
}
