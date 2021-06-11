#include "StartGameMode.h"
#include "Object/Obj.h"
#include "Scene/Scene.h"
#include "UI/UIButton.h"
#include "Resource/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "MainGameMode.h"
#include "Engine.h"
#include "Camera/CameraManager.h"
#include "Component/Camera.h"
#include "Component/Mesh2DComponent.h"
#include "Resource/Material.h"
#include "Component/SpriteComponent.h"
#include "PathManager.h"
#include "Device.h"
#include "Input.h"
#include "UI/UIFont.h"
#include "Component/Sound.h"
#include "SoundManager.h"
#include "../BossManager.h"

bool CStartGameMode::m_bDelete = false;
bool CStartGameMode::m_bStay = false;

CStartGameMode::CStartGameMode()	:
	m_pTitle(nullptr)
	, m_pAn(nullptr)
	, m_pGamePad(nullptr)
	, m_pLogo(nullptr)
	, m_fTime(0.f)
	, m_iStage(0)
	, m_fFadeInLimit(0.5f)
	, m_fFadeOutLimit(1.5f)
	, m_bFadeIn(true)
	, m_pArrowL(nullptr)
	, m_pArrowR(nullptr)
	, m_iSelect(0)
	, m_iDepth(0)
	, m_iMenu(0)
	, m_pExitMenu(nullptr)
	, m_pArrowL2(nullptr)
	, m_pArrowR2(nullptr)
	, m_pStartMenu(nullptr)
	, m_pArrowL3(nullptr)
	, m_pArrowR3(nullptr)
	, m_pHelp(nullptr)
	, m_fStayTimeLimit(1.5f)
	, m_pBGM(nullptr)
	, m_pEffect(nullptr)
{
}

CStartGameMode::~CStartGameMode()
{
	GET_SINGLE(CInput)->DeleteActionCallBack("X");
	GET_SINGLE(CInput)->DeleteActionCallBack("C");
	GET_SINGLE(CInput)->DeleteActionCallBack("UpArrow");
	GET_SINGLE(CInput)->DeleteActionCallBack("DownArrow");

	SAFE_RELEASE(m_pTitle);
	SAFE_RELEASE(m_pAn);
	SAFE_RELEASE(m_pGamePad);
	SAFE_RELEASE(m_pLogo);
	SAFE_RELEASE(m_pArrowL);
	SAFE_RELEASE(m_pArrowR);
	SAFE_RELEASE(m_pExitMenu);
	SAFE_RELEASE(m_pArrowL2);
	SAFE_RELEASE(m_pArrowR2);
	SAFE_RELEASE(m_pStartMenu);
	SAFE_RELEASE(m_pArrowL3);
	SAFE_RELEASE(m_pArrowR3);
	SAFE_RELEASE(m_pHelp);
	SAFE_RELEASE(m_pBGM);
	SAFE_RELEASE(m_pEffect);
}

bool CStartGameMode::Init()
{
	GET_SINGLE(CBossManager)->LoadSlotInfo();

	static bool bInit = true;

	if (bInit)
	{
		GET_SINGLE(CPathManager)->AddPath("Player_Sound_Path", L"Data\\SOUND\\PLAYER\\");
		GET_SINGLE(CPathManager)->AddPath("Sound_Path2", L"Data\\SOUND\\");

		bInit = false;
	}

	GET_SINGLE(CInput)->AddActionKey("X", DIK_X);
	GET_SINGLE(CInput)->AddActionKey("C", DIK_C);
	GET_SINGLE(CInput)->AddActionKey("UpArrow", DIK_UP);
	GET_SINGLE(CInput)->AddActionKey("DownArrow", DIK_DOWN);
	GET_SINGLE(CInput)->AddActionKey("LeftArrow", DIK_LEFT);
	GET_SINGLE(CInput)->AddActionKey("RightArrow", DIK_RIGHT);

	GET_SINGLE(CInput)->AddActionBind("X", KEY_TYPE::KT_DOWN, this, &CStartGameMode::DownX);
	GET_SINGLE(CInput)->AddActionBind("C", KEY_TYPE::KT_DOWN, this, &CStartGameMode::DownC);
	GET_SINGLE(CInput)->AddActionBind("UpArrow", KEY_TYPE::KT_DOWN, this, &CStartGameMode::DownUpArrow);
	GET_SINGLE(CInput)->AddActionBind("DownArrow", KEY_TYPE::KT_DOWN, this, &CStartGameMode::DownDownArrow);

	if (!LoadResources())
		return false;

	LoadSequance("data\\startSeqs.csv");

	LoadFrame("data\\startFrames.csv");

	if (!SetUI())
		return false;

	CCamera* pUICam = GET_SINGLE(CCameraManager)->GetUICam();

	pUICam->SetRect(0.25f, 0.75f, 0.25f, 0.75f);

	SAFE_RELEASE(pUICam);

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	pCam->SetWorldPos(800.f, 600.f, 0.f);

	pCam->SetRect(0.25f, 0.75f, 0.25f, 0.75f);

	LoadXml("IMG\\jam.png.xml");

	LoadXml("MAPS\\JAM\\jamtitle.tmx");

	m_pFadeObj = m_pScene->CreateObject<CObj>("Fade", m_pScene->FindLayer("Fore"));

	m_pFade = m_pFadeObj->CreateComponent<CMesh2DComponent>("FadeCom", m_pScene->FindLayer("Speacial"));

	Resolution tRS = pCam->GetResolution();

	SAFE_RELEASE(pCam);

	m_pFade->SetWorldScale((float)tRS.iWidth, (float)tRS.iHeight * 4.f, 0.f);
	m_pFade->SetPivot(0.5f, 0.5f, 0.f);
	m_pFade->SetTexture(REGISTER_TYPE::RT_DIF, "util");
	m_pFade->SetWorldPos(0.f, -(float)tRS.iHeight/2.f, -520.f);
	m_pFade->AddRenderState("DepthNoWrite");

	CMaterial* pFadeMtrl = m_pFade->GetMaterial();

	pFadeMtrl->SetDiffuseColor(0.f, 0.f, 0.f, 0.f);

	SAFE_RELEASE(pFadeMtrl);

	m_pFadeObj->SetRootComponent(m_pFade);

	SetFade(1.f, 1.f, 0.f);

	GET_SINGLE(CSoundManager)->LoadSound("Forest", SOUND_TYPE::BGM,
		"SOUND\\SFX\\AMBIENCE\\Forest.ogg", DATA_PATH);

	GET_SINGLE(CSoundManager)->LoadSound("Motif", SOUND_TYPE::EFFECT,
		"UI\\Motif.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Move", SOUND_TYPE::EFFECT,
		"UI\\Move.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("No", SOUND_TYPE::EFFECT,
		"UI\\No.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Opening", SOUND_TYPE::EFFECT,
		"UI\\Opening.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Select", SOUND_TYPE::EFFECT,
		"UI\\Select.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("StartGame", SOUND_TYPE::EFFECT,
		"UI\\StartGame.ogg", "Sound_Path2");

	m_pBGM = m_pFadeObj->CreateComponent<CSound>("BGM", m_pScene->FindLayer("Default"));

	m_pBGM->SetSound("Forest");
	m_pBGM->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	m_pBGM->SetVol(100.f);
	m_pBGM->SetMaxVol(100.f);
	m_pBGM->SetMinVol(100.f);

	m_pFade->AddChild(m_pBGM);

	m_pEffect = m_pFadeObj->CreateComponent<CSound>("Effect", m_pScene->FindLayer("Default"));

	m_pEffect->SetSound("Opening");
	m_pEffect->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	m_pEffect->SetVol(100.f);
	m_pEffect->SetMaxVol(100.f);
	m_pEffect->SetMinVol(100.f);

	m_pFade->AddChild(m_pEffect);

	return true;
}

