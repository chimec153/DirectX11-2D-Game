#include "MainGameMode.h"
#include "../Object/Player.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Texture.h"
#include "../Object/Minion.h"
#include "../Object/Bullet.h"
#include "../Object/IceBall.h"
#include "../UI/Button.h"
#include "Resource/ResourceManager.h"
#include "../Object/Effect.h"
#include "../UI/UIBar.h"
#include "SoundManager.h"
#include "../UI/CheckObj.h"
#include "../UI/RadioObj.h"
#include "UI/Slider.h"
#include "SoundManager.h"
#include "Scene/SceneManager.h"
#include "StartGameMode.h"
#include "UI/UIButton.h"
#include "Tile/TileMap.h"
#include "../UI/Missile.h"
#include "../../tinyxml.h"
#include "PathManager.h"
#include "Component/Sound.h"
#include "Camera/CameraManager.h"
#include "Component/Camera.h"
#include "Component/ColliderRect.h"
#include "ColossusMode.h"
#include "Component/SpriteComponent.h"
#include "Resource/Material.h"
#include "SoundManager.h"
#include "PathManager.h"
#include "Resource/ShaderManager.h"
#include "../../Custom.h"
#include "Resource/GraphicShader.h"
#include "Resource/ParticleSystem.h"
#include "../Object/Soul.h"
#include "Device.h"
#include "Component/Mesh2DComponent.h"
#include "../BossManager.h"
#include "../Object/SoulMonster.h"
#include "../Object/Bow.h"
#include "../Object/Arrow.h"
#include "Input.h"

CMainGameMode::CMainGameMode()	:
	m_bOpen(false)
	, m_pEft(nullptr)
{
	m_iGameModeClassType = (int)GAMEMODE_CLASS_TYPE::GCT_MAIN;
}

CMainGameMode::~CMainGameMode()
{
	SAFE_RELEASE(m_pEft);
}

bool CMainGameMode::Init()
{
	GET_SINGLE(CSoundManager)->Stop("Hub1");

	int iSlot = GET_SINGLE(CBossManager)->GetSlot();

	SLOTINFO tInfo = GET_SINGLE(CBossManager)->GetInfo(iSlot);

	static bool bInit = false;

	if (!bInit)
	{
		bInit = true;
		LoadSound();

		LoadResource();

		LoadSequance("data\\Sequances.csv");

		LoadFrame("data\\Frames.csv");

		LoadParticle();

		GET_SINGLE(CInput)->AddActionKey("0", DIK_0);
		GET_SINGLE(CInput)->AddActionBind("0", KEY_TYPE::KT_DOWN, SetImgui);
	}

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	pCam->SetRect(0.3375f, 0.6625f, 0.3375f, 0.6625f);
	pCam->SetCallBack<CMainGameMode>(this, &CMainGameMode::Open);

	SAFE_RELEASE(pCam);

	CLayer* pLayer = m_pScene->FindLayer("Default");

	pLayer->Set2D();

	CPlayer* pPlayer = m_pScene->CreateObject<CPlayer>("player", pLayer);

	SetPlayer(pPlayer);

	CTileMap::ClearAnim();

	LoadXml("IMG\\jam.png.xml");

	LoadXml("MAPS\\JAM\\JAM.tmx");

	pPlayer->SetWorldPos(1376.f, 160.f * 16.f - 1359.f, 0.f);

	CBow* pBow = m_pScene->CreateObject<CBow>("Bow", pLayer);

	CArrow* pArrow = m_pScene->CreateObject<CArrow>("Arrow", pLayer);

	pPlayer->SetBow(pBow);
	pPlayer->SetArrow(pArrow);

	SAFE_RELEASE(pBow);
	SAFE_RELEASE(pArrow);

	SAFE_RELEASE(pPlayer);	

	pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	pCam->SetWorldPos(1376.f + 8.f, 160.f * 16.f - 1359.f + 8.f, 0.f);

	SAFE_RELEASE(pCam);

	CLayer* pBack = m_pScene->FindLayer("Back");

	CBullet* pBullet = CScene::CreateProtoObj<CBullet>("Bullet", m_pScene, m_pScene->GetSceneType());

	SAFE_RELEASE(pBullet);

	CEffect* pEft = CScene::CreateProtoObj<CEffect>("Fire", m_pScene, m_pScene->GetSceneType());

	CSpriteComponent* pFire = pEft->CreateComponent<CSpriteComponent>("Fire");

	pFire->CreateSprite("Fire", "Fire", LOOP_OPTION::LOOP);
	pFire->SetPivot(0.5f, 0.5f, 0.f);
	pFire->SetWorldScale(16.f, 16.f, 1.f);

	pEft->SetRootComponent(pFire);

	SAFE_RELEASE(pFire);

	SAFE_RELEASE(pEft);

	CEffect* pChargeEffect = CScene::CreateProtoObj<CEffect>("Charge", m_pScene, m_pScene->GetSceneType());
	
	CSpriteComponent* pCharge = pChargeEffect->CreateComponent<CSpriteComponent>("Charge", m_pScene->FindLayer("Default"));

	pCharge->CreateSprite("Charge", "Charge", LOOP_OPTION::ONCE_DESTROY);

	pCharge->SetPivot(0.5f, 0.5f, 0.f);
	pCharge->SetRelativeScale(16.f, 16.f, 1.f);
	pCharge->AddNotify("Charge", "FireEnd", 1.f);

	pChargeEffect->SetRootComponent(pCharge);
	pChargeEffect->SetSpeed(0.f);

	SAFE_RELEASE(pCharge);

	SAFE_RELEASE(pChargeEffect);

	CEffect* pWaterEffect = CScene::CreateProtoObj<CEffect>("Water", m_pScene, m_pScene->GetSceneType());

	CSpriteComponent* pWater = pWaterEffect->CreateComponent<CSpriteComponent>("Water");

	pWater->CreateSprite("Water", "Water", LOOP_OPTION::LOOP);

	pWater->SetPivot(0.5f, 0.5f, 0.f);
	pWater->SetRelativeScale(32.f, 16.f, 1.f);
	pWater->AddNotify("Water", "Water", 1.f);
	pWater->AddCallBack<CEffect>("Water", "Water", pWaterEffect, &CEffect::Destroy);
	pWater->AddRenderState("AlphaBlend");

	CMaterial* pMtrl = pWater->GetMaterial();

	pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.3f);

	SAFE_RELEASE(pMtrl);

	pWaterEffect->SetRootComponent(pWater);
	pWaterEffect->SetSpeed(0.f);
	pWaterEffect->SetScaleSpeed(Vector3(16.f, 8.f, 0.f));

	SAFE_RELEASE(pWater);

	SAFE_RELEASE(pWaterEffect);

	CMissile* pMissile = CScene::CreateProtoObj<CMissile>("Missile", m_pScene, m_pScene->GetSceneType());

	SAFE_RELEASE(pMissile);

	CreateUI();

	CEffect* pBossEffect = m_pScene->CreateObject<CEffect>("BossEffect", 
		m_pScene->FindLayer("Ground"), m_pScene->GetSceneType());

	CSpriteComponent* pBossCom = pBossEffect->CreateComponent<CSpriteComponent>("BossEffect", 
		m_pScene->FindLayer("Ground"));

	pBossCom->SetWorldScale(48.f, 48.f, 0.f);
	pBossCom->SetPivot(0.5f, 0.5f, 0.f);

	pBossCom->CreateSprite("Effect2", "BossEffect1", LOOP_OPTION::LOOP);
	pBossCom->AddRenderState("DepthNoWrite");

	pBossEffect->SetRootComponent(pBossCom);

	CColliderRect* pRC = pBossEffect->CreateComponent<CColliderRect>("SavePointBody", m_pScene->FindLayer("Default"));

	pRC->SetExtent(48.f, 48.f);
	pRC->SetCallBack(COLLISION_STATE::INIT, this, &CMainGameMode::Col);

	pBossCom->AddChild(pRC);

	SAFE_RELEASE(pRC);

	CSpriteComponent* pColEft = pBossEffect->CreateComponent<CSpriteComponent>("ColEffect", m_pScene->FindLayer("Ground"));

	pColEft->SetWorldScale(16.f, 16.f, 0.f);
	pColEft->SetPivot(0.5f, 0.5f, 0.f);

	pColEft->CreateSprite("Effect2", "BossCircle", LOOP_OPTION::LOOP);
	pColEft->AddRelativePos(16.f, 0.f, 0.f);
	pColEft->SetInheritScale(false);
	pColEft->AddRenderState("DepthNoWrite");

	if (!((int)tInfo.eType & (int)BOSS_TYPE::COLOSSUS))
	{
		pColEft->Enable(false);
	}

	pBossCom->AddChild(pColEft);

	SAFE_RELEASE(pColEft);

	pColEft = pBossEffect->CreateComponent<CSpriteComponent>("EyeEffect", m_pScene->FindLayer("Ground"));

	pColEft->SetWorldScale(16.f, 16.f, 0.f);
	pColEft->SetPivot(0.5f, 0.5f, 0.f);

	pColEft->CreateSprite("Effect2", "BossCircle", LOOP_OPTION::LOOP);
	pColEft->AddRelativePos(-16.f, 0.f, 0.f);
	pColEft->SetInheritScale(false);
	pColEft->AddRenderState("DepthNoWrite");

	if (!((int)tInfo.eType & (int)BOSS_TYPE::EYECUBE))
	{
		pColEft->Enable(false);
	}

	pBossCom->AddChild(pColEft);

	SAFE_RELEASE(pColEft);

	pColEft = pBossEffect->CreateComponent<CSpriteComponent>("BrainEffect", m_pScene->FindLayer("Ground"));

	pColEft->SetWorldScale(16.f, 16.f, 0.f);
	pColEft->SetPivot(0.5f, 0.5f, 0.f);

	pColEft->CreateSprite("Effect2", "BossCircle", LOOP_OPTION::LOOP);
	pColEft->AddRelativePos(0.f, -16.f, 0.f);
	pColEft->SetInheritScale(false);
	pColEft->AddRenderState("DepthNoWrite");

	if (!((int)tInfo.eType & (int)BOSS_TYPE::BRAINFREEZE))
	{
		pColEft->Enable(false);
	}

	pBossCom->AddChild(pColEft);

	SAFE_RELEASE(pColEft);

	SAFE_RELEASE(pBossCom);

	pBossEffect->SetWorldPos(1376.f, 2552.f - 1351.f, 0.f);
	pBossEffect->SetSpeed(0.f);
	pBossEffect->SetAlphaSpeed(0.f);

	SAFE_RELEASE(pBossEffect);

	m_pFadeObj = m_pScene->CreateObject<CObj>("Fade", m_pScene->FindLayer("Fore"));

	m_pFade = m_pFadeObj->CreateComponent<CMesh2DComponent>("FadeCom", m_pScene->FindLayer("Speacial"));

	Resolution tRS = RESOLUTION;

	m_pFade->SetWorldScale((float)tRS.iWidth, (float)tRS.iHeight, 0.f);
	m_pFade->SetPivot(0.5f, 0.5f, 0.f);
	m_pFade->SetTexture(REGISTER_TYPE::RT_DIF, "util");
	m_pFade->SetWorldPos(0.f, 0.f, -520.f);
	m_pFade->AddRenderState("DepthNoWrite");

	CMaterial* pFadeMtrl = m_pFade->GetMaterial();

	pFadeMtrl->SetDiffuseColor(0.f, 0.f, 0.f, 0.f);

	SAFE_RELEASE(pFadeMtrl);

	m_pFadeObj->SetRootComponent(m_pFade);

	CObj* pDoor = m_pScene->CreateObject<CObj>("Door", m_pScene->FindLayer("Default"));

	CSpriteComponent* pCom = pDoor->CreateComponent<CSpriteComponent>("DoorL", m_pScene->FindLayer("Temp"));

	pCom->CreateSprite("Idle", "MegaDoorL", LOOP_OPTION::LOOP);
	pCom->SetWorldScale(88.f, 224.f, 0.f);
	pCom->SetPivot(0.5f, 0.5f, 0.f);
	pCom->SetWorldPos(1332.f, 2360.f, 0.f);
	pCom->AddRenderState("DepthNoWrite");	

	CColliderRect* pLRC = pDoor->CreateComponent<CColliderRect>("DoorLBody", m_pScene->FindLayer("Temp"));

	pLRC->SetExtent(88.f, 224.f);

	pCom->AddChild(pLRC);

	SAFE_RELEASE(pLRC);

	pDoor->SetRootComponent(pCom);

	CSpriteComponent* pComR = pDoor->CreateComponent<CSpriteComponent>("DoorR", m_pScene->FindLayer("Temp"));

	pComR->CreateSprite("Idle", "MegaDoorR", LOOP_OPTION::LOOP);
	pComR->SetWorldScale(88.f, 224.f, 0.f);
	pComR->SetPivot(0.5f, 0.5f, 0.f);
	pComR->AddRelativePos(88.f, 0.f, 0.f);
	pComR->SetInheritScale(false);
	pComR->AddRenderState("DepthNoWrite");

	pCom->AddChild(pComR);

	CColliderRect* pRRC = pDoor->CreateComponent<CColliderRect>("DoorRBody", m_pScene->FindLayer("Temp"));

	pRRC->SetExtent(88.f, 224.f);

	pComR->AddChild(pRRC);

	SAFE_RELEASE(pRRC);

	CSpriteComponent* pLight = pDoor->CreateComponent<CSpriteComponent>("Light", m_pScene->FindLayer("Light"));

	pLight->CreateSprite("Idle", "MegaDoorLight", LOOP_OPTION::LOOP);
	pLight->SetWorldScale(176.f, 224.f, 0.f);
	pLight->SetPivot(0.5f, 0.5f, 0.f);
	pLight->AddRelativePos(44.f, 0.f, 0.f);
	pLight->SetInheritScale(false);
	pLight->AddRenderState("DepthNoWrite");

	pMtrl = pLight->GetMaterial();

	pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.f);

	SAFE_RELEASE(pMtrl);

	pCom->AddChild(pLight);

	SAFE_RELEASE(pLight);
	SAFE_RELEASE(pCom);
	SAFE_RELEASE(pComR);
	SAFE_RELEASE(pDoor);

	m_pEft = m_pFadeObj->CreateComponent<CSound>("Effect", m_pScene->FindLayer("Default"));

	m_pEft->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

	m_pFade->AddChild(m_pEft);

	return true;
}

