#include "pch.h"
#include "BrainMode.h"
#include "../Object/Fire.h"
#include "../Object/BrainFreeze.h"
#include "../Object/FireButton.h"
#include "Scene/Scene.h"
#include "../BossManager.h"
#include "../Object/Effect.h"
#include "Component/SpriteComponent.h"
#include "Resource/Material.h"
#include "Component/ColliderRect.h"

bool CBrainMode::m_bClear = false;

CBrainMode::CBrainMode()	:
	m_pFire(nullptr)
{
	SetType(BOSS_TYPE::BRAINFREEZE);
}

CBrainMode::~CBrainMode()
{
	SAFE_RELEASE(m_pFire);
}

CFire* CBrainMode::GetFire() const
{
	if (m_pFire)
		m_pFire->AddRef();

	return m_pFire;
}

bool CBrainMode::Init()
{
	CFireButton* pButton = CScene::CreateProtoObj<CFireButton>("4292", m_pScene, m_pScene->GetSceneType());

	SAFE_RELEASE(pButton);

	m_pFire = m_pScene->CreateObject<CFire>("Fire", m_pScene->FindLayer("Default"), m_pScene->GetSceneType());

	m_pFire->Enable(false);

	if (!CTileMode::Init())
		return false;

	if (m_bClear)
	{
		CObj* pObj = m_pScene->FindLayer("Default")->FindObj("boss_brainfreeze");

		if (pObj)
		{
			pObj->Destroy();

			pObj->Release();
		}

		pObj = GET_SINGLE(CBossManager)->FindMonster("boss_brainfreeze");

		m_pScene->FindLayer("Default")->AddObj(pObj);

		SAFE_RELEASE(pObj);
	}

	m_pPlayer->SetWorldPos(464.f, 384.f, 0.f);

	for (int i = 0; i < 4; ++i)
	{
		char strName[32] = {};

		sprintf_s(strName, "Mucus%d", i+1);

		CEffect* pMucus1 = CScene::CreateProtoObj<CEffect>(strName, m_pScene, m_pScene->GetSceneType());

		pMucus1->SetSpeed(0.f);

		CSpriteComponent* pCom = pMucus1->CreateComponent<CSpriteComponent>("Mucus", m_pScene->FindLayer("Temp"));

		pCom->SetWorldScale(32.f, 32.f, 0.f);
		pCom->SetPivot(0.5f, 0.5f, 0.f);
		pCom->CreateSprite("idle", strName, LOOP_OPTION::LOOP);
		pCom->AddRenderState("DepthNoWrite");
		pCom->AddNotify("idle", "Scale", 7.f);
		pCom->AddNotify("idle", "Destroy", 8.f);		

		CMaterial* pMtrl = pCom->GetMaterial();

		pMtrl->SetDiffuseColor(1.f, 0.5f, 0.25f, 0.5f);

		SAFE_RELEASE(pMtrl);

		pMucus1->SetRootComponent(pCom);

		CColliderRect* pCol = pMucus1->CreateComponent<CColliderRect>("MucusBody", m_pScene->FindLayer("Default"));

		pCol->SetExtent(32.f, 32.f);

		pCom->AddChild(pCol);

		SAFE_RELEASE(pCol);

		SAFE_RELEASE(pCom);

		SAFE_RELEASE(pMucus1);
	}

	SetFontText("BRAINFREEZE");
	SetDescText("BRAIN~OF~THE~GUARDIAN");

	return true;
}

void CBrainMode::Start()
{
	CTileMode::Start();

	int iSlot = GET_SINGLE(CBossManager)->GetSlot();
	SLOTINFO tInfo = GET_SINGLE(CBossManager)->GetInfo(iSlot);

	if ((int)tInfo.eType & (int)BOSS_TYPE::BRAINFREEZE)
	{
		GET_SINGLE(CBossManager)->Load();
	}

	CObj* pObj = GET_SINGLE(CBossManager)->FindMonster("boss_brainfreeze");

	if (pObj)
	{
		CObj* pPrevObj = m_pScene->FindLayer("Default")->FindObj("boss_brainfreeze");

		if (pPrevObj)
		{
			pPrevObj->Destroy();

			pPrevObj->Release();
		}

		m_pScene->FindLayer("Default")->AddObj(pObj);

		pObj->Release();
	}
}