void CStartGameMode::Update(float fTime)
{
	CTileMode::Update(fTime);

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	pCam->SetWorldPos(800.f, 600.f, 0.f);
	SAFE_RELEASE(pCam);

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
			CSpriteComponent* pCom = m_pGamePad->FindComByType<CSpriteComponent>();

			pMtrl = pCom->GetMaterial();

			SAFE_RELEASE(pCom);
		}
			break;
		case Stage::AN:
		{
			CSpriteComponent* pCom = m_pAn->FindComByType<CSpriteComponent>();

			pMtrl = pCom->GetMaterial();

			SAFE_RELEASE(pCom);
		}
			break;
		case Stage::LOGO:
		{
			CSpriteComponent* pCom = m_pLogo->FindComByType<CSpriteComponent>();

			pMtrl = pCom->GetMaterial();

			SAFE_RELEASE(pCom);
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
			m_bStay = true;
		}
	}

	else if (m_bStay)
	{
		if (m_fTime >= m_fStayTimeLimit)
		{
			m_fTime = 0.f;
			m_bStay = false;
		}
	}

	else
	{
		CMaterial* pMtrl = nullptr;

		switch ((Stage)m_iStage)
		{
		case Stage::PAD:
		{
			CSpriteComponent* pCom = m_pGamePad->FindComByType<CSpriteComponent>();

			pMtrl = pCom->GetMaterial();

			SAFE_RELEASE(pCom);
		}
			break;
		case Stage::AN:
		{
			CSpriteComponent* pCom = m_pAn->FindComByType<CSpriteComponent>();

			pMtrl = pCom->GetMaterial();

			SAFE_RELEASE(pCom);
		}
			break;
		case Stage::LOGO:
		{
			CSpriteComponent* pCom = m_pLogo->FindComByType<CSpriteComponent>();

			pMtrl = pCom->GetMaterial();

			SAFE_RELEASE(pCom);
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
			m_bStay = false;

			switch ((Stage)m_iStage)
			{
			case Stage::PAD:
				m_pGamePad->Enable(false);
				break;
			case Stage::AN:
				m_pAn->Enable(false);
				break;
			case Stage::LOGO:
				m_pLogo->Enable(false);
				break;
			}

			if (m_iStage != (int)Stage::TITLE)
			{
				m_iStage++;
			}

			switch ((Stage)m_iStage)
			{
			case Stage::AN:
				m_pAn->Enable(true);
				m_pBGM->Play(0.f);
				m_pEffect->Play(0.f);
				break;
			case Stage::LOGO:
				m_pLogo->Enable(true);
				break;
			case Stage::TITLE:
				m_pTitle->Enable(true);

				m_pHelp->Enable(true);

				m_pEffect->SetSound("Motif");
				m_pEffect->Play(0.f);
				break;
			}
		}
	}

	if (m_iStage >= (int)Stage::TITLE)
	{
		SetFade(0.5f, 0.5f, 0.f);
	}
}