void CMainGameMode::Update(float fTime)
{
	CTileMode::Update(fTime);

	if (m_bOpen)
	{
		static bool bIncrease = true;
		static bool bDoor = false;

		CObj* pObj = m_pScene->FindLayer("Default")->FindObj("Door");

		CSceneComponent* pCom = pObj->FindSceneComponent("Light");

		CMaterial* pMtrl = pCom->GetMaterial();

		Vector4 vDif = pMtrl->GetDif();

		if (!bDoor)
		{
			if (bIncrease)
			{
				vDif.w += fTime / 2.f;

				if (vDif.w >= 1.f)
				{
					vDif.w = 1.f;
					bIncrease = false;
				}
			}

			else
			{
				vDif.w -= fTime / 2.f;

				if (vDif.w <= 0.f)
				{
					vDif.w = 0.f;
					bDoor = true;

					ShakeCam(150.f, 5.f, 10.f);
				}
			}

			pMtrl->SetDiffuseColor(vDif);
		}

		else
		{
			static float fDist = 0.f;

			CSceneComponent* pComL = pObj->FindSceneComponent("DoorL");
			CSceneComponent* pComR = pObj->FindSceneComponent("DoorR");

			pComL->AddWorldPos(-8.f * fTime, 0.f, 0.f);

			pComR->AddRelativePos(16.f * fTime, 0.f, 0.f);

			fDist += fTime * 8.f;

			if (fDist >= 11.f * 8.f)
			{
				bDoor = false;
				m_bOpen = false;
			}

			SAFE_RELEASE(pComL);
			SAFE_RELEASE(pComR);
		}

		SAFE_RELEASE(pMtrl);

		SAFE_RELEASE(pCom);

		SAFE_RELEASE(pObj);
	}
}

