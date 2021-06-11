#include "pch.h"
#include "EndGameMode.h"
#include "Scene/Scene.h"
#include "UI/UIFont.h"
#include "../Object/Player.h"
#include "Scene/SceneManager.h"
#include "Device.h"
#include "Component/Mesh2DComponent.h"
#include "Resource/Material.h"
#include "Component/SpriteComponent.h"
#include "Resource/ResourceManager.h"
#include "PathManager.h"
#include "StartGameMode.h"

CEndGameMode::CEndGameMode()	:
m_fTime(0.f)
, m_iStage(0)
, m_fFadeInLimit(0.5f)
, m_fFadeOutLimit(1.5f)
, m_bFadeIn(true)
, m_fStayTimeLimit(1.5f)
{
}

CEndGameMode::~CEndGameMode()
{
}

bool CEndGameMode::Init()
{
	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

	CPlayer* pPlayer = (CPlayer*)pScene->GetGameMode()->GetPlayer();

	float fTime = 123153.24f;

	if (pPlayer)
	{
		fTime = pPlayer->GetPlayTime();
	}

	SAFE_RELEASE(pPlayer);

	int iSecond = (int)fTime;

	int iMinute = iSecond / 60;

	int iHour = iMinute / 60;

	iSecond -= iMinute * 60;

	iMinute -= iHour * 60;

	Resolution tRS = RESOLUTION;

	CObj* pUI = m_pScene->CreateObject<CObj>("UI", m_pScene->FindLayer("UI"), m_pScene->GetSceneType());

	CUIFont* pTime = pUI->CreateComponent<CUIFont>("Time", m_pScene->FindLayer("Default"));

	pTime->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	pTime->SetWorldPos((float)tRS.iWidth/2.f, (float)tRS.iHeight/2.f, 0.f);

	TCHAR strTime[64] = {};

	float fFrac = ((float)fTime - (int)fTime) * 100.f;

	fFrac -= fFrac - (int)fFrac;

	swprintf_s(strTime, TEXT("Clear Time: %02d:%02d:%02d.%02d"), iHour, iMinute, iSecond, (int)fFrac);

	pTime->SetText(strTime);
	pTime->SetRect(-200.f, 50.f, 200.f, 0.f);
	pTime->SetAlignH(TEXT_ALIGN_HORISONTAL::CENTER);
	pTime->SetAlignV(TEXT_ALIGN_VERTICAL::MID);

	pUI->SetRootComponent(pTime);

	SAFE_RELEASE(pTime);

	SAFE_RELEASE(pUI);

	m_pFadeObj = m_pScene->CreateObject<CObj>("Fade", m_pScene->FindLayer("Fore"));

	m_pFade = m_pFadeObj->CreateComponent<CMesh2DComponent>("FadeCom", m_pScene->FindLayer("Speacial"));

	m_pFade->SetWorldScale((float)tRS.iWidth, (float)tRS.iHeight * 4.f, 0.f);
	m_pFade->SetPivot(0.5f, 0.5f, 0.f);
	m_pFade->SetTexture(REGISTER_TYPE::RT_DIF, "util");
	m_pFade->SetWorldPos(0.f, -(float)tRS.iHeight / 2.f, -520.f);
	m_pFade->AddRenderState("DepthNoWrite");

	CMaterial* pFadeMtrl = m_pFade->GetMaterial();

	pFadeMtrl->SetDiffuseColor(0.f, 0.f, 0.f, 0.f);

	SAFE_RELEASE(pFadeMtrl);

	m_pFadeObj->SetRootComponent(m_pFade);

	SetFade(1.f, 1.f, 0.f);

	return true;
}

void CEndGameMode::Update(float fTime)
{
	CTileMode::Update(fTime);
	static bool bStay = false;

	if (m_iStage < (int)Stage::TITLE)
	{
		m_fTime += fTime;
	}

	if (m_bFadeIn)
	{
		CMaterial* pMtrl = nullptr;

		switch ((Stage)m_iStage)
		{
		case Stage::PAD:
		{
			CMaterial* pMtrl = m_pFade->GetMaterial();

			pMtrl->SetDiffuseColor(0.f, 0.f, 0.f, (m_fFadeInLimit - m_fTime) / m_fFadeInLimit);

			SAFE_RELEASE(pMtrl);
		}
		break;
		}

		if (pMtrl)
		{
			pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, m_fTime / m_fFadeInLimit);

			pMtrl->Release();
		}

		if (m_fTime >= m_fFadeInLimit)
		{
			m_fTime = 0.f;
			m_bFadeIn = false;
			bStay = true;
		}
	}

	else if (bStay)
	{
		if (m_fTime >= m_fStayTimeLimit)
		{
			m_fTime = 0.f;
			bStay = false;
		}
	}
	else
	{
		CMaterial* pMtrl = nullptr;

		switch ((Stage)m_iStage)
		{
		case Stage::PAD:
		{
			CMaterial* pMtrl = m_pFade->GetMaterial();

			pMtrl->SetDiffuseColor(0.f, 0.f, 0.f, m_fTime / m_fFadeOutLimit);

			SAFE_RELEASE(pMtrl);
		}
		break;
		}

		if (pMtrl)
		{
			pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, (m_fFadeOutLimit - m_fTime) / m_fFadeOutLimit);

			pMtrl->Release();
		}

		if (m_fTime >= m_fFadeOutLimit)
		{
			m_fTime = 0.f;
			m_bFadeIn = true;

			switch ((Stage)m_iStage)
			{
			case Stage::PAD:
			{
				GET_SINGLE(CSceneManager)->CreateNextScene();

				GET_SINGLE(CSceneManager)->SetGameMode<CStartGameMode>(false);
			}
				break;
			}

		}
	}
}