bool CStartGameMode::SetUI()
{
	Resolution tRS = RESOLUTION;

	m_pGamePad = m_pScene->CreateObject<CObj>("GamePad", m_pScene->FindLayer("UI"), m_pScene->GetSceneType());

	CSpriteComponent* pGamePadCom = m_pGamePad->CreateComponent<CSpriteComponent>("GamePad", m_pScene->FindLayer("UI"));

	pGamePadCom->SetWorldScale(480.f, 360.f, 0.f);

	pGamePadCom->SetPivot(0.5f, 0.5f, 0.f);

	pGamePadCom->CreateSprite("Idle", "GamePad", LOOP_OPTION::LOOP);
	pGamePadCom->AddRenderState("DepthNoWrite");

	pGamePadCom->SetWorldPos((float)tRS.iWidth /2.f, (float)tRS.iHeight/2.f, 0.f);
	pGamePadCom->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	m_pGamePad->SetRootComponent(pGamePadCom);

	SAFE_RELEASE(pGamePadCom);

	m_pLogo = m_pScene->CreateObject<CObj>("Devolver", m_pScene->FindLayer("UI"), m_pScene->GetSceneType());

	CSpriteComponent* pDevCom = m_pLogo->CreateComponent<CSpriteComponent>("Devolver", m_pScene->FindLayer("UI"));

	pDevCom->SetWorldScale(480.f, 360.f, 0.f);

	pDevCom->SetPivot(0.5f, 0.5f, 0.f);

	pDevCom->CreateSprite("Idle", "Devolver", LOOP_OPTION::LOOP);
	pDevCom->AddRenderState("DepthNoWrite");

	pDevCom->SetWorldPos((float)tRS.iWidth / 2.f, (float)tRS.iHeight / 2.f, 0.f);
	pDevCom->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

	CMaterial* pMtrl = pDevCom->GetMaterial();

	pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.f);

	SAFE_RELEASE(pMtrl);

	m_pLogo->SetRootComponent(pDevCom);
	m_pLogo->Enable(false);

	SAFE_RELEASE(pDevCom);

	m_pAn = m_pScene->CreateObject<CObj>("An", m_pScene->FindLayer("UI"), m_pScene->GetSceneType());

	CSpriteComponent* pAnCom = m_pAn->CreateComponent<CSpriteComponent>("An", m_pScene->FindLayer("UI"));

	pAnCom->SetWorldScale(480.f, 360.f, 0.f);

	pAnCom->SetPivot(0.5f, 0.5f, 0.f);

	pAnCom->CreateSprite("Idle", "An", LOOP_OPTION::LOOP);
	pAnCom->AddRenderState("DepthNoWrite");

	pAnCom->SetWorldPos((float)tRS.iWidth / 2.f, (float)tRS.iHeight / 2.f, 0.f);
	pAnCom->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

	pMtrl = pAnCom->GetMaterial();

	pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.f);

	SAFE_RELEASE(pMtrl);
	m_pAn->SetRootComponent(pAnCom);
	m_pAn->Enable(false);

	SAFE_RELEASE(pAnCom);

	m_pTitle = m_pScene->CreateObject<CObj>("Title", m_pScene->FindLayer("UI"), m_pScene->GetSceneType());

	CSpriteComponent* pTitleCom = m_pTitle->CreateComponent<CSpriteComponent>("Title", m_pScene->FindLayer("UI"));

	pTitleCom->SetWorldScale(272.f, 128.f, 0.f);

	pTitleCom->SetPivot(0.5f, 0.5f, 0.f);

	pTitleCom->CreateSprite("Idle", "Title", LOOP_OPTION::LOOP);
	pTitleCom->AddRenderState("DepthNoWrite");

	pTitleCom->SetWorldPos((float)tRS.iWidth / 2.f, (float)tRS.iHeight*5 / 8.f, 0.f);
	pTitleCom->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

	m_pTitle->SetRootComponent(pTitleCom);
	m_pTitle->Enable(false);

	CSpriteComponent* pRedux = m_pTitle->CreateComponent<CSpriteComponent>("Redux", m_pScene->FindLayer("UI"));

	pRedux->SetWorldScale(272.f, 16.f, 0.f);

	pRedux->SetPivot(0.5f, 0.5f, 0.f);

	pRedux->CreateSprite("Idle", "Redux", LOOP_OPTION::LOOP);
	pRedux->AddRenderState("DepthNoWrite");

	pRedux->AddWorldPos(0.f, -72.f, 0.f);
	pRedux->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	pRedux->SetInheritScale(false);

	pTitleCom->AddChild(pRedux);

	CUIFont* pStart = m_pTitle->CreateComponent<CUIFont>("Start", m_pScene->FindLayer("UI"));

	pStart->SetPivot(0.5f, 0.5f, 0.f);
	pStart->SetText(TEXT("Start"));
	pStart->SetRect(0.f, 0.f, 400.f, 0.f);
	pStart->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	pStart->SetAlignH(TEXT_ALIGN_HORISONTAL::CENTER);
	pStart->SetAlignV(TEXT_ALIGN_VERTICAL::MID);
	pStart->AddRelativePos(-200.f, 22.f, 0.f);

	pRedux->AddChild(pStart);

	CUIFont* pOption = m_pTitle->CreateComponent<CUIFont>("Option", m_pScene->FindLayer("UI"));

	pOption->SetPivot(0.5f, 0.5f, 0.f);
	pOption->SetText(TEXT("Option"));
	pOption->SetRect(0.f, 0.f, 400.f, 0.f);
	pOption->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	pOption->SetAlignH(TEXT_ALIGN_HORISONTAL::CENTER);
	pOption->SetAlignV(TEXT_ALIGN_VERTICAL::MID);
	pOption->AddRelativePos(0.f, -32.f, 0.f);

	pStart->AddChild(pOption);

	CUIFont* pExit = m_pTitle->CreateComponent<CUIFont>("Exit", m_pScene->FindLayer("UI"));

	pExit->SetPivot(0.5f, 0.5f, 0.f);
	pExit->SetText(TEXT("Exit"));
	pExit->SetRect(0.f, 0.f, 400.f, 0.f);
	pExit->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	pExit->SetAlignH(TEXT_ALIGN_HORISONTAL::CENTER);
	pExit->SetAlignV(TEXT_ALIGN_VERTICAL::MID);
	pExit->AddRelativePos(0.f, -32.f, 0.f);

	pOption->AddChild(pExit);

	m_pArrowL = m_pTitle->CreateComponent<CSpriteComponent>("ArrowL", m_pScene->FindLayer("UI"));

	m_pArrowL->SetWorldScale(16.f, 16.f, 0.f);

	m_pArrowL->SetPivot(0.5f, 0.5f, 0.f);

	m_pArrowL->CreateSprite("Idle", "ArrowL3", LOOP_OPTION::LOOP);
	m_pArrowL->AddRenderState("DepthNoWrite");

	m_pArrowL->AddWorldPos(-48.f, -21.f, 0.f);
	m_pArrowL->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	m_pArrowL->SetInheritScale(false);

	pRedux->AddChild(m_pArrowL);

	m_pArrowR = m_pTitle->CreateComponent<CSpriteComponent>("ArrowR", m_pScene->FindLayer("UI"));

	m_pArrowR->SetWorldScale(16.f, 16.f, 0.f);

	m_pArrowR->SetPivot(0.5f, 0.5f, 0.f);

	m_pArrowR->CreateSprite("Idle", "ArrowR3", LOOP_OPTION::LOOP);
	m_pArrowR->AddRenderState("DepthNoWrite");

	m_pArrowR->AddWorldPos(48.f, -21.f, 0.f);
	m_pArrowR->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	m_pArrowR->SetInheritScale(false);

	pRedux->AddChild(m_pArrowR);

	SAFE_RELEASE(pStart);

	SAFE_RELEASE(pOption);

	SAFE_RELEASE(pExit);

	SAFE_RELEASE(pRedux);

	SAFE_RELEASE(pTitleCom);

	m_pExitMenu = m_pScene->CreateObject<CObj>("ExitMenu", m_pScene->FindLayer("UI"), m_pScene->GetSceneType());

	CSpriteComponent* pBarL = m_pExitMenu->CreateComponent<CSpriteComponent>("ExitBarL", m_pScene->FindLayer("UI"));

	pBarL->SetWorldScale(112.f, 16.f, 0.f);

	pBarL->SetPivot(0.5f, 0.5f, 0.f);

	pBarL->CreateSprite("Idle", "LongBarL", LOOP_OPTION::LOOP);
	pBarL->AddRenderState("DepthNoWrite");

	pBarL->SetWorldPos((float)tRS.iWidth / 2.f - 96.f, (float)tRS.iHeight  / 2.f, 0.f);
	pBarL->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

	CSpriteComponent* pBarR = m_pExitMenu->CreateComponent<CSpriteComponent>("ExitBarR", m_pScene->FindLayer("UI"));

	pBarR->SetWorldScale(112.f, 16.f, 0.f);

	pBarR->SetPivot(0.5f, 0.5f, 0.f);

	pBarR->CreateSprite("Idle", "LongBarR", LOOP_OPTION::LOOP);
	pBarR->AddRenderState("DepthNoWrite");
	pBarR->SetInheritScale(false);
	pBarR->AddRelativePos(192.f, 0.f, 0.f);
	pBarR->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

	pBarL->AddChild(pBarR);
	m_pExitMenu->SetRootComponent(pBarL);
	m_pExitMenu->Enable(false);

	SAFE_RELEASE(pBarR);

	CUIFont* pFont = m_pExitMenu->CreateComponent<CUIFont>("ExitFont", m_pScene->FindLayer("UI"));

	pFont->SetRect(-200.f, 0.f, 200.f, 0.f);
	pFont->SetPivot(0.5f, 0.5f, 0.f);
	pFont->SetText(TEXT("Exit game?"));
	pFont->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	pFont->SetAlignH(TEXT_ALIGN_HORISONTAL::CENTER);
	pFont->SetAlignV(TEXT_ALIGN_VERTICAL::MID);
	pFont->AddRelativePos(96.f, 0.f, 0.f);

	pBarL->AddChild(pFont);
	SAFE_RELEASE(pBarL);

	CUIFont* pYes = m_pExitMenu->CreateComponent<CUIFont>("Yes", m_pScene->FindLayer("UI"));

	pYes->SetRect(-200.f, 0.f, 200.f, 0.f);
	pYes->SetPivot(0.5f, 0.5f, 0.f);
	pYes->SetText(TEXT("Yes"));
	pYes->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	pYes->SetAlignH(TEXT_ALIGN_HORISONTAL::CENTER);
	pYes->SetAlignV(TEXT_ALIGN_VERTICAL::MID);
	pYes->AddRelativePos(0.f, -68.f, 0.f);

	pFont->AddChild(pYes);

	CUIFont* pNo = m_pExitMenu->CreateComponent<CUIFont>("No", m_pScene->FindLayer("UI"));

	pNo->SetRect(-200.f, 0.f, 200.f, 0.f);
	pNo->SetPivot(0.5f, 0.5f, 0.f);
	pNo->SetText(TEXT("No"));
	pNo->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	pNo->SetAlignH(TEXT_ALIGN_HORISONTAL::CENTER);
	pNo->SetAlignV(TEXT_ALIGN_VERTICAL::MID);
	pNo->AddRelativePos(0.f, -32.f, 0.f);

	pYes->AddChild(pNo);

	SAFE_RELEASE(pNo);

	SAFE_RELEASE(pYes);

	m_pArrowL2 = m_pExitMenu->CreateComponent<CSpriteComponent>("ArrowL2", m_pScene->FindLayer("UI"));

	m_pArrowL2->SetWorldScale(16.f, 16.f, 0.f);

	m_pArrowL2->SetPivot(0.5f, 0.5f, 0.f);

	m_pArrowL2->CreateSprite("Idle", "ArrowL1", LOOP_OPTION::LOOP);
	m_pArrowL2->AddRenderState("DepthNoWrite");

	m_pArrowL2->AddWorldPos(-48.f, -36.f, 0.f);
	m_pArrowL2->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	m_pArrowL2->SetInheritScale(false);

	pFont->AddChild(m_pArrowL2);

	m_pArrowR2 = m_pExitMenu->CreateComponent<CSpriteComponent>("ArrowR2", m_pScene->FindLayer("UI"));

	m_pArrowR2->SetWorldScale(16.f, 16.f, 0.f);

	m_pArrowR2->SetPivot(0.5f, 0.5f, 0.f);

	m_pArrowR2->CreateSprite("Idle", "ArrowR1", LOOP_OPTION::LOOP);
	m_pArrowR2->AddRenderState("DepthNoWrite");

	m_pArrowR2->AddWorldPos(48.f, -36.f, 0.f);
	m_pArrowR2->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	m_pArrowR2->SetInheritScale(false);

	pFont->AddChild(m_pArrowR2);

	SAFE_RELEASE(pFont);

	m_pStartMenu = m_pScene->CreateObject<CObj>("StartMenu", m_pScene->FindLayer("UI"), m_pScene->GetSceneType());

	pBarL = m_pStartMenu->CreateComponent<CSpriteComponent>("StartBarL", m_pScene->FindLayer("UI"));

	pBarL->SetWorldScale(112.f, 16.f, 0.f);

	pBarL->SetPivot(0.5f, 0.5f, 0.f);

	pBarL->CreateSprite("Idle", "LongBarL", LOOP_OPTION::LOOP);
	pBarL->AddRenderState("DepthNoWrite");

	pBarL->SetWorldPos((float)tRS.iWidth / 2.f - 112.f - 32.f, (float)tRS.iHeight * 23.f / 32.f, 0.f);
	pBarL->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

	pBarR = m_pStartMenu->CreateComponent<CSpriteComponent>("StartBarR", m_pScene->FindLayer("UI"));

	pBarR->SetWorldScale(112.f, 16.f, 0.f);

	pBarR->SetPivot(0.5f, 0.5f, 0.f);

	pBarR->CreateSprite("Idle", "LongBarR", LOOP_OPTION::LOOP);
	pBarR->AddRenderState("DepthNoWrite");
	pBarR->SetInheritScale(false);
	pBarR->AddRelativePos(224.f + 64.f, 0.f, 0.f);
	pBarR->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

	pBarL->AddChild(pBarR);

	m_pStartMenu->SetRootComponent(pBarL);
	m_pStartMenu->Enable(false);

	pFont = m_pExitMenu->CreateComponent<CUIFont>("StartFont", m_pScene->FindLayer("UI"));

	pFont->SetRect(-200.f, 0.f, 200.f, 0.f);
	pFont->SetPivot(0.5f, 0.5f, 0.f);
	pFont->SetText(TEXT("Select a Save Slot"));
	pFont->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	pFont->SetAlignH(TEXT_ALIGN_HORISONTAL::CENTER);
	pFont->SetAlignV(TEXT_ALIGN_VERTICAL::MID);
	pFont->AddRelativePos(112.f + 32.f, 136.f + 16.f + 4.f + 64.f + 16.f, 0.f);

	pBarL->AddChild(pFont);

	m_pArrowL3 = m_pStartMenu->CreateComponent<CSpriteComponent>("ArrowL3", m_pScene->FindLayer("UI"));

	m_pArrowL3->SetWorldScale(16.f, 16.f, 0.f);

	m_pArrowL3->SetPivot(0.5f, 0.5f, 0.f);

	m_pArrowL3->CreateSprite("Idle", "ArrowL2", LOOP_OPTION::LOOP);
	m_pArrowL3->AddRenderState("DepthNoWrite");

	m_pArrowL3->AddWorldPos(-96.f, -24.f - 64.f - 96.f, 0.f);
	m_pArrowL3->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	m_pArrowL3->SetInheritScale(false);
	m_pArrowL3->Enable(false);

	pFont->AddChild(m_pArrowL3);

	m_pArrowR3 = m_pStartMenu->CreateComponent<CSpriteComponent>("ArrowR3", m_pScene->FindLayer("UI"));

	m_pArrowR3->SetWorldScale(16.f, 16.f, 0.f);

	m_pArrowR3->SetPivot(0.5f, 0.5f, 0.f);

	m_pArrowR3->CreateSprite("Idle", "ArrowR2", LOOP_OPTION::LOOP);
	m_pArrowR3->AddRenderState("DepthNoWrite");

	m_pArrowR3->AddWorldPos(96.f, -24.f - 64.f - 96.f, 0.f);
	m_pArrowR3->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	m_pArrowR3->SetInheritScale(false);
	m_pArrowR3->Enable(false);

	pFont->AddChild(m_pArrowR3);

	for (int i = 0; i < 4; ++i)
	{
		SLOTINFO tInfo = GET_SINGLE(CBossManager)->GetInfo(i);

		char strName[32] = {};

		sprintf_s(strName, "SaveSlot%d", i+1);

		CSpriteComponent* pSaveSlot1 = m_pStartMenu->CreateComponent<CSpriteComponent>(strName, m_pScene->FindLayer("UI"));

		pSaveSlot1->SetWorldScale(224.f, 80.f, 0.f);

		pSaveSlot1->SetPivot(0.5f, 0.5f, 0.f);

		pSaveSlot1->CreateSprite("Idle", "Save", LOOP_OPTION::LOOP);
		pSaveSlot1->AddRenderState("DepthNoWrite");
		pSaveSlot1->SetInheritScale(false);
		pSaveSlot1->AddRelativePos(112.f + 32.f, -36.f - 32.f - 64.f * i, 0.f);
		pSaveSlot1->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

		if (i != 0)
		{
			CMaterial* pMtrl = pSaveSlot1->GetMaterial();

			pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.5f);

			SAFE_RELEASE(pMtrl);
		}

		pBarL->AddChild(pSaveSlot1);

		CSpriteComponent* pNum = m_pStartMenu->CreateComponent<CSpriteComponent>("Number");

		pNum->SetWorldScale(48.f, 48.f, 0.f);

		pNum->SetPivot(0.5f, 0.5f, 0.f);

		char strNum[32] = {};

		sprintf_s(strNum, "Number%d", i+1);

		pNum->CreateSprite("Idle", strNum, LOOP_OPTION::LOOP);
		pNum->AddRenderState("DepthNoWrite");
		pNum->SetInheritScale(false);
		pNum->AddRelativePos(-80.f, 0.f, 0.f);
		pNum->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

		if (i != 0)
		{
			CMaterial* pMtrl = pNum->GetMaterial();

			pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.5f);

			SAFE_RELEASE(pMtrl);
		}

		pSaveSlot1->AddChild(pNum);

		SAFE_RELEASE(pNum);

		memset(strName, 0, 32);

		sprintf_s(strName, "Text%d", i + 1);

		CUIFont* pFont = m_pStartMenu->CreateComponent<CUIFont>(strName);

		if (!tInfo.bStart)
		{
			pFont->SetText(TEXT("NEW DATA"));
		}

		else
		{
			TCHAR strStat[256] = {};

			swprintf_s(strStat, TEXT("Kill: %d\nDeath: %d\nPlay Time: %02d:%02d:%02d"), 
				tInfo.iKill, tInfo.iDeath, 
				(int)tInfo.fPlayTime / 3600, 
				(int)tInfo.fPlayTime / 60 - (int)tInfo.fPlayTime / 3600 * 60, 
				(int)tInfo.fPlayTime % 60);

			pFont->SetText(strStat);
		}

		pFont->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
		pFont->SetRect(-200.f, 0.f, 200.f, 0.f);
		pFont->SetAlignH(TEXT_ALIGN_HORISONTAL::CENTER);
		pFont->SetAlignV(TEXT_ALIGN_VERTICAL::MID);
		pFont->AddRelativePos(0.f, 92.f-64.f * i + 80.f, 0.f);

		if (i != 0)
		{
			pFont->SetColor(1.f, 1.f, 1.f, 0.5f);
		}

		pSaveSlot1->AddChild(pFont);

		SAFE_RELEASE(pFont);

		for (int k = 0; k < 2; ++k)
		{
			for (int j = 0; j < 2; ++j)
			{
				char strNum[32] = {};

				sprintf_s(strNum, "Icon%d", k * 2 + j + 1);

				CSpriteComponent* pIcon = m_pStartMenu->CreateComponent<CSpriteComponent>(strNum, m_pScene->FindLayer("UI"));

				pIcon->SetWorldScale(16.f, 16.f, 0.f);
				pIcon->SetPivot(0.5f, 0.5f, 0.f);
				pIcon->CreateSprite("Idle", strNum, LOOP_OPTION::LOOP);
				pIcon->SetInheritScale(false);
				pIcon->AddWorldPos(72.f + (j * 2 - 1) * 8.f, (k * -2 + 1) * 8.f, 0.f);
				pIcon->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
				pIcon->AddRenderState("DepthNoWrite");

				if (i != 0)
				{
					CMaterial* pMtrl = pIcon->GetMaterial();

					if ((int)tInfo.eType & (1 <<( k * 2 + j)))
					{
						pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.5f);
					}

					else
					{
						pMtrl->SetDiffuseColor(0.25f, 0.25f, 0.25f, 0.5f);
					}

					SAFE_RELEASE(pMtrl);
				}

				pSaveSlot1->AddChild(pIcon);

				SAFE_RELEASE(pIcon);
			}
		}

		if (i == 3)
		{
			CSpriteComponent* pDelete = m_pStartMenu->CreateComponent<CSpriteComponent>("Delete", m_pScene->FindLayer("UI"));

			pDelete->CreateSprite("Idle", "Panel", LOOP_OPTION::LOOP);
			pDelete->SetWorldScale(80.f, 16.f, 0.f);
			pDelete->SetPivot(0.5f, 0.5f, 0.f);
			pDelete->AddRelativePos(0.f, -40.f, 0.f);
			pDelete->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
			pDelete->SetInheritScale(false);
			pDelete->AddRenderState("DepthNoWrite");

			CMaterial* pMtrl = pDelete->GetMaterial();

			pMtrl->SetDiffuseColor(0.25f, 0.25f, 0.25f, 1.f);

			SAFE_RELEASE(pMtrl);

			pSaveSlot1->AddChild(pDelete);

			CUIFont* pDeleteText = m_pStartMenu->CreateComponent<CUIFont>("DeleteText", m_pScene->FindLayer("UI"));

			pDeleteText->SetFormat("Normal2");
			pDeleteText->SetText(TEXT("DELETE DATA"));
			pDeleteText->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
			pDeleteText->SetRect(-200.f, 0.f, 200.f, 0.f);
			pDeleteText->SetAlignH(TEXT_ALIGN_HORISONTAL::CENTER);
			pDeleteText->SetAlignV(TEXT_ALIGN_VERTICAL::MID);
			pDeleteText->SetColor(1.f, 1.f, 1.f, 1.f);
			pDeleteText->AddRelativePos(0.f, 96.f - 64.f * 3 - 36.f-8.f + 78.f, 0.f);
			pDeleteText->AddRenderState("DepthNoWrite");

			pDelete->AddChild(pDeleteText);

			SAFE_RELEASE(pDeleteText);

			SAFE_RELEASE(pDelete);
		}

		SAFE_RELEASE(pSaveSlot1);
	}

	SAFE_RELEASE(pBarL);
	SAFE_RELEASE(pBarR);

	SAFE_RELEASE(pFont); 
	
	m_pHelp = m_pScene->CreateObject<CObj>("Help", m_pScene->FindLayer("UI"), m_pScene->GetSceneType());

	CSpriteComponent* pSelectButton = m_pHelp->CreateComponent<CSpriteComponent>("SelectButton", m_pScene->FindLayer("UI"));

	pSelectButton->SetWorldScale(16.f, 16.f, 0.f);

	pSelectButton->SetPivot(0.5f, 0.5f, 0.f);

	pSelectButton->CreateSprite("Idle", "ButtonA", LOOP_OPTION::LOOP);
	pSelectButton->AddRenderState("DepthNoWrite");
	pSelectButton->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

	pSelectButton->SetWorldPos((float)tRS.iWidth / 3.f, (float)tRS.iHeight /3.f, 0.f);

	m_pHelp->SetRootComponent(pSelectButton);

	CUIFont* pSelect = m_pHelp->CreateComponent<CUIFont>("Select", m_pScene->FindLayer("UI"));

	pSelect->SetRect(-200.f, 0.f, 200.f,0.f);
	pSelect->SetPivot(0.5f, 0.5f, 0.f);
	pSelect->SetText(TEXT("Select"));
	pSelect->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	pSelect->SetAlignV(TEXT_ALIGN_VERTICAL::MID);
	pSelect->AddRelativePos(-(float)tRS.iWidth / 21.f, -(float)tRS.iHeight / 6.f, 0.f);

	pSelectButton->AddChild(pSelect);

	SAFE_RELEASE(pSelect);

	CSpriteComponent* pBackButton = m_pHelp->CreateComponent<CSpriteComponent>("BackButton", m_pScene->FindLayer("UI"));

	pBackButton->SetWorldScale(16.f, 16.f, 0.f);

	pBackButton->SetPivot(0.5f, 0.5f, 0.f);

	pBackButton->CreateSprite("Idle", "ButtonB", LOOP_OPTION::LOOP);
	pBackButton->AddRenderState("DepthNoWrite");

	pBackButton->AddRelativePos(0.f, -16.f, 0.f);
	pBackButton->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	pBackButton->SetInheritScale(false);

	pSelectButton->AddChild(pBackButton);

	SAFE_RELEASE(pSelectButton);

	CUIFont* pBack = m_pHelp->CreateComponent<CUIFont>("Back", m_pScene->FindLayer("UI"));

	pBack->SetRect(-200.f, 0.f, 200.f, 0.f);
	pBack->SetPivot(0.5f, 0.5f, 0.f);
	pBack->SetText(TEXT("Back"));
	pBack->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	pBack->SetAlignV(TEXT_ALIGN_VERTICAL::MID);
	pBack->AddRelativePos(-(float)tRS.iWidth / 21.f, -(float)tRS.iHeight / 6.f - 10.f, 0.f);

	pBackButton->AddChild(pBack);

	SAFE_RELEASE(pBack);

	SAFE_RELEASE(pBackButton);

	m_pHelp->Enable(false);

	UpdateSlot();

	return true;
}