void CMainGameMode::LoadResource()
{
	if (!GET_SINGLE(CResourceManager)->LoadTexture("Pro", TEXT("player.png")))
		return;

	GET_SINGLE(CSoundManager)->LoadSound("BtnPress", SOUND_TYPE::EFFECT, "UI\\Select.mp3");

	GET_SINGLE(CResourceManager)->LoadTexture("Bow", TEXT("Bow.png"));

	GET_SINGLE(CResourceManager)->LoadTexture("Arrow", TEXT("Arrow.png"));

	GET_SINGLE(CResourceManager)->LoadTexture("Effect", TEXT("Particles.png"));

	GET_SINGLE(CResourceManager)->LoadTexture("UnderWorld", TEXT("Underworld.png"));
	GET_SINGLE(CResourceManager)->LoadTexture("Overworld", TEXT("Overworld.png"));

	GET_SINGLE(CResourceManager)->LoadTexture("colossus", TEXT("BOSS\\COLOSSUS\\colossus.png"));
	GET_SINGLE(CResourceManager)->LoadTexture("eyecube", TEXT("BOSS\\EYECUBE\\eyecube.png"));
	GET_SINGLE(CResourceManager)->LoadTexture("brain", TEXT("BOSS\\BRAIN\\brainfreeze.png"));
	GET_SINGLE(CResourceManager)->LoadTexture("jam", TEXT("jam.png"));

	CTexture* pTex = GET_SINGLE(CResourceManager)->FindTexture("Pro");
	CTexture* pBowTex = GET_SINGLE(CResourceManager)->FindTexture("Bow");
	CTexture* pArrowTex = GET_SINGLE(CResourceManager)->FindTexture("Arrow");
	CTexture* pEftTex = GET_SINGLE(CResourceManager)->FindTexture("Effect");

	CResourceManager* pRes = GET_SINGLE(CResourceManager);

	pRes->CreateAni2DSeq("Arrow", pTex);

	pRes->AddFrame("Arrow", Vector2(16.f * 31.f, 0.f), Vector2(16.f * 32.f, 16.f));

	pRes->CreateAni2DSeq("ArrowStop", pTex);

	pRes->AddFrame("ArrowStop", Vector2(16.f * 30.f, 16.f), Vector2(16.f * 31.f, 32.f));

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("Fire", pEftTex);

	for (int i = 0; i < 9; ++i)
		GET_SINGLE(CResourceManager)->AddFrame("Fire", Vector2(i * 16.f, 176.f), Vector2((i+1) * 16.f, 191.f));

	GET_SINGLE(CResourceManager)->AddNotify("Fire", "FireEnd", 8);

	SAFE_RELEASE(pTex);
	SAFE_RELEASE(pBowTex);
	SAFE_RELEASE(pArrowTex);
	SAFE_RELEASE(pEftTex);

	//pRes->LoadTexture("Yeti", TEXT("Yeti.png"));

	//CTexture* pYetiTex = pRes->FindTexture("Yeti");

	//pRes->CreateAni2DSeq("YetiIdleR", pYetiTex, 0.3f);
	//for (int i = 0; i < 3; ++i)
	//	pRes->AddFrame("YetiIdleR", Vector2(96.f * i, 0.f), Vector2(96.f * (1 + i), 96.f));

	//pRes->CreateAni2DSeq("YetiAttackR", pYetiTex, 0.2f);
	//for (int i = 0; i < 3; ++i)
	//	pRes->AddFrame("YetiAttackR", Vector2(96.f * (i + 3), 0.f), Vector2(96.f * (4 + i), 96.f));

	//pRes->CreateAni2DSeq("YetiRollR", pYetiTex, 0.3f);
	//for (int i = 0; i < 3; ++i)
	//	pRes->AddFrame("YetiRollR", Vector2(96.f * (6.f + i), 0.f), Vector2(96.f * (7.f + i), 96.f));


	//pRes->CreateAni2DSeq("YetiIdleU", pYetiTex, 0.3f);
	//for (int i = 0; i < 3; ++i)
	//	pRes->AddFrame("YetiIdleU", Vector2(96.f * i, 96.f), Vector2(96.f * (1 + i), 96.f * 2.f));

	//pRes->CreateAni2DSeq("YetiAttackU", pYetiTex, 0.2f);
	//for (int i = 0; i < 3; ++i)
	//	pRes->AddFrame("YetiAttackU", Vector2(96.f * (i + 3), 96.f), Vector2(96.f * (4 + i), 96.f * 2.f));

	//pRes->CreateAni2DSeq("YetiRollU", pYetiTex, 0.3f);
	//for (int i = 0; i < 3; ++i)
	//	pRes->AddFrame("YetiRollU", Vector2(96.f * (6.f + i), 96.f), Vector2(96.f * (7.f + i), 96.f * 2.f));


	//pRes->CreateAni2DSeq("YetiIdleD", pYetiTex, 0.3f);
	//for (int i = 0; i < 3; ++i)
	//	pRes->AddFrame("YetiIdleD", Vector2(96.f * i, 96.f * 2.f), Vector2(96.f * (1 + i), 96.f * 3.f));

	//pRes->CreateAni2DSeq("YetiAttackD", pYetiTex, 0.2f);
	//for (int i = 0; i < 3; ++i)
	//	pRes->AddFrame("YetiAttackD", Vector2(96.f * (i + 3), 96.f * 2.f), Vector2(96.f * (4 + i), 96.f * 3.f));

	//pRes->CreateAni2DSeq("YetiRollD", pYetiTex, 0.3f);
	//for (int i = 0; i < 3; ++i)
	//	pRes->AddFrame("YetiRollD", Vector2(96.f * (6.f + i), 96.f * 2.f), Vector2(96.f * (7.f + i), 96.f * 3.f));


	//pRes->CreateAni2DSeq("YetiIdleRU", pYetiTex, 0.3f);
	//for (int i = 0; i < 3; ++i)
	//	pRes->AddFrame("YetiIdleRU", Vector2(96.f * i, 96.f * 3.f), Vector2(96.f * (1 + i), 96.f * 4.f));

	//pRes->CreateAni2DSeq("YetiAttackRU", pYetiTex, 0.2f);
	//for (int i = 0; i < 3; ++i)
	//	pRes->AddFrame("YetiAttackRU", Vector2(96.f * (i + 3), 96.f * 3.f), Vector2(96.f * (4 + i), 96.f * 4.f));

	//pRes->CreateAni2DSeq("YetiRollRU", pYetiTex, 0.3f);
	//for (int i = 0; i < 3; ++i)
	//	pRes->AddFrame("YetiRollRU", Vector2(96.f * (6.f + i), 96.f * 3.f), Vector2(96.f * (7.f + i), 96.f * 4.f));


	//pRes->CreateAni2DSeq("YetiIdleRD", pYetiTex, 0.3f);
	//for (int i = 0; i < 3; ++i)
	//	pRes->AddFrame("YetiIdleRD", Vector2(96.f * i, 96.f * 4.f), Vector2(96.f * (1 + i), 96.f * 5.f));

	//pRes->CreateAni2DSeq("YetiAttackRD", pYetiTex, 0.2f);
	//for (int i = 0; i < 3; ++i)
	//	pRes->AddFrame("YetiAttackRD", Vector2(96.f * (i + 3), 96.f * 4.f), Vector2(96.f * (4 + i), 96.f * 5.f));

	//pRes->CreateAni2DSeq("YetiRollRD", pYetiTex, 0.3f);
	//for (int i = 0; i < 3; ++i)
	//	pRes->AddFrame("YetiRollRD", Vector2(96.f * (6.f + i), 96.f * 4.f), Vector2(96.f * (7.f + i), 96.f * 5.f));


	//pRes->CreateAni2DSeq("YetiDieStandR", pYetiTex, 0.3f);
	//for (int i = 0; i < 3; ++i)
	//	pRes->AddFrame("YetiDieStandR", Vector2(96.f * i, 96.f * 5.f), Vector2(96.f * (1 + i), 96.f * 6.f));

	//pRes->CreateAni2DSeq("YetiDieStandU", pYetiTex, 0.3f);
	//for (int i = 0; i < 3; ++i)
	//	pRes->AddFrame("YetiDieStandU", Vector2(96.f * i, 96.f * 6.f), Vector2(96.f * (1 + i), 96.f * 7.f));

	//pRes->CreateAni2DSeq("YetiDieStandD", pYetiTex, 0.3f);
	//for (int i = 0; i < 3; ++i)
	//	pRes->AddFrame("YetiDieStandD", Vector2(96.f * i, 96.f * 7.f), Vector2(96.f * (1 + i), 96.f * 8.f));

	//pRes->CreateAni2DSeq("YetiDieStandRU", pYetiTex, 0.3f);
	//for (int i = 0; i < 3; ++i)
	//	pRes->AddFrame("YetiDieStandRU", Vector2(96.f * i, 96.f * 8.f), Vector2(96.f * (1 + i), 96.f * 9.f));

	//pRes->CreateAni2DSeq("YetiDieStandRD", pYetiTex, 0.3f);
	//for (int i = 0; i < 3; ++i)
	//	pRes->AddFrame("YetiDieStandRD", Vector2(96.f * i, 96.f * 9.f), Vector2(96.f * (1 + i), 96.f * 10.f));


	//pRes->CreateAni2DSeq("YetiDieR", pYetiTex, 0.3f);
	//for (int i = 4; i < 7; ++i)
	//	pRes->AddFrame("YetiDieR", Vector2(96.f * i, 96.f * 5.f), Vector2(96.f * (1 + i), 96.f * 6.f));

	//pRes->CreateAni2DSeq("YetiDieU", pYetiTex, 0.3f);
	//for (int i = 4; i < 7; ++i)
	//	pRes->AddFrame("YetiDieU", Vector2(96.f * i, 96.f * 6.f), Vector2(96.f * (1 + i), 96.f * 7.f));

	//pRes->CreateAni2DSeq("YetiDieD", pYetiTex, 0.3f);
	//for (int i = 3; i < 7; ++i)
	//	pRes->AddFrame("YetiDieD", Vector2(96.f * i, 96.f * 7.f), Vector2(96.f * (1 + i), 96.f * 8.f));

	//pRes->CreateAni2DSeq("YetiDieRU", pYetiTex, 0.3f);
	//for (int i = 4; i < 7; ++i)
	//	pRes->AddFrame("YetiDieRU", Vector2(96.f * i, 96.f * 8.f), Vector2(96.f * (1 + i), 96.f * 9.f));

	//pRes->CreateAni2DSeq("YetiDieRD", pYetiTex, 0.3f);
	//for (int i = 4; i < 7; ++i)
	//	pRes->AddFrame("YetiDieRD", Vector2(96.f * i, 96.f * 9.f), Vector2(96.f * (1 + i), 96.f * 10.f));


	//pRes->CreateAni2DSeq("IceBall", pYetiTex, 2.f);

	//pRes->AddFrame("IceBall", Vector2(32.f, 1024.f - 32.f), Vector2(64.f, 1024.f));

	//pRes->CreateAni2DSeq("IceBallShadow", pYetiTex, 2.f);

	//pRes->AddFrame("IceBallShadow", Vector2(0.f, 1024.f - 32.f), Vector2(32.f, 1024.f));

	//pRes->CreateAni2DSeq("YetiShadow", pYetiTex, 2.f);

	//pRes->AddFrame("YetiShadow", Vector2(0.f, 960.f), Vector2(80.f, 992.f));

	//SAFE_RELEASE(pYetiTex);
}