bool CStartGameMode::LoadResources()
{
	GET_SINGLE(CResourceManager)->LoadTexture("title", TEXT("IMG\\TITLE\\title.png"), DATA_PATH);

	GET_SINGLE(CResourceManager)->LoadTexture("an", TEXT("IMG\\TITLE\\an.png"), DATA_PATH);

	GET_SINGLE(CResourceManager)->LoadTexture("devolver", TEXT("IMG\\TITLE\\devolver.png"), DATA_PATH);

	GET_SINGLE(CResourceManager)->LoadTexture("fontsmall", TEXT("IMG\\TITLE\\fontsmall.png"), DATA_PATH);

	GET_SINGLE(CResourceManager)->LoadTexture("gamepad", TEXT("IMG\\TITLE\\gamepad.png"), DATA_PATH);

	return true;
}

void CStartGameMode::MainGameMode(float)
{
	GET_SINGLE(CSceneManager)->CreateNextScene();

	GET_SINGLE(CSceneManager)->SetGameMode<CMainGameMode>(false);
}

void CStartGameMode::ExitGame(float)
{
	GET_SINGLE(CEngine)->ExitGame();
}

bool CStartGameMode::LoadSequance(const char* pFileName, const std::string& strPathKey)
{
	char strFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultibytePath(strPathKey);

	if (pPath)
		strcpy_s(strFullPath, pPath);

	strcat_s(strFullPath, pFileName);

	FILE* pFile = nullptr;

	fopen_s(&pFile, strFullPath, "rt");

	if (pFile)
	{
		char cLine[1024] = {};

		fgets(cLine, 1024, pFile);

		while (true)
		{
			char cLine[1024] = {};

			fgets(cLine, 1024, pFile);

			if (strcmp(cLine, "") == 0)
				break;

			char* pContext = nullptr;

			char* pResult = strtok_s(cLine, ", ", &pContext);

			char* pTex = strtok_s(nullptr, ", ", &pContext);

			char* pTime = strtok_s(nullptr, ", ", &pContext);

			float fMaxTime = (float)atof(pTime);

			float fRate = 0.f;

			if (pContext)
			{
				fRate = (float)atof(pContext);
			}

			GET_SINGLE(CResourceManager)->CreateAni2DSeq(pResult, pTex, fMaxTime, fRate);
		}

		fclose(pFile);
	}

	return true;
}

bool CStartGameMode::LoadFrame(const char* pFileName, const std::string& strPathKey)
{
	char strFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultibytePath(strPathKey);

	if (pPath)
		strcpy_s(strFullPath, pPath);

	strcat_s(strFullPath, pFileName);

	FILE* pFile = nullptr;

	fopen_s(&pFile, strFullPath, "rt");

	if (pFile)
	{
		char cLine[1024] = {};

		fgets(cLine, 1024, pFile);

		while (true)
		{
			char cLine[1024] = {};

			fgets(cLine, 1024, pFile);

			if (strcmp(cLine, "") == 0)
				break;

			char* pContext = nullptr;

			char* pResult = strtok_s(cLine, ", ", &pContext);

			char* pStartX = strtok_s(nullptr, ", ", &pContext);

			char* pStartY = strtok_s(nullptr, ", ", &pContext);

			char* pEndX = strtok_s(nullptr, ", ", &pContext);

			char* pEndY = strtok_s(nullptr, ", ", &pContext);

			Vector2 vStart = Vector2((float)atof(pStartX), (float)atof(pStartY));
			Vector2 vEnd = Vector2((float)atof(pEndX), (float)atof(pEndY));

			GET_SINGLE(CResourceManager)->AddFrame(pResult, vStart, vEnd);
		}

		fclose(pFile);
	}

	return true;
}