void CMainGameMode::LoadSound()
{
	GET_SINGLE(CSoundManager)->LoadSound("BrainA", SOUND_TYPE::BGM,
		"MUSIC\\BOSS\\BrainA.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("BrainB", SOUND_TYPE::BGM,
		"MUSIC\\BOSS\\BrainB.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Colossus", SOUND_TYPE::BGM,
		"MUSIC\\BOSS\\Colossus.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Eyecube", SOUND_TYPE::BGM,
		"MUSIC\\BOSS\\Eyecube.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("arrow_charge", SOUND_TYPE::EFFECT,
		"arrow_charge.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("arrow_retrieval", SOUND_TYPE::EFFECT,
		"arrow_retrieval.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("arrow_shoot", SOUND_TYPE::EFFECT,
		"arrow_shoot.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("Arrow_windloop", SOUND_TYPE::EFFECT,
		"Arrow_windloop.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("ArrowIgnite", SOUND_TYPE::EFFECT,
		"ArrowIgnite.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("ArrowImpact1", SOUND_TYPE::EFFECT,
		"ArrowImpact1.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("ArrowImpact2", SOUND_TYPE::EFFECT,
		"ArrowImpact2.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("ArrowImpact3", SOUND_TYPE::EFFECT,
		"ArrowImpact3.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("ArrowImpact4", SOUND_TYPE::EFFECT,
		"ArrowImpact4.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("ArrowPlantImpact1", SOUND_TYPE::EFFECT,
		"ArrowPlantImpact1.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("ArrowPlantImpact2", SOUND_TYPE::EFFECT,
		"ArrowPlantImpact2.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("ArrowPlantImpact3", SOUND_TYPE::EFFECT,
		"ArrowPlantImpact3.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("ArrowPlantImpact4", SOUND_TYPE::EFFECT,
		"ArrowPlantImpact4.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("AvariceEntrance", SOUND_TYPE::EFFECT,
		"AvariceEntrance.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("BigDoor", SOUND_TYPE::EFFECT,
		"BigDoor.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("ElevatorAscendIn", SOUND_TYPE::EFFECT,
		"ElevatorAscendIn.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("ElevatorAscendOut", SOUND_TYPE::EFFECT,
		"ElevatorAscendOut.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("ElevatorDescendIn", SOUND_TYPE::EFFECT,
		"ElevatorDescendIn.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("ElevatorDescendOut", SOUND_TYPE::EFFECT,
		"ElevatorDescendOut.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("EnterDoor", SOUND_TYPE::EFFECT,
		"EnterDoor.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("EyeDoor", SOUND_TYPE::EFFECT,
		"EyeDoor.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("EyeDoorEyeOpen", SOUND_TYPE::EFFECT,
		"EyeDoorEyeOpen.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("InWaterLoop", SOUND_TYPE::EFFECT,
		"InWaterLoop.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("LightTorch", SOUND_TYPE::EFFECT,
		"LightTorch.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("LongRoll", SOUND_TYPE::EFFECT,
		"LongRoll.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("LongRollEnd", SOUND_TYPE::EFFECT,
		"LongRollEnd.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("MinorArrowImpact1", SOUND_TYPE::EFFECT,
		"MinorArrowImpact1.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("MinorArrowImpact2", SOUND_TYPE::EFFECT,
		"MinorArrowImpact2.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("MinorArrowImpact3", SOUND_TYPE::EFFECT,
		"MinorArrowImpact3.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("MinorArrowImpact4", SOUND_TYPE::EFFECT,
		"MinorArrowImpact4.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("Pickuparrow", SOUND_TYPE::EFFECT,
		"Pickuparrow.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("Roll", SOUND_TYPE::EFFECT,
		"Roll.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("Roll_bash", SOUND_TYPE::EFFECT,
		"Roll_bash.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("Blob", SOUND_TYPE::EFFECT,
		"Death\\Blob.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("Crush", SOUND_TYPE::EFFECT,
		"Death\\Crush.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("Impact", SOUND_TYPE::EFFECT,
		"Death\\Impact.ogg", "Player_Sound_Path");

	GET_SINGLE(CSoundManager)->LoadSound("Laser", SOUND_TYPE::EFFECT,
		"Death\\Laser.ogg", "Player_Sound_Path");

	for (int i = 0; i < 8; ++i)
	{
		char strNum[32] = {};

		sprintf_s(strNum, "GloopStep%d", i + 1);

		char strPath[32] = {};

		sprintf_s(strPath, "Gloop\\Step%d.ogg", i + 1);

		GET_SINGLE(CSoundManager)->LoadSound(strNum, SOUND_TYPE::EFFECT,
			strPath, "Player_Sound_Path");
	}

	for (int i = 0; i < 8; ++i)
	{
		char strNum[32] = {};

		sprintf_s(strNum, "GrassStep%d", i + 1);

		char strPath[32] = {};

		sprintf_s(strPath, "Grass\\Step%d.ogg", i + 1);

		GET_SINGLE(CSoundManager)->LoadSound(strNum, SOUND_TYPE::EFFECT,
			strPath, "Player_Sound_Path");
	}

	for (int i = 0; i < 8; ++i)
	{
		char strNum[32] = {};

		sprintf_s(strNum, "IceStep%d", i + 1);

		char strPath[32] = {};

		sprintf_s(strPath, "Ice\\Step%d.ogg", i + 1);

		GET_SINGLE(CSoundManager)->LoadSound(strNum, SOUND_TYPE::EFFECT,
			strPath, "Player_Sound_Path");
	}

	for (int i = 0; i < 8; ++i)
	{
		char strNum[32] = {};

		sprintf_s(strNum, "SandStep%d", i + 1);

		char strPath[32] = {};

		sprintf_s(strPath, "Sand\\Step%d.ogg", i + 1);

		GET_SINGLE(CSoundManager)->LoadSound(strNum, SOUND_TYPE::EFFECT,
			strPath, "Player_Sound_Path");
	}

	GET_SINGLE(CSoundManager)->LoadSound("SavePoint", SOUND_TYPE::EFFECT,
		"SAVEPOINT\\SavePoint.ogg", "Player_Sound_Path");

	for (int i = 0; i < 8; ++i)
	{
		char strNum[32] = {};

		sprintf_s(strNum, "ShallowStep%d", i + 1);

		char strPath[32] = {};

		sprintf_s(strPath, "Shallow\\Step%d.ogg", i + 1);

		GET_SINGLE(CSoundManager)->LoadSound(strNum, SOUND_TYPE::EFFECT,
			strPath, "Player_Sound_Path");
	}

	for (int i = 0; i < 8; ++i)
	{
		char strNum[32] = {};

		sprintf_s(strNum, "SnowStep%d", i + 1);

		char strPath[32] = {};

		sprintf_s(strPath, "Snow\\Step%d.ogg", i + 1);

		GET_SINGLE(CSoundManager)->LoadSound(strNum, SOUND_TYPE::EFFECT,
			strPath, "Player_Sound_Path");
	}

	for (int i = 0; i < 8; ++i)
	{
		char strNum[32] = {};

		sprintf_s(strNum, "StoneStep%d", i + 1);

		char strPath[32] = {};

		sprintf_s(strPath, "Stone\\Step%d.ogg", i + 1);

		GET_SINGLE(CSoundManager)->LoadSound(strNum, SOUND_TYPE::EFFECT,
			strPath, "Player_Sound_Path");
	}

	for (int i = 0; i < 8; ++i)
	{
		char strNum[32] = {};

		sprintf_s(strNum, "VineStep%d", i + 1);

		char strPath[32] = {};

		sprintf_s(strPath, "Vine\\Step%d.ogg", i + 1);

		GET_SINGLE(CSoundManager)->LoadSound(strNum, SOUND_TYPE::EFFECT,
			strPath, "Player_Sound_Path");
	}

	for (int i = 0; i < 8; ++i)
	{
		char strNum[32] = {};

		sprintf_s(strNum, "VineRockStep%d", i + 1);

		char strPath[32] = {};

		sprintf_s(strPath, "VineRock\\Step%d.ogg", i + 1);

		GET_SINGLE(CSoundManager)->LoadSound(strNum, SOUND_TYPE::EFFECT,
			strPath, "Player_Sound_Path");
	}

	for (int i = 0; i < 8; ++i)
	{
		char strNum[32] = {};

		sprintf_s(strNum, "WaterStep%d", i + 1);

		char strPath[32] = {};

		sprintf_s(strPath, "Water\\Step%d.ogg", i + 1);

		GET_SINGLE(CSoundManager)->LoadSound(strNum, SOUND_TYPE::EFFECT,
			strPath, "Player_Sound_Path");
	}

	GET_SINGLE(CSoundManager)->LoadSound("BrainTelekinesis", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\BRAINFREEZE\\BrainTelekinesis.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("BunsenIgnite", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\BRAINFREEZE\\BunsenIgnite.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("BunsenLoop", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\BRAINFREEZE\\BunsenLoop.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("ButtonContact", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\BRAINFREEZE\\ButtonContact.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("ButtonPush", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\BRAINFREEZE\\ButtonPush.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("FastMovementLoop", SOUND_TYPE::BGM,
		"SFX\\BOSS\\BRAINFREEZE\\FastMovementLoop.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("HeavyImpact1", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\BRAINFREEZE\\HeavyImpact1.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("HeavyImpact2", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\BRAINFREEZE\\HeavyImpact2.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("HeavyImpact3", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\BRAINFREEZE\\HeavyImpact3.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("HeavyImpact4", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\BRAINFREEZE\\HeavyImpact4.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Land1", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\BRAINFREEZE\\Land1.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Land2", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\BRAINFREEZE\\Land2.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Land3", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\BRAINFREEZE\\Land3.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Land4", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\BRAINFREEZE\\Land4.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Melt", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\BRAINFREEZE\\Melt.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("NormalMovementLoop", SOUND_TYPE::BGM,
		"SFX\\BOSS\\BRAINFREEZE\\NormalMovementLoop.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("SoftImpact1", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\BRAINFREEZE\\SoftImpact1.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("SoftImpact2", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\BRAINFREEZE\\SoftImpact2.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("SoftImpact3", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\BRAINFREEZE\\SoftImpact3.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("SoftImpact4", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\BRAINFREEZE\\SoftImpact4.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("TelekinesisJump", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\BRAINFREEZE\\TelekinesisJump.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("TelekinesisSlide", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\BRAINFREEZE\\TelekinesisSlide.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Hit1", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\COLOSSUS\\Hit1.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Hit2", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\COLOSSUS\\Hit2.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Hit3", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\COLOSSUS\\Hit3.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Hit4", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\COLOSSUS\\Hit4.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Rise", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\COLOSSUS\\Rise.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("RiseBodyLand", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\COLOSSUS\\RiseBodyLand.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Roar", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\COLOSSUS\\Roar.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Special", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\COLOSSUS\\Special.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("SpecialBodyLand", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\COLOSSUS\\SpecialBodyLand.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("SpecialFistLand", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\COLOSSUS\\SpecialFistLand.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("EyeClose", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\EYECUBE\\EyeClose.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("EyeOpen", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\EYECUBE\\EyeOpen.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Land", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\EYECUBE\\Land.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Laser", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\EYECUBE\\Laser.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("LaserCharge", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\EYECUBE\\LaserCharge.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("LaserJump", SOUND_TYPE::EFFECT,
		"SFX\\BOSS\\EYECUBE\\LaserJump.ogg", "Sound_Path2");

	for (int i = 0; i < 8; ++i)
	{
		char strName[32] = {};

		sprintf_s(strName, "Roll%d", i + 1);

		char strPath[32] = {};

		sprintf_s(strPath, "SFX\\BOSS\\EYECUBE\\Roll%d.ogg", i + 1);

		GET_SINGLE(CSoundManager)->LoadSound(strName, SOUND_TYPE::EFFECT,
			strPath, "Sound_Path2");
	}

	for (int i = 0; i < 8; ++i)
	{
		char strName[32] = {};

		sprintf_s(strName, "Slam%d", i + 1);

		char strPath[32] = {};

		sprintf_s(strPath, "SFX\\BOSS\\EYECUBE\\Slam%d.ogg", i + 1);

		GET_SINGLE(CSoundManager)->LoadSound(strName, SOUND_TYPE::EFFECT,
			strPath, "Sound_Path2");
	}

	GET_SINGLE(CSoundManager)->LoadSound("Ascension", SOUND_TYPE::EFFECT,
		"SFX\\TITANKILL\\Ascension.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("HeavyDrumLoop", SOUND_TYPE::BGM,
		"SFX\\TITANKILL\\HeavyDrumLoop.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Impact_Boss", SOUND_TYPE::EFFECT,
		"SFX\\TITANKILL\\Impact.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("IntroLayer1", SOUND_TYPE::EFFECT,
		"SFX\\TITANKILL\\IntroLayer1.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("IntroLayer2", SOUND_TYPE::EFFECT,
		"SFX\\TITANKILL\\IntroLayer2.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("LightDrumLoop", SOUND_TYPE::BGM,
		"SFX\\TITANKILL\\LightDrumLoop.ogg", "Sound_Path2");

	GET_SINGLE(CSoundManager)->LoadSound("Release", SOUND_TYPE::EFFECT,
		"SFX\\TITANKILL\\Release.ogg", "Sound_Path2");
}

void CMainGameMode::LoadParticle()
{
	CParticleSystem* pParticle = GET_SINGLE(CResourceManager)->CreateParticle("Souls", false, 256, sizeof(PARTICLE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	SAFE_RELEASE(pParticle);

	CParticleSystem* pSRV = GET_SINGLE(CResourceManager)->CreateParticle("SoulsShare", false, 1, sizeof(PARTICLESHARE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	if (pSRV)
	{
		pSRV->SetStartColor(Vector4(1.f, 1.f, 1.f, 1.f));
		pSRV->SetEndColor(Vector4(1.f, 1.f, 1.f, 1.f));
		pSRV->SetEndScale(Vector3(0.f, 0.f, 0.f));
		pSRV->SetStartScale(Vector3(24.f, 24.f, 0.f));
		pSRV->SetMinSpeed(0.f);
		pSRV->SetMaxSpeed(0.f);
		pSRV->SetMove(false);
		pSRV->SetMaxLifeTime(2.f);
		pSRV->SetMinLifeTime(2.f);
		pSRV->SetMaxCount(256);
		pSRV->SetRange(Vector3(0.f, 0.f, 0.f));
	}

	SAFE_RELEASE(pSRV);

	CParticleSystem* pDustParticle = GET_SINGLE(CResourceManager)->CreateParticle("Dust", false, 128, sizeof(PARTICLE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	SAFE_RELEASE(pDustParticle);

	CParticleSystem* pDustSRV = GET_SINGLE(CResourceManager)->CreateParticle("DustShare", false, 1, sizeof(PARTICLESHARE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	if (pDustSRV)
	{
		pDustSRV->SetStartColor(Vector4(0.2f, 0.2f, 0.2f, 1.f));
		pDustSRV->SetEndColor(Vector4(0.2f, 0.2f, 0.2f, 0.f));
		pDustSRV->SetEndScale(Vector3(32.f, 32.f, 0.f));
		pDustSRV->SetStartScale(Vector3(32.f, 32.f, 0.f));
		pDustSRV->SetMinSpeed(112.f);
		pDustSRV->SetMaxSpeed(112.f);
		pDustSRV->SetAccel(Vector3(-112.f, 0.f, 0.f));
		pDustSRV->SetMove(true);
		pDustSRV->SetMaxLifeTime(1.f);
		pDustSRV->SetMinLifeTime(1.f);
		pDustSRV->SetMaxCount(128);
		pDustSRV->SetRange(Vector3(192.f, 192.f, 192.f));
		pDustSRV->SetAngle(XM_PI);
	}

	SAFE_RELEASE(pDustSRV);

	pParticle = GET_SINGLE(CResourceManager)->CreateParticle("DustCol", false, 64, sizeof(PARTICLE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	SAFE_RELEASE(pParticle);

	pSRV = GET_SINGLE(CResourceManager)->CreateParticle("DustColShare", false, 1, sizeof(PARTICLESHARE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	if (pSRV)
	{
		pSRV->SetStartColor(Vector4(91.f/255.f, 72.f/ 255.f, 53.f / 255.f, 1.f));
		pSRV->SetEndColor(Vector4(91.f / 255.f, 72.f / 255.f, 53.f / 255.f, 1.f));
		pSRV->SetEndScale(Vector3(0.f, 0.f, 0.f));
		pSRV->SetStartScale(Vector3(32.f, 32.f, 0.f));
		pSRV->SetMinSpeed(48.f);
		pSRV->SetMaxSpeed(48.f);
		pSRV->SetMove(true);
		pSRV->SetMaxLifeTime(1.f);
		pSRV->SetMinLifeTime(1.f);
		pSRV->SetMaxCount(64);
		pSRV->SetRange(Vector3(128.f, 128.f, 32.f));
		pSRV->SetAngle(XM_PIDIV2 / 3.f);
		pSRV->SetAccel(Vector3(-32.f, 0.f, 0.f));
	}

	SAFE_RELEASE(pSRV);

	CParticleSystem* pIceParticle = GET_SINGLE(CResourceManager)->CreateParticle("Ice", false, 32, sizeof(PARTICLE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	SAFE_RELEASE(pIceParticle);

	CParticleSystem* pIceSRV = GET_SINGLE(CResourceManager)->CreateParticle("IceShare", false, 1, sizeof(PARTICLESHARE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	if (pIceSRV)
	{
		pIceSRV->SetStartColor(Vector4(1.f, 1.f, 1.f, 1.f));
		pIceSRV->SetEndColor(Vector4(1.f, 1.f, 1.f, 0.f));
		pIceSRV->SetEndScale(Vector3(32.f, 32.f, 0.f));
		pIceSRV->SetStartScale(Vector3(0.f, 0.f, 0.f));
		pIceSRV->SetMinSpeed(16.f);
		pIceSRV->SetMaxSpeed(16.f);
		pIceSRV->SetMoveDir(Vector3(0.f, 1.f, 0.f));
		pIceSRV->SetMove(true);
		pIceSRV->SetMaxLifeTime(1.f);
		pIceSRV->SetMinLifeTime(1.f);
		pIceSRV->SetMaxCount(32);
		pIceSRV->SetRange(Vector3(320.f, 240.f, 0.f));
		pIceSRV->SetAngle(XM_PI);
	}

	SAFE_RELEASE(pIceSRV);

	CParticleSystem* pSnowParticle = GET_SINGLE(CResourceManager)->CreateParticle("Snow", false, 12, sizeof(PARTICLE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	SAFE_RELEASE(pSnowParticle);

	CParticleSystem* pSnowSRV = GET_SINGLE(CResourceManager)->CreateParticle("SnowShare", false, 1, sizeof(PARTICLESHARE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	if (pSnowSRV)
	{
		pSnowSRV->SetStartColor(Vector4(1.f, 1.f, 1.f, 1.f));
		pSnowSRV->SetEndColor(Vector4(1.f, 1.f, 1.f, 0.f));
		pSnowSRV->SetEndScale(Vector3(16.f, 16.f, 0.f));
		pSnowSRV->SetStartScale(Vector3(16.f, 16.f, 0.f));
		pSnowSRV->SetMaxSpeed(8.f);
		pSnowSRV->SetMinSpeed(8.f);
		pSnowSRV->SetMoveDir(Vector3(0.f, 1.f, 0.f));
		pSnowSRV->SetMove(true);
		pSnowSRV->SetMaxLifeTime(1.f);
		pSnowSRV->SetMinLifeTime(1.f);
		pSnowSRV->SetMaxCount(12);
		pSnowSRV->SetRange(Vector3(320.f, 240.f, 0.f));
	}

	SAFE_RELEASE(pSnowSRV);

	CParticleSystem* pGrassParticle = GET_SINGLE(CResourceManager)->CreateParticle("Grass", false, 512, sizeof(PARTICLE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	SAFE_RELEASE(pGrassParticle);

	CParticleSystem* pGrassSRV = GET_SINGLE(CResourceManager)->CreateParticle("GrassShare", false, 1, sizeof(PARTICLESHARE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	if (pGrassSRV)
	{
		pGrassSRV->SetStartColor(Vector4(120.f / 255.f, 180.f / 255.f, 130.f / 255.f, 1.f));
		pGrassSRV->SetEndColor(Vector4(120.f / 255.f, 180.f / 255.f, 130.f / 255.f, 0.f));
		pGrassSRV->SetEndScale(Vector3(3.f, 3.f, 0.f));
		pGrassSRV->SetStartScale(Vector3(3.f, 3.f, 0.f));
		pGrassSRV->SetMaxSpeed(64.f);
		pGrassSRV->SetMinSpeed(64.f);
		pGrassSRV->SetMoveDir(Vector3(0.f, 1.f, 0.f));
		pGrassSRV->SetMove(true);
		pGrassSRV->SetMaxLifeTime(0.5f);
		pGrassSRV->SetMinLifeTime(0.5f);
		pGrassSRV->SetMaxCount(512);
		pGrassSRV->SetRange(Vector3(48.f, 48.f, 0.f));
		pGrassSRV->SetAngle(DirectX::XM_2PI * 2.f);
		pGrassSRV->SetAccel(Vector3(-256.f, -0.f, 0.f));
	}

	SAFE_RELEASE(pGrassSRV);

	CParticleSystem* pTileDustParticle = GET_SINGLE(CResourceManager)->CreateParticle("TileDust", false, 64, sizeof(PARTICLE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	SAFE_RELEASE(pTileDustParticle);

	CParticleSystem* pTileDustSRV = GET_SINGLE(CResourceManager)->CreateParticle("TileDustShare", false, 1, sizeof(PARTICLESHARE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	if (pTileDustSRV)
	{
		pTileDustSRV->SetStartColor(Vector4(132.f / 255.f, 128.f / 255.f, 127.f / 255.f, 1.f));
		pTileDustSRV->SetEndColor(Vector4(132.f / 255.f, 128.f / 255.f, 127.f / 255.f, 0.f));
		pTileDustSRV->SetEndScale(Vector3(0.f, 0.f, 0.f));
		pTileDustSRV->SetStartScale(Vector3(16.f, 16.f, 0.f));
		pTileDustSRV->SetMaxSpeed(24.f);
		pTileDustSRV->SetMinSpeed(12.f);
		pTileDustSRV->SetMoveDir(Vector3(0.f, 1.f, 0.f));
		pTileDustSRV->SetMove(true);
		pTileDustSRV->SetMaxLifeTime(0.5f);
		pTileDustSRV->SetMinLifeTime(0.5f);
		pTileDustSRV->SetMaxCount(64);
		pTileDustSRV->SetRange(Vector3(48.f, 48.f, 0.f));
		pTileDustSRV->SetAnim(3, 0.1f);
	}

	SAFE_RELEASE(pTileDustSRV);

	pTileDustParticle = GET_SINGLE(CResourceManager)->CreateParticle("TileDustCol", false, 64, sizeof(PARTICLE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	SAFE_RELEASE(pTileDustParticle);

	pTileDustSRV = GET_SINGLE(CResourceManager)->CreateParticle("TileDustColShare", false, 1, sizeof(PARTICLESHARE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	if (pTileDustSRV)
	{
		pTileDustSRV->SetStartColor(Vector4(132.f / 255.f, 128.f / 255.f, 127.f / 255.f, 1.f));
		pTileDustSRV->SetEndColor(Vector4(132.f / 255.f, 128.f / 255.f, 127.f / 255.f, 1.f));
		pTileDustSRV->SetEndScale(Vector3(0.f, 0.f, 0.f));
		pTileDustSRV->SetStartScale(Vector3(16.f, 16.f, 0.f));
		pTileDustSRV->SetMaxSpeed(112.f);
		pTileDustSRV->SetMinSpeed(112.f);
		pTileDustSRV->SetMove(true);
		pTileDustSRV->SetMaxLifeTime(1.5f);
		pTileDustSRV->SetMinLifeTime(1.5f);
		pTileDustSRV->SetMaxCount(64);
		pTileDustSRV->SetRange(Vector3(20.f, 20.f, 0.f));
		pTileDustSRV->SetAccel(Vector3(-80.f, 0.f, 0.f));
	}

	SAFE_RELEASE(pTileDustSRV);

	pTileDustParticle = GET_SINGLE(CResourceManager)->CreateParticle("BlueDust", false, 64, sizeof(PARTICLE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	SAFE_RELEASE(pTileDustParticle);

	pTileDustSRV = GET_SINGLE(CResourceManager)->CreateParticle("BlueDustShare", false, 1, sizeof(PARTICLESHARE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	if (pTileDustSRV)
	{
		pTileDustSRV->SetStartColor(Vector4(28.f / 255.f, 56.f / 255.f, 69.f / 255.f, 1.f));
		pTileDustSRV->SetEndColor(Vector4(28.f / 255.f, 56.f / 255.f, 69.f / 255.f, 0.f));
		pTileDustSRV->SetEndScale(Vector3(0.f, 0.f, 0.f));
		pTileDustSRV->SetStartScale(Vector3(16.f, 16.f, 0.f));
		pTileDustSRV->SetMaxSpeed(12.f);
		pTileDustSRV->SetMinSpeed(12.f);
		pTileDustSRV->SetMoveDir(Vector3(0.f, 1.f, 0.f));
		pTileDustSRV->SetMove(true);
		pTileDustSRV->SetMaxLifeTime(0.5f);
		pTileDustSRV->SetMinLifeTime(0.5f);
		pTileDustSRV->SetMaxCount(64);
		pTileDustSRV->SetRange(Vector3(48.f, 48.f, 0.f));
		pTileDustSRV->SetAnim(3, 0.1f);
	}

	SAFE_RELEASE(pTileDustSRV);

	CParticleSystem* pWaterParticle = GET_SINGLE(CResourceManager)->CreateParticle("Water", false, 5, sizeof(PARTICLE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	SAFE_RELEASE(pWaterParticle);

	CParticleSystem* pWaterSRV = GET_SINGLE(CResourceManager)->CreateParticle("WaterShare", false, 1, sizeof(PARTICLESHARE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	if (pWaterSRV)
	{
		pWaterSRV->SetStartColor(Vector4(1.f, 1.f, 1.f, 1.f));
		pWaterSRV->SetEndColor(Vector4(1.f, 1.f, 1.f, 0.f));
		pWaterSRV->SetEndScale(Vector3(64.f, 32.f, 0.f));
		pWaterSRV->SetStartScale(Vector3(32.f, 16.f, 0.f));
		pWaterSRV->SetMove(false);
		pWaterSRV->SetMaxLifeTime(3.f);
		pWaterSRV->SetMinLifeTime(3.f);
		pWaterSRV->SetMaxCount(5);
		pWaterSRV->SetRange(Vector3(0.f, 0.f, 0.f));
	}

	SAFE_RELEASE(pWaterSRV);

	CParticleSystem* pWaterSplash = GET_SINGLE(CResourceManager)->CreateParticle("WaterSplash", false, 64, sizeof(PARTICLE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	SAFE_RELEASE(pWaterSplash);

	CParticleSystem* pWaterSplashSRV = GET_SINGLE(CResourceManager)->CreateParticle("WaterSplashShare", false, 1, sizeof(PARTICLESHARE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	if (pWaterSplashSRV)
	{
		pWaterSplashSRV->SetStartColor(Vector4(1.f, 1.f, 1.f, 1.f));
		pWaterSplashSRV->SetEndColor(Vector4(1.f, 1.f, 1.f, 1.f));
		pWaterSplashSRV->SetEndScale(Vector3(16.f, 16.f, 0.f));
		pWaterSplashSRV->SetStartScale(Vector3(16.f, 16.f, 0.f));
		pWaterSplashSRV->SetMaxSpeed(50.f);
		pWaterSplashSRV->SetMinSpeed(0.f);
		pWaterSplashSRV->SetAccel(Vector3(-100.f, 0.f, 0.f));
		pWaterSplashSRV->SetMoveDir(Vector3(0.f, 1.f, 0.f));
		pWaterSplashSRV->SetMove(true);
		pWaterSplashSRV->SetMaxLifeTime(0.75f);
		pWaterSplashSRV->SetMinLifeTime(0.75f);
		pWaterSplashSRV->SetMaxCount(64);
		pWaterSplashSRV->SetRange(Vector3(64.f, 64.f, 0.f));
		pWaterSplashSRV->SetAnim(4, 0.f);
	}

	SAFE_RELEASE(pWaterSplashSRV);

	CParticleSystem* pBlood = GET_SINGLE(CResourceManager)->CreateParticle("Blood", false, 5, sizeof(PARTICLE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	SAFE_RELEASE(pBlood);

	CParticleSystem* pBloodSRV = GET_SINGLE(CResourceManager)->CreateParticle("BloodShare", false, 1, sizeof(PARTICLESHARE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	if (pBloodSRV)
	{
		pBloodSRV->SetStartColor(Vector4(1.f, 0.f, 0.f, 1.f));
		pBloodSRV->SetEndColor(Vector4(1.f, 0.f, 0.f, 1.f));
		pBloodSRV->SetEndScale(Vector3(16.f, 16.f, 0.f));
		pBloodSRV->SetStartScale(Vector3(16.f, 16.f, 0.f));
		pBloodSRV->SetMove(false);
		pBloodSRV->SetMaxLifeTime(5.f);
		pBloodSRV->SetMinLifeTime(5.f);
		pBloodSRV->SetMaxCount(5);
		pBloodSRV->SetRange(Vector3(640.f, 640.f, 0.f));
		pBloodSRV->SetAnim(4, 0.f);
	}

	SAFE_RELEASE(pBloodSRV);

	CParticleSystem* pArrowFire = GET_SINGLE(CResourceManager)->CreateParticle("ArrowFire", false, 128, sizeof(PARTICLE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	SAFE_RELEASE(pArrowFire);

	CParticleSystem* pArrowFireSRV = GET_SINGLE(CResourceManager)->CreateParticle("ArrowFireShare", false, 1, sizeof(PARTICLESHARE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	if (pArrowFireSRV)
	{
		pArrowFireSRV->SetStartColor(Vector4(1.f, 1.f, 1.f, 1.f));
		pArrowFireSRV->SetEndColor(Vector4(1.f, 1.f, 1.f, 0.f));
		pArrowFireSRV->SetEndScale(Vector3(0.f, 0.f, 0.f));
		pArrowFireSRV->SetStartScale(Vector3(16.f, 16.f, 0.f));
		pArrowFireSRV->SetMaxSpeed(8.f);
		pArrowFireSRV->SetMinSpeed(8.f);
		pArrowFireSRV->SetMoveDir(Vector3(0.f, 1.f, 0.f));
		pArrowFireSRV->SetMove(true);
		pArrowFireSRV->SetMaxLifeTime(1.f);
		pArrowFireSRV->SetMinLifeTime(1.f);
		pArrowFireSRV->SetMaxCount(128);
		pArrowFireSRV->SetRange(Vector3(1.f, 1.f, 0.f));
		pArrowFireSRV->SetAnim(4, 0.1f);
	}

	SAFE_RELEASE(pArrowFireSRV);

	pParticle = GET_SINGLE(CResourceManager)->CreateParticle("ColDown", false, 256, sizeof(PARTICLE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	SAFE_RELEASE(pParticle);

	pSRV = GET_SINGLE(CResourceManager)->CreateParticle("ColDownShare", false, 1, sizeof(PARTICLESHARE), 0,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	if (pSRV)
	{
		pSRV->SetStartColor(Vector4(186.f/255.f, 153.f / 255.f, 94.f / 255.f, 1.f));
		pSRV->SetEndColor(Vector4(40.f / 255.f, 34.f / 255.f, 24.f / 255.f, 0.f));
		pSRV->SetEndScale(Vector3(24.f, 24.f, 0.f));
		pSRV->SetStartScale(Vector3(24.f, 24.f, 0.f));
		pSRV->SetMinSpeed(0.f);
		pSRV->SetMaxSpeed(0.f);
		pSRV->SetMove(false);
		pSRV->SetMaxLifeTime(2.f);
		pSRV->SetMinLifeTime(2.f);
		pSRV->SetMaxCount(256);
		pSRV->SetRange(Vector3(512.f, 32.f, 0.f));
	}

	SAFE_RELEASE(pSRV);
}

void CMainGameMode::StartScene(float)
{
	GET_SINGLE(CSceneManager)->CreateNextScene();

	GET_SINGLE(CSceneManager)->SetGameMode<CStartGameMode>(false);
}

void CMainGameMode::CreateUI()
{
	CLayer* pUI = m_pScene->FindLayer("UI");

	pUI->SetUI();

	CObj* pButton =  m_pScene->CreateObject<CObj>("Button", pUI);

	pButton->Enable(false);

	CSpriteComponent* pBtn = pButton->CreateComponent<CSpriteComponent>("Return", pUI);

	pBtn->CreateSprite("On", "ReturnOn", LOOP_OPTION::LOOP);
	pBtn->CreateSprite("Off", "Return", LOOP_OPTION::LOOP);

	pBtn->SetWorldScale(32.f, 32.f, 0.f);
	pBtn->SetPivot(0.5f, 0.5f, 0.f);

	Resolution tRS = RESOLUTION;
	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();
	RectInfo tInfo = pCam->GetRect();

	SAFE_RELEASE(pCam);

	pBtn->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	pBtn->SetWorldPos((float)tRS.iWidth / 2.f - 32.f, (float)tRS.iHeight * tInfo.fT, 0.f);

	pBtn->AddRenderState("NoCullBack");
	pBtn->AddRenderState("DepthNoWrite");

	pButton->SetRootComponent(pBtn);

	CSpriteComponent* pBtn2 = pButton->CreateComponent<CSpriteComponent>("Option", pUI);

	pBtn2->CreateSprite("Off", "Option", LOOP_OPTION::LOOP);
	pBtn2->CreateSprite("On", "OptionOn", LOOP_OPTION::LOOP);

	pBtn2->SetWorldScale(32.f, 32.f, 0.f);
	pBtn2->SetPivot(0.5f, 0.5f, 0.f);
	pBtn2->SetInheritScale(false);
	pBtn2->AddRelativePos(32.f, 0.f, 0.f);
	pBtn2->AddRenderState("NoCullBack");
	pBtn2->AddRenderState("DepthNoWrite");

	pBtn2->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

	pBtn->AddChild(pBtn2);

	CSpriteComponent* pBtn3 = pButton->CreateComponent<CSpriteComponent>("Exit", pUI);

	pBtn3->CreateSprite("Off", "Exit", LOOP_OPTION::LOOP);
	pBtn3->CreateSprite("On", "ExitOn", LOOP_OPTION::LOOP);

	pBtn3->SetWorldScale(32.f, 32.f, 0.f);
	pBtn3->SetPivot(0.5f, 0.5f, 0.f);
	pBtn3->SetInheritScale(false);
	pBtn3->AddRelativePos(64.f, 0.f, 0.f);
	pBtn3->AddRenderState("NoCullBack");
	pBtn3->AddRenderState("DepthNoWrite");

	pBtn3->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

	pBtn->AddChild(pBtn3);

	SAFE_RELEASE(pBtn3);

	SAFE_RELEASE(pBtn2);

	SAFE_RELEASE(pBtn);

	SAFE_RELEASE(pButton);
}

bool CMainGameMode::LoadXml(const char* pFileName, const std::string& strPathKey)
{
	return CTileMode::LoadXml(pFileName, strPathKey);
}

void CMainGameMode::searchXMLData(TiXmlElement* pElem)
{
	m_bHub = true;
	CTileMode::searchXMLData(pElem);
}

bool CMainGameMode::LoadSequance(const char* pFileName, const std::string& strPathKey)
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

bool CMainGameMode::LoadFrame(const char* pFileName, const std::string& strPathKey)
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

void CMainGameMode::Open(float)
{
	CObj* pObj = GET_SINGLE(CBossManager)->FindMonster("boss_colossus");

	if (!pObj)
	{
		return;
	}

	SAFE_RELEASE(pObj);

	pObj = GET_SINGLE(CBossManager)->FindMonster("boss_eyecube");

	if (!pObj)
	{
		return;
	}

	SAFE_RELEASE(pObj);

	pObj = GET_SINGLE(CBossManager)->FindMonster("boss_brainfreeze");

	if (!pObj)
	{
		return;
	}

	static bool bOnce = false;

	if (!bOnce)
	{
		if (GET_SINGLE(CSceneManager)->GetScene() == m_pScene)
		{
			m_bOpen = true;

			m_pEft->SetSoundAndPlay("BigDoor");

			m_pEft->SetSound("Hub1");
			m_pEft->SetFade(true, false);

			bOnce = true;
		}
	}

	SAFE_RELEASE(pObj);
}

void CMainGameMode::Col(CCollider* pSrc, CCollider* pDst, float fTime)
{
	std::string strDst = pDst->GetName();

	if (strDst == "PlayerBody")
	{
		bool bSaved = GET_SINGLE(CBossManager)->IsSaved();

		if (!bSaved)
		{
			m_pEft->SetSoundAndPlay("SavePoint");

			SetFadeColor(0.f, 0.f, 0.f);
			SetFade(0.5f, 0.f, -1.f);

			CEffect* pBossEffect = m_pScene->CreateObject<CEffect>("BossEffect2",
				m_pScene->FindLayer("Ground"), m_pScene->GetSceneType());

			CSpriteComponent* pBossCom = pBossEffect->CreateComponent<CSpriteComponent>("BossEffect",
				m_pScene->FindLayer("Ground"));

			pBossCom->SetWorldScale(48.f, 48.f, 0.f);
			pBossCom->SetPivot(0.5f, 0.5f, 0.f);

			pBossCom->CreateSprite("Effect2", "BossEffect1", LOOP_OPTION::LOOP);

			pBossCom->AddNotify("Effect2", "End", 3.f);

			pBossCom->AddCallBack<CEffect>("Effect2", "End", pBossEffect, &CEffect::Destroy);

			pBossCom->AddRenderState("AlphaBlend");
			pBossCom->AddRenderState("DepthNoWrite");

			pBossEffect->SetRootComponent(pBossCom);

			SAFE_RELEASE(pBossEffect);

			SAFE_RELEASE(pBossCom);

			GET_SINGLE(CBossManager)->Save();

			int iSlot = GET_SINGLE(CBossManager)->GetSlot();

			SLOTINFO tInfo = GET_SINGLE(CBossManager)->GetInfo(iSlot);

			if ((int)tInfo.eType & (int)BOSS_TYPE::COLOSSUS)
			{
				CEffect* pBossEffect = (CEffect*)m_pScene->FindLayer("Ground")->FindObj("BossEffect");

				CSceneComponent* pCom = pBossEffect->FindSceneComponent("ColEffect");

				pCom->Enable(true);

				SAFE_RELEASE(pCom);

				SAFE_RELEASE(pBossEffect);
			}

			if ((int)tInfo.eType & (int)BOSS_TYPE::EYECUBE)
			{
				CEffect* pBossEffect = (CEffect*)m_pScene->FindLayer("Ground")->FindObj("BossEffect");

				CSceneComponent* pCom = pBossEffect->FindSceneComponent("EyeEffect");

				pCom->Enable(true);

				SAFE_RELEASE(pCom);

				SAFE_RELEASE(pBossEffect);
			}

			if ((int)tInfo.eType & (int)BOSS_TYPE::BRAINFREEZE)
			{
				CEffect* pBossEffect = (CEffect*)m_pScene->FindLayer("Ground")->FindObj("BossEffect");

				CSceneComponent* pCom = pBossEffect->FindSceneComponent("BrainEffect");

				pCom->Enable(true);

				SAFE_RELEASE(pCom);

				SAFE_RELEASE(pBossEffect);
			}
		}
	}
}

void SetImgui(float)
{
	bool bImgui = GET_SINGLE(CEngine)->IsImgui();

	GET_SINGLE(CEngine)->SetImgui(!bImgui);
}