void CStartGameMode::DownX(float fTime)
{
	if (m_iStage < (int)Stage::TITLE)
	{
		m_fTime = 10.f;
		m_bFadeIn = false;
		m_bStay = false;
	}

	else if (m_iStage == (int)Stage::TITLE)
	{
		if (m_iDepth == 0)
		{
			if (m_iSelect == 0)
			{
				StartMenu();

				m_iMenu = 0;

				UpdateSlot();
			}

			else if (m_iSelect == 1)
			{
				Option();
				m_iMenu = 1;
			}

			else
			{
				ExitMenu();
				m_iMenu = 2;
			}	

			m_iDepth++;
			m_iSelect = 0;
		}

		else if (m_iDepth == 1)
		{
			if (m_iMenu == 0)
			{
				if (m_iSelect < 4)
				{
					if (!m_bDelete)
					{
						m_pBGM->SetSound("Forest");
						m_pBGM->Stop();

						m_pEffect->SetSound("Opening");
						m_pEffect->Stop();

						m_pBGM->SetFade(true, false);

						SetFade(0.f, 1.f, 1.f);

						SLOTINFO tInfo = GET_SINGLE(CBossManager)->GetInfo(m_iSelect);

						GET_SINGLE(CBossManager)->SetSlot(m_iSelect);

						/*if (tInfo.bStart)
						{
							GET_SINGLE(CBossManager)->Load();
						}*/

						MainGameMode(fTime);
					}

					else
					{
						GET_SINGLE(CBossManager)->DeleteSlot(m_iSelect);

						UpdateSlot();

						GET_SINGLE(CBossManager)->SaveSlotInfo();
					}
				}

				else if (m_iSelect == 4)
				{
					m_bDelete ^= true;

					CMaterial* pMtrl = m_pArrowL3->GetMaterial();

					if (m_bDelete)
					{
						pMtrl->SetDiffuseColor(1.f, 0.f, 0.f, 1.f);
					}

					else
					{
						pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 1.f);
					}

					SAFE_RELEASE(pMtrl);

					pMtrl = m_pArrowR3->GetMaterial();

					if (m_bDelete)
					{
						pMtrl->SetDiffuseColor(1.f, 0.f, 0.f, 1.f);
					}

					else
					{
						pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 1.f);
					}

					SAFE_RELEASE(pMtrl);

				}
			}

			else if (m_iMenu == 2)
			{
				if (m_iSelect == 0)
				{
					GET_SINGLE(CEngine)->ExitGame();
				}

				else if (m_iSelect == 1)
				{
					m_iSelect = 2;
					m_iDepth = 0;
					m_pTitle->Enable(true);
					m_pExitMenu->Enable(false);
				}
			}
		}
	}
}

void CStartGameMode::DownC(float fTime)
{
	if (m_iDepth > 0)
	{
		m_iDepth--;

		if (m_iDepth == 0)
		{
			if (m_iMenu == 0)
			{
				if (!m_bDelete)
				{
					m_pStartMenu->Enable(false);
					m_pTitle->Enable(true);
					m_iSelect = m_iMenu;
				}

				else
				{
					m_bDelete = false;

					m_pArrowL3->Enable(false);
					m_pArrowR3->Enable(false);

					CMaterial* pMtrl = m_pArrowL3->GetMaterial();
						pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 1.f);
					

					SAFE_RELEASE(pMtrl);

					pMtrl = m_pArrowR3->GetMaterial();

						pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 1.f);

					SAFE_RELEASE(pMtrl);

					++m_iDepth;
				}
			}

			else if (m_iMenu == 1)
			{
				m_pTitle->Enable(true);
				m_iSelect = m_iMenu;
			}

			else if (m_iMenu == 2)
			{
				m_pExitMenu->Enable(false);
				m_pTitle->Enable(true);
				m_iSelect = m_iMenu;
			}

			m_iMenu = 0;
		}

		m_pEffect->SetSound("Move");
		m_pEffect->Play(0.f);
	}
}

void CStartGameMode::DownUpArrow(float fTime)
{
	m_pEffect->SetSound("Move");
	m_pEffect->Play(0.f);

	if (m_iDepth == 0)
	{
		if (m_iSelect > 0)
		{
			m_pArrowL->AddRelativePos(0.f, 16.f, 0.f);
			m_pArrowR->AddRelativePos(0.f, 16.f, 0.f);

			m_iSelect--;
		}
	}

	else if (m_iDepth == 1)
	{
		if (m_iMenu == 0)
		{
			m_iSelect = (m_iSelect + 4) % 5;
			UpdateSlot();
		}

		else if (m_iMenu == 2)
		{
			if (m_iSelect > 0)
			{
				m_pArrowL2->AddRelativePos(0.f, 16.f, 0.f);
				m_pArrowR2->AddRelativePos(0.f, 16.f, 0.f);

				m_iSelect--;
			}
		}
	}
}

void CStartGameMode::DownDownArrow(float fTime)
{
	m_pEffect->SetSound("Move");
	m_pEffect->Play(0.f);
	if (m_iDepth == 0)
	{
		if (m_iSelect <2)
		{
			m_pArrowL->AddRelativePos(0.f, -16.f, 0.f);
			m_pArrowR->AddRelativePos(0.f, -16.f, 0.f);

			m_iSelect++;
		}
	}

	else if (m_iDepth == 1)
	{
		if (m_iMenu == 0)
		{
			m_iSelect = (m_iSelect + 6) % 5;
			UpdateSlot();
		}

		else if (m_iMenu == 2)
		{
			if (m_iSelect <1)
			{
				m_pArrowL2->AddRelativePos(0.f, -16.f, 0.f);
				m_pArrowR2->AddRelativePos(0.f, -16.f, 0.f);

				m_iSelect++;
			}
		}
	}
}

void CStartGameMode::Option()
{
	m_pTitle->Enable(false);
}

void CStartGameMode::ExitMenu()
{
	m_pTitle->Enable(false);
	m_pExitMenu->Enable(true);

	m_pArrowL2->SetRelativePos(-48.f, -36.f, 0.f);
	m_pArrowR2->SetRelativePos(48.f, -36.f, 0.f);
}

void CStartGameMode::StartMenu()
{
	m_pTitle->Enable(false);
	m_pStartMenu->Enable(true);

	m_pArrowL3->SetRelativePos(128.f, -172.f - 64.f, 0.f);
	m_pArrowR3->SetRelativePos(-128.f, -172.f - 64.f, 0.f);
}

void CStartGameMode::UpdateSlot()
{
	for (int i = 0; i < 4; ++i)
	{
		SLOTINFO tInfo = GET_SINGLE(CBossManager)->GetInfo(i);

		char strText[32] = {};

		sprintf_s(strText, "Text%d", i+1);

		CUIFont* pCom = (CUIFont*)m_pStartMenu->FindSceneComponent(strText);

		TCHAR strStat[256] = {};

		if (tInfo.bStart)
		{
			swprintf_s(strStat, TEXT("Kill: %d\nDeath: %d\nPlay Time: %02d:%02d:%02d"),
				tInfo.iKill, tInfo.iDeath,
				(int)tInfo.fPlayTime / 3600,
				(int)tInfo.fPlayTime / 60 - (int)tInfo.fPlayTime / 3600 * 60,
				(int)tInfo.fPlayTime % 60);
		}

		else
		{
			swprintf_s(strStat, TEXT("NEW DATA"));
		}

		pCom->SetText(strStat);

		//pCom->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
		//pCom->SetRect(-200.f, 0.f, 200.f, 0.f);
		pCom->SetAlignH(TEXT_ALIGN_HORISONTAL::CENTER);
		pCom->SetAlignV(TEXT_ALIGN_VERTICAL::MID);

		if (pCom)
		{
			if (m_iSelect == i)
			{
				pCom->SetColor(1.f, 1.f, 1.f, 1.f);
			}

			else
			{
				pCom->SetColor(1.f, 1.f, 1.f, 0.5f);
			}

			pCom->Release();
		}

		char strSlot[32] = {};

		sprintf_s(strSlot, "SaveSlot%d", i+1);

		CSceneComponent* __pCom = m_pStartMenu->FindSceneComponent(strSlot);

		if (__pCom)
		{
			for (int j = 0; j < 2; ++j)
			{
				for (int k = 0; k < 2; ++k)
				{
					char strCom[32] = {};

					sprintf_s(strCom, "Icon%d", j * 2 + k + 1);

					CSceneComponent* _pCom = __pCom->FindComponent(strCom);

					if (_pCom)
					{
						CMaterial* pMtrl = _pCom->GetMaterial();

						if ((int)tInfo.eType & (1 << (k * 2 + j)))
						{
							if (m_iSelect == i)
							{
								pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 1.f);
							}

							else
							{
								pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.5f);
							}
						}

						else
						{
							if (m_iSelect == i)
							{
								pMtrl->SetDiffuseColor(0.25f, 0.25f, 0.25f, 1.f);
							}

							else
							{
								pMtrl->SetDiffuseColor(0.25f, 0.25f, 0.25f, 0.5f);
							}
						}

						SAFE_RELEASE(pMtrl);

						_pCom->Release();
					}
				}
			}

			CSceneComponent* _pCom = __pCom->FindComponent("Number");

			if (_pCom)
			{
				CMaterial* pMtrl = _pCom->GetMaterial();

				if (m_iSelect == i)
				{
					pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 1.f );
				}

				else
				{
					pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.5f );
				}

				SAFE_RELEASE(pMtrl);

				_pCom->Release();
			}

			CMaterial* pMtrl = __pCom->GetMaterial();

			if (m_iSelect == i)
			{
				pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 1.f);
			}

			else
			{
				pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.5f );
			}

			SAFE_RELEASE(pMtrl);

			__pCom->Release();
		}
	}

	m_pArrowL3->SetRelativePos(128.f, -64.f * m_iSelect - 172.f - 52.f - 80.f, 0.f);
	m_pArrowR3->SetRelativePos(-128.f, -64.f * m_iSelect - 172.f - 52.f - 80.f, 0.f);

	if (m_iSelect == 4)
	{
		m_pArrowL3->AddRelativePos(-80.f, 24.f, 0.f);
		m_pArrowR3->AddRelativePos(80.f, 24.f, 0.f);

		m_pArrowL3->Enable(true);
		m_pArrowR3->Enable(true);

		CSceneComponent* pCom = m_pStartMenu->FindSceneComponent("Delete");

		CMaterial* pMtrl = pCom->GetMaterial();

		pMtrl->SetDiffuseColor(0.5f, 0.5f, 0.5f, 1.f);

		SAFE_RELEASE(pMtrl);

		SAFE_RELEASE(pCom);
	}

	else
	{
		if (!m_bDelete)
		{
			m_pArrowL3->Enable(false);
			m_pArrowR3->Enable(false);
		}

		CSceneComponent* pCom = m_pStartMenu->FindSceneComponent("Delete");

		CMaterial* pMtrl = pCom->GetMaterial();

		pMtrl->SetDiffuseColor(0.25f, 0.25f, 0.25f, 1.f);

		SAFE_RELEASE(pMtrl);

		SAFE_RELEASE(pCom);
	}
}
