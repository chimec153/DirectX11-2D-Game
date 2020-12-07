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

CMainGameMode::CMainGameMode()
{
	m_iGameModeClassType = (int)GAMEMODE_CLASS_TYPE::GCT_MAIN;
}

CMainGameMode::~CMainGameMode()
{
}

bool CMainGameMode::Init()
{
	LoadResource();

	LoadXml("MAPS\\JAM\\JAM.tmx");

	CLayer* pLayer = m_pScene->FindLayer("Default");

	pLayer->Set2D();

	CPlayer* pPlayer = m_pScene->CreateObject<CPlayer>("player", pLayer);

	SetPlayer(pPlayer);

	pPlayer->SetWorldPos(1024.f, 1024.f, 0.f);

	SAFE_RELEASE(pPlayer);

	CLayer* pBack = m_pScene->FindLayer("Back");
/*
	CObj* pTileObj = m_pScene->CreateObject<CObj>("TileObj", pBack);

	CTileMap* pMap = pTileObj->CreateComponent<CTileMap>("TileMap");

	pMap->CreateTile(100, 100, Vector3(16.f, 16.f, 0.f), Vector2(), TILE_TYPE::RECT);

	pMap->SetTileTexture("Overworld");

	pTileObj->SetRootComponent(pMap);

	SAFE_RELEASE(pMap);

	SAFE_RELEASE(pTileObj);*/

	CBullet* pBullet = CScene::CreateProtoObj<CBullet>("Bullet", m_pScene, m_pScene->GetSceneType());

	SAFE_RELEASE(pBullet);

	/*CMinion* pMin = m_pScene->CreateObject<CMinion>("Min", pLayer);

	SAFE_RELEASE(pMin);*/

	/*CIceBall* pIce = CScene::CreateProtoObj<CIceBall>("Ice", m_pScene, m_pScene->GetSceneType());

	SAFE_RELEASE(pIce);*/

	CEffect* pEft = CScene::CreateProtoObj<CEffect>("Fire", m_pScene, m_pScene->GetSceneType());

	SAFE_RELEASE(pEft);

	CMissile* pMissile = CScene::CreateProtoObj<CMissile>("Missile", m_pScene, m_pScene->GetSceneType());

	SAFE_RELEASE(pMissile);

	//CreateUI();

	return true;
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
	//GET_SINGLE(CResourceManager)->LoadTexture("Overworld", TEXT("Overworld.png"));

	CTexture* pTex = GET_SINGLE(CResourceManager)->FindTexture("Pro");
	CTexture* pBowTex = GET_SINGLE(CResourceManager)->FindTexture("Bow");
	CTexture* pArrowTex = GET_SINGLE(CResourceManager)->FindTexture("Arrow");
	CTexture* pEftTex = GET_SINGLE(CResourceManager)->FindTexture("Effect");

	CResourceManager* pRes = GET_SINGLE(CResourceManager);

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProIdleR", pTex, 10.f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowIdleR", pBowTex, 10.f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowIdleR", pArrowTex, 10.f);
	GET_SINGLE(CResourceManager)->AddFrame("ProIdleR", Vector2(0.f, 0.f), Vector2(16.f, 16.f));
	GET_SINGLE(CResourceManager)->AddFrame("BowIdleR", Vector2(0.f, 0.f), Vector2(16.f, 16.f));
	GET_SINGLE(CResourceManager)->AddFrame("ArrowIdleR", Vector2(0.f, 0.f), Vector2(16.f, 16.f));

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProWalkR", pTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowWalkR", pBowTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowWalkR", pArrowTex, 0.5f);

	for (int i = 0; i < 6; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProWalkR", Vector2(16.f * i, 0.f), Vector2(16.f * (i + 1), 16.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowWalkR", Vector2(16.f * i, 0.f), Vector2(16.f * (i + 1), 16.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowWalkR", Vector2(16.f * i, 0.f), Vector2(16.f * (i + 1), 16.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProRollR", pTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowRollR", pBowTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowRollR", pArrowTex, 0.5f);
	for (int i = 6; i < 12; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProRollR", Vector2(16.f * i, 0.f), Vector2(16.f * (i + 1), 16.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowRollR", Vector2(16.f * i, 0.f), Vector2(16.f * (i + 1), 16.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowRollR", Vector2(16.f * i, 0.f), Vector2(16.f * (i + 1), 16.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProAttackR", pTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowAttackR", pBowTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowAttackR", pArrowTex);
	for (int i = 0; i < 1; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProAttackR", Vector2(16.f * (i + 13), 0.f), Vector2(16.f * (i + 14), 16.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowAttackR", Vector2(16.f * (i + 13), 0.f), Vector2(16.f * (i + 14), 16.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowAttackR", Vector2(16.f * (i + 13), 0.f), Vector2(16.f * (i + 14), 16.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProCallR", pTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowCallR", pBowTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowCallR", pArrowTex);
	for (int i = 14; i < 15; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProCallR", Vector2(16.f * i, 0.f), Vector2(16.f * (i + 1), 16.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowCallR", Vector2(16.f * i, 0.f), Vector2(16.f * (i + 1), 16.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowCallR", Vector2(16.f * i, 0.f), Vector2(16.f * (i + 1), 16.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProIdleU", pTex, 10.f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowIdleU", pBowTex, 10.f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowIdleU", pArrowTex, 10.f);
	GET_SINGLE(CResourceManager)->AddFrame("ProIdleU", Vector2(0.f, 16.f), Vector2(16.f, 16.f * 2.f));
	GET_SINGLE(CResourceManager)->AddFrame("BowIdleU", Vector2(0.f, 16.f), Vector2(16.f, 16.f * 2.f));
	GET_SINGLE(CResourceManager)->AddFrame("ArrowIdleU", Vector2(0.f, 16.f), Vector2(16.f, 16.f * 2.f));

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProWalkU", pTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowWalkU", pBowTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowWalkU", pArrowTex, 0.5f);
	for (int i = 0; i < 6; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProWalkU", Vector2(16.f * i, 16.f), Vector2(16.f * (i + 1), 16.f * 2.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowWalkU", Vector2(16.f * i, 16.f), Vector2(16.f * (i + 1), 16.f * 2.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowWalkU", Vector2(16.f * i, 16.f), Vector2(16.f * (i + 1), 16.f * 2.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProRollU", pTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowRollU", pBowTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowRollU", pArrowTex, 0.5f);
	for (int i = 6; i < 12; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProRollU", Vector2(16.f * i, 16.f), Vector2(16.f * (i + 1), 16.f * 2.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowRollU", Vector2(16.f * i, 16.f), Vector2(16.f * (i + 1), 16.f * 2.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowRollU", Vector2(16.f * i, 16.f), Vector2(16.f * (i + 1), 16.f * 2.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProAttackU", pTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowAttackU", pBowTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowAttackU", pArrowTex);
	for (int i = 0; i < 1; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProAttackU", Vector2(16.f * (i + 13), 16.f), Vector2(16.f * (i + 14), 16.f * 2.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowAttackU", Vector2(16.f * (i + 13), 16.f), Vector2(16.f * (i + 14), 16.f * 2.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowAttackU", Vector2(16.f * (i + 13), 16.f), Vector2(16.f * (i + 14), 16.f * 2.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProCallU", pTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowCallU", pBowTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowCallU", pArrowTex);
	for (int i = 14; i < 15; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProCallU", Vector2(16.f * i, 16.f), Vector2(16.f * (i + 1), 16.f * 2.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowCallU", Vector2(16.f * i, 16.f), Vector2(16.f * (i + 1), 16.f * 2.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowCallU", Vector2(16.f * i, 16.f), Vector2(16.f * (i + 1), 16.f * 2.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProIdleL", pTex, 10.f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowIdleL", pBowTex, 10.f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowIdleL", pArrowTex, 10.f);
	GET_SINGLE(CResourceManager)->AddFrame("ProIdleL", Vector2(0.f, 16.f * 2.f), Vector2(16.f, 16.f * 3.f));
	GET_SINGLE(CResourceManager)->AddFrame("BowIdleL", Vector2(0.f, 16.f * 2.f), Vector2(16.f, 16.f * 3.f));
	GET_SINGLE(CResourceManager)->AddFrame("ArrowIdleL", Vector2(0.f, 16.f * 2.f), Vector2(16.f, 16.f * 3.f));

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProWalkL", pTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowWalkL", pBowTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowWalkL", pArrowTex, 0.5f);
	for (int i = 0; i < 6; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProWalkL", Vector2(16.f * i, 16.f * 2.f), Vector2(16.f * (i + 1), 16.f * 3.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowWalkL", Vector2(16.f * i, 16.f * 2.f), Vector2(16.f * (i + 1), 16.f * 3.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowWalkL", Vector2(16.f * i, 16.f * 2.f), Vector2(16.f * (i + 1), 16.f * 3.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProRollL", pTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowRollL", pBowTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowRollL", pArrowTex, 0.5f);
	for (int i = 6; i < 12; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProRollL", Vector2(16.f * i, 16.f * 2.f), Vector2(16.f * (i + 1), 16.f * 3.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowRollL", Vector2(16.f * i, 16.f * 2.f), Vector2(16.f * (i + 1), 16.f * 3.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowRollL", Vector2(16.f * i, 16.f * 2.f), Vector2(16.f * (i + 1), 16.f * 3.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProAttackL", pTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowAttackL", pBowTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowAttackL", pArrowTex);
	for (int i = 0; i < 1; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProAttackL", Vector2(16.f * (i + 13), 16.f * 2.f), Vector2(16.f * (i + 14), 16.f * 3.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowAttackL", Vector2(16.f * (i + 13), 16.f * 2.f), Vector2(16.f * (i + 14), 16.f * 3.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowAttackL", Vector2(16.f * (i + 13), 16.f * 2.f), Vector2(16.f * (i + 14), 16.f * 3.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProCallL", pTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowCallL", pBowTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowCallL", pArrowTex);
	for (int i = 14; i < 15; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProCallL", Vector2(16.f * i, 16.f * 2.f), Vector2(16.f * (i + 1), 16.f * 3.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowCallL", Vector2(16.f * i, 16.f * 2.f), Vector2(16.f * (i + 1), 16.f * 3.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowCallL", Vector2(16.f * i, 16.f * 2.f), Vector2(16.f * (i + 1), 16.f * 3.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProIdleD", pTex, 10.f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowIdleD", pBowTex, 10.f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowIdleD", pArrowTex, 10.f);
	GET_SINGLE(CResourceManager)->AddFrame("ProIdleD", Vector2(0.f, 16.f * 3.f), Vector2(16.f, 16.f * 4.f));
	GET_SINGLE(CResourceManager)->AddFrame("BowIdleD", Vector2(0.f, 16.f * 3.f), Vector2(16.f, 16.f * 4.f));
	GET_SINGLE(CResourceManager)->AddFrame("ArrowIdleD", Vector2(0.f, 16.f * 3.f), Vector2(16.f, 16.f * 4.f));

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProWalkD", pTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowWalkD", pBowTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowWalkD", pArrowTex, 0.5f);
	for (int i = 0; i < 6; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProWalkD", Vector2(16.f * i, 16.f * 3.f), Vector2(16.f * (i + 1), 16.f * 4.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowWalkD", Vector2(16.f * i, 16.f * 3.f), Vector2(16.f * (i + 1), 16.f * 4.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowWalkD", Vector2(16.f * i, 16.f * 3.f), Vector2(16.f * (i + 1), 16.f * 4.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProRollD", pTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowRollD", pBowTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowRollD", pArrowTex, 0.5f);
	for (int i = 6; i < 12; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProRollD", Vector2(16.f * i, 16.f * 3.f), Vector2(16.f * (i + 1), 16.f * 4.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowRollD", Vector2(16.f * i, 16.f * 3.f), Vector2(16.f * (i + 1), 16.f * 4.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowRollD", Vector2(16.f * i, 16.f * 3.f), Vector2(16.f * (i + 1), 16.f * 4.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProAttackD", pTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowAttackD", pBowTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowAttackD", pArrowTex);
	for (int i = 0; i < 1; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProAttackD", Vector2(16.f * (i + 13), 16.f * 3.f), Vector2(16.f * (i + 14), 16.f * 4.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowAttackD", Vector2(16.f * (i + 13), 16.f * 3.f), Vector2(16.f * (i + 14), 16.f * 4.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowAttackD", Vector2(16.f * (i + 13), 16.f * 3.f), Vector2(16.f * (i + 14), 16.f * 4.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProCallD", pTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowCallD", pBowTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowCallD", pArrowTex);
	for (int i = 14; i < 15; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProCallD", Vector2(16.f * i, 16.f * 3.f), Vector2(16.f * (i + 1), 16.f * 4.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowCallD", Vector2(16.f * i, 16.f * 3.f), Vector2(16.f * (i + 1), 16.f * 4.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowCallD", Vector2(16.f * i, 16.f * 3.f), Vector2(16.f * (i + 1), 16.f * 4.f));
	}


	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProIdleRD", pTex, 10.f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowIdleRD", pBowTex, 10.f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowIdleRD", pArrowTex, 10.f);
	GET_SINGLE(CResourceManager)->AddFrame("ProIdleRD", Vector2(0.f, 16.f * 4.f), Vector2(16.f, 16.f * 5.f));
	GET_SINGLE(CResourceManager)->AddFrame("BowIdleRD", Vector2(0.f, 16.f * 4.f), Vector2(16.f, 16.f * 5.f));
	GET_SINGLE(CResourceManager)->AddFrame("ArrowIdleRD", Vector2(0.f, 16.f * 4.f), Vector2(16.f, 16.f * 5.f));

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProWalkRD", pTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowWalkRD", pBowTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowWalkRD", pArrowTex, 0.5f);
	for (int i = 0; i < 6; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProWalkRD", Vector2(16.f * i, 16.f * 4.f), Vector2(16.f * (i + 1), 16.f * 5.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowWalkRD", Vector2(16.f * i, 16.f * 4.f), Vector2(16.f * (i + 1), 16.f * 5.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowWalkRD", Vector2(16.f * i, 16.f * 4.f), Vector2(16.f * (i + 1), 16.f * 5.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProRollRD", pTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowRollRD", pBowTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowRollRD", pArrowTex, 0.5f);
	for (int i = 6; i < 12; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProRollRD", Vector2(16.f * i, 16.f * 4.f), Vector2(16.f * (i + 1), 16.f * 5.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowRollRD", Vector2(16.f * i, 16.f * 4.f), Vector2(16.f * (i + 1), 16.f * 5.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowRollRD", Vector2(16.f * i, 16.f * 4.f), Vector2(16.f * (i + 1), 16.f * 5.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProAttackRD", pTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowAttackRD", pBowTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowAttackRD", pArrowTex);
	for (int i = 0; i < 1; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProAttackRD", Vector2(16.f * (i + 13), 16.f * 4.f), Vector2(16.f * (i + 14), 16.f * 5.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowAttackRD", Vector2(16.f * (i + 13), 16.f * 4.f), Vector2(16.f * (i + 14), 16.f * 5.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowAttackRD", Vector2(16.f * (i + 13), 16.f * 4.f), Vector2(16.f * (i + 14), 16.f * 5.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProCallRD", pTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowCallRD", pBowTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowCallRD", pArrowTex);
	for (int i = 14; i < 15; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProCallRD", Vector2(16.f * i, 16.f * 4.f), Vector2(16.f * (i + 1), 16.f * 5.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowCallRD", Vector2(16.f * i, 16.f * 4.f), Vector2(16.f * (i + 1), 16.f * 5.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowCallRD", Vector2(16.f * i, 16.f * 4.f), Vector2(16.f * (i + 1), 16.f * 5.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProIdleLD", pTex, 10.f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowIdleLD", pBowTex, 10.f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowIdleLD", pArrowTex, 10.f);
	GET_SINGLE(CResourceManager)->AddFrame("ProIdleLD", Vector2(0.f, 16.f * 5.f), Vector2(16.f, 16.f * 6.f));
	GET_SINGLE(CResourceManager)->AddFrame("BowIdleLD", Vector2(0.f, 16.f * 5.f), Vector2(16.f, 16.f * 6.f));
	GET_SINGLE(CResourceManager)->AddFrame("ArrowIdleLD", Vector2(0.f, 16.f * 5.f), Vector2(16.f, 16.f * 6.f));

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProWalkLD", pTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowWalkLD", pBowTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowWalkLD", pArrowTex, 0.5f);
	for (int i = 0; i < 6; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProWalkLD", Vector2(16.f * i, 16.f * 5.f), Vector2(16.f * (i + 1), 16.f * 6.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowWalkLD", Vector2(16.f * i, 16.f * 5.f), Vector2(16.f * (i + 1), 16.f * 6.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowWalkLD", Vector2(16.f * i, 16.f * 5.f), Vector2(16.f * (i + 1), 16.f * 6.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProRollLD", pTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowRollLD", pBowTex, 0.5f);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowRollLD", pArrowTex, 0.5f);
	for (int i = 6; i < 12; ++i)
	{
		GET_SINGLE(CResourceManager)->AddFrame("ProRollLD", Vector2(16.f * i, 16.f * 5.f), Vector2(16.f * (i + 1), 16.f * 6.f));
		GET_SINGLE(CResourceManager)->AddFrame("BowRollLD", Vector2(16.f * i, 16.f * 5.f), Vector2(16.f * (i + 1), 16.f * 6.f));
		GET_SINGLE(CResourceManager)->AddFrame("ArrowRollLD", Vector2(16.f * i, 16.f * 5.f), Vector2(16.f * (i + 1), 16.f * 6.f));
	}

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ProAttackLD", pTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("BowAttackLD", pBowTex);
	GET_SINGLE(CResourceManager)->CreateAni2DSeq("ArrowAttackLD", pArrowTex);
	for (int i = 0; i < 1; ++i)
	{
		pRes->AddFrame("ProAttackLD", Vector2(16.f * (i + 13), 16.f * 5.f), Vector2(16.f * (i + 14), 16.f * 6.f));
		pRes->AddFrame("BowAttackLD", Vector2(16.f * (i + 13), 16.f * 5.f), Vector2(16.f * (i + 14), 16.f * 6.f));
		pRes->AddFrame("ArrowAttackLD", Vector2(16.f * (i + 13), 16.f * 5.f), Vector2(16.f * (i + 14), 16.f * 6.f));
	}

	pRes->CreateAni2DSeq("ProCallLD", pTex);
	pRes->CreateAni2DSeq("BowCallLD", pBowTex);
	pRes->CreateAni2DSeq("ArrowCallLD", pArrowTex);
	for (int i = 14; i < 15; ++i)
	{
		pRes->AddFrame("ProCallLD", Vector2(16.f * i, 16.f * 5.f), Vector2(16.f * (i + 1), 16.f * 6.f));
		pRes->AddFrame("BowCallLD", Vector2(16.f * i, 16.f * 5.f), Vector2(16.f * (i + 1), 16.f * 6.f));
		pRes->AddFrame("ArrowCallLD", Vector2(16.f * i, 16.f * 5.f), Vector2(16.f * (i + 1), 16.f * 6.f));
	}


	pRes->CreateAni2DSeq("ProIdleLU", pTex, 10.f);
	pRes->CreateAni2DSeq("BowIdleLU", pBowTex, 10.f);
	pRes->CreateAni2DSeq("ArrowIdleLU", pArrowTex, 10.f);
	pRes->AddFrame("ProIdleLU", Vector2(0.f, 16.f * 6.f), Vector2(16.f, 16.f * 7.f));
	pRes->AddFrame("BowIdleLU", Vector2(0.f, 16.f * 6.f), Vector2(16.f, 16.f * 7.f));
	pRes->AddFrame("ArrowIdleLU", Vector2(0.f, 16.f * 6.f), Vector2(16.f, 16.f * 7.f));

	pRes->CreateAni2DSeq("ProWalkLU", pTex, 0.5f);
	pRes->CreateAni2DSeq("BowWalkLU", pBowTex, 0.5f);
	pRes->CreateAni2DSeq("ArrowWalkLU", pArrowTex, 0.5f);
	for (int i = 0; i < 6; ++i)
	{
		pRes->AddFrame("ProWalkLU", Vector2(16.f * i, 16.f * 6.f), Vector2(16.f * (i + 1), 16.f * 7.f));
		pRes->AddFrame("BowWalkLU", Vector2(16.f * i, 16.f * 6.f), Vector2(16.f * (i + 1), 16.f * 7.f));
		pRes->AddFrame("ArrowWalkLU", Vector2(16.f * i, 16.f * 6.f), Vector2(16.f * (i + 1), 16.f * 7.f));
	}

	pRes->CreateAni2DSeq("ProRollLU", pTex, 0.5f);
	pRes->CreateAni2DSeq("BowRollLU", pBowTex, 0.5f);
	pRes->CreateAni2DSeq("ArrowRollLU", pArrowTex, 0.5f);
	for (int i = 6; i < 12; ++i)
	{
		pRes->AddFrame("ProRollLU", Vector2(16.f * i, 16.f * 6.f), Vector2(16.f * (i + 1), 16.f * 7.f));
		pRes->AddFrame("BowRollLU", Vector2(16.f * i, 16.f * 6.f), Vector2(16.f * (i + 1), 16.f * 7.f));
		pRes->AddFrame("ArrowRollLU", Vector2(16.f * i, 16.f * 6.f), Vector2(16.f * (i + 1), 16.f * 7.f));
	}

	pRes->CreateAni2DSeq("ProAttackLU", pTex);
	pRes->CreateAni2DSeq("BowAttackLU", pBowTex);
	pRes->CreateAni2DSeq("ArrowAttackLU", pArrowTex);
	for (int i = 0; i < 1; ++i)
	{
		pRes->AddFrame("ProAttackLU", Vector2(16.f * (i + 13), 16.f * 6.f), Vector2(16.f * (i + 14), 16.f * 7.f));
		pRes->AddFrame("BowAttackLU", Vector2(16.f * (i + 13), 16.f * 6.f), Vector2(16.f * (i + 14), 16.f * 7.f));
		pRes->AddFrame("ArrowAttackLU", Vector2(16.f * (i + 13), 16.f * 6.f), Vector2(16.f * (i + 14), 16.f * 7.f));
	}

	pRes->CreateAni2DSeq("ProCallLU", pTex);
	pRes->CreateAni2DSeq("BowCallLU", pBowTex);
	pRes->CreateAni2DSeq("ArrowCallLU", pArrowTex);
	for (int i = 14; i < 15; ++i)
	{
		pRes->AddFrame("ProCallLU", Vector2(16.f * i, 16.f * 6.f), Vector2(16.f * (i + 1), 16.f * 7.f));
		pRes->AddFrame("BowCallLU", Vector2(16.f * i, 16.f * 6.f), Vector2(16.f * (i + 1), 16.f * 7.f));
		pRes->AddFrame("ArrowCallLU", Vector2(16.f * i, 16.f * 6.f), Vector2(16.f * (i + 1), 16.f * 7.f));
	}


	pRes->CreateAni2DSeq("ProIdleRU", pTex, 10.f);
	pRes->CreateAni2DSeq("BowIdleRU", pBowTex, 10.f);
	pRes->CreateAni2DSeq("ArrowIdleRU", pArrowTex, 10.f);
	pRes->AddFrame("ProIdleRU", Vector2(0.f, 16.f * 7.f), Vector2(16.f, 16.f * 8.f));
	pRes->AddFrame("BowIdleRU", Vector2(0.f, 16.f * 7.f), Vector2(16.f, 16.f * 8.f));
	pRes->AddFrame("ArrowIdleRU", Vector2(0.f, 16.f * 7.f), Vector2(16.f, 16.f * 8.f));

	pRes->CreateAni2DSeq("ProWalkRU", pTex, 0.5f);
	pRes->CreateAni2DSeq("BowWalkRU", pBowTex, 0.5f);
	pRes->CreateAni2DSeq("ArrowWalkRU", pArrowTex, 0.5f);
	for (int i = 0; i < 6; ++i)
	{
		pRes->AddFrame("ProWalkRU", Vector2(16.f * i, 16.f * 7.f), Vector2(16.f * (i + 1), 16.f * 8.f));
		pRes->AddFrame("BowWalkRU", Vector2(16.f * i, 16.f * 7.f), Vector2(16.f * (i + 1), 16.f * 8.f));
		pRes->AddFrame("ArrowWalkRU", Vector2(16.f * i, 16.f * 7.f), Vector2(16.f * (i + 1), 16.f * 8.f));
	}

	pRes->CreateAni2DSeq("ProRollRU", pTex, 0.5f);
	pRes->CreateAni2DSeq("BowRollRU", pBowTex, 0.5f);
	pRes->CreateAni2DSeq("ArrowRollRU", pArrowTex, 0.5f);
	for (int i = 6; i < 12; ++i)
	{
		pRes->AddFrame("ProRollRU", Vector2(16.f * i, 16.f * 7.f), Vector2(16.f * (i + 1), 16.f * 8.f));
		pRes->AddFrame("BowRollRU", Vector2(16.f * i, 16.f * 7.f), Vector2(16.f * (i + 1), 16.f * 8.f));
		pRes->AddFrame("ArrowRollRU", Vector2(16.f * i, 16.f * 7.f), Vector2(16.f * (i + 1), 16.f * 8.f));
	}

	pRes->CreateAni2DSeq("ProAttackRU", pTex);
	pRes->CreateAni2DSeq("BowAttackRU", pBowTex);
	pRes->CreateAni2DSeq("ArrowAttackRU", pArrowTex);
	for (int i = 0; i < 1; ++i)
	{
		pRes->AddFrame("ProAttackRU", Vector2(16.f * (i + 13), 16.f * 7.f), Vector2(16.f * (i + 14), 16.f * 8.f));
		pRes->AddFrame("BowAttackRU", Vector2(16.f * (i + 13), 16.f * 7.f), Vector2(16.f * (i + 14), 16.f * 8.f));
		pRes->AddFrame("ArrowAttackRU", Vector2(16.f * (i + 13), 16.f * 7.f), Vector2(16.f * (i + 14), 16.f * 8.f));
	}

	pRes->CreateAni2DSeq("ProCallRU", pTex);
	pRes->CreateAni2DSeq("BowCallRU", pBowTex);
	pRes->CreateAni2DSeq("ArrowCallRU", pArrowTex);
	for (int i = 14; i < 15; ++i)
	{
		pRes->AddFrame("ProCallRU", Vector2(16.f * i, 16.f * 7.f), Vector2(16.f * (i + 1), 16.f * 8.f));
		pRes->AddFrame("BowCallRU", Vector2(16.f * i, 16.f * 7.f), Vector2(16.f * (i + 1), 16.f * 8.f));
		pRes->AddFrame("ArrowCallRU", Vector2(16.f * i, 16.f * 7.f), Vector2(16.f * (i + 1), 16.f * 8.f));
	}

	pRes->CreateAni2DSeq("Arrow", pTex);

	for (int i = 0; i < 1; ++i)
		pRes->AddFrame("Arrow", Vector2(16.f * 31.f, 0.f), Vector2(16.f * 32.f, 16.f));

	GET_SINGLE(CResourceManager)->CreateAni2DSeq("Fire", pEftTex);

	for (int i = 0; i < 9; ++i)
		GET_SINGLE(CResourceManager)->AddFrame("Fire", Vector2(i * 16.f, 176.f), Vector2((i+1) * 16.f, 191.f));

	GET_SINGLE(CResourceManager)->AddNotify("Fire", "FireEnd", 8);

	SAFE_RELEASE(pTex);
	SAFE_RELEASE(pBowTex);
	SAFE_RELEASE(pArrowTex);
	SAFE_RELEASE(pEftTex);

	pRes->LoadTexture("Yeti", TEXT("Yeti.png"));

	CTexture* pYetiTex = pRes->FindTexture("Yeti");

	pRes->CreateAni2DSeq("YetiIdleR", pYetiTex, 0.3f);
	for (int i = 0; i < 3; ++i)
		pRes->AddFrame("YetiIdleR", Vector2(96.f * i, 0.f), Vector2(96.f * (1 + i), 96.f));

	pRes->CreateAni2DSeq("YetiAttackR", pYetiTex, 0.2f);
	for (int i = 0; i < 3; ++i)
		pRes->AddFrame("YetiAttackR", Vector2(96.f * (i + 3), 0.f), Vector2(96.f * (4 + i), 96.f));

	pRes->CreateAni2DSeq("YetiRollR", pYetiTex, 0.3f);
	for (int i = 0; i < 3; ++i)
		pRes->AddFrame("YetiRollR", Vector2(96.f * (6.f + i), 0.f), Vector2(96.f * (7.f + i), 96.f));


	pRes->CreateAni2DSeq("YetiIdleU", pYetiTex, 0.3f);
	for (int i = 0; i < 3; ++i)
		pRes->AddFrame("YetiIdleU", Vector2(96.f * i, 96.f), Vector2(96.f * (1 + i), 96.f * 2.f));

	pRes->CreateAni2DSeq("YetiAttackU", pYetiTex, 0.2f);
	for (int i = 0; i < 3; ++i)
		pRes->AddFrame("YetiAttackU", Vector2(96.f * (i + 3), 96.f), Vector2(96.f * (4 + i), 96.f * 2.f));

	pRes->CreateAni2DSeq("YetiRollU", pYetiTex, 0.3f);
	for (int i = 0; i < 3; ++i)
		pRes->AddFrame("YetiRollU", Vector2(96.f * (6.f + i), 96.f), Vector2(96.f * (7.f + i), 96.f * 2.f));


	pRes->CreateAni2DSeq("YetiIdleD", pYetiTex, 0.3f);
	for (int i = 0; i < 3; ++i)
		pRes->AddFrame("YetiIdleD", Vector2(96.f * i, 96.f * 2.f), Vector2(96.f * (1 + i), 96.f * 3.f));

	pRes->CreateAni2DSeq("YetiAttackD", pYetiTex, 0.2f);
	for (int i = 0; i < 3; ++i)
		pRes->AddFrame("YetiAttackD", Vector2(96.f * (i + 3), 96.f * 2.f), Vector2(96.f * (4 + i), 96.f * 3.f));

	pRes->CreateAni2DSeq("YetiRollD", pYetiTex, 0.3f);
	for (int i = 0; i < 3; ++i)
		pRes->AddFrame("YetiRollD", Vector2(96.f * (6.f + i), 96.f * 2.f), Vector2(96.f * (7.f + i), 96.f * 3.f));


	pRes->CreateAni2DSeq("YetiIdleRU", pYetiTex, 0.3f);
	for (int i = 0; i < 3; ++i)
		pRes->AddFrame("YetiIdleRU", Vector2(96.f * i, 96.f * 3.f), Vector2(96.f * (1 + i), 96.f * 4.f));

	pRes->CreateAni2DSeq("YetiAttackRU", pYetiTex, 0.2f);
	for (int i = 0; i < 3; ++i)
		pRes->AddFrame("YetiAttackRU", Vector2(96.f * (i + 3), 96.f * 3.f), Vector2(96.f * (4 + i), 96.f * 4.f));

	pRes->CreateAni2DSeq("YetiRollRU", pYetiTex, 0.3f);
	for (int i = 0; i < 3; ++i)
		pRes->AddFrame("YetiRollRU", Vector2(96.f * (6.f + i), 96.f * 3.f), Vector2(96.f * (7.f + i), 96.f * 4.f));


	pRes->CreateAni2DSeq("YetiIdleRD", pYetiTex, 0.3f);
	for (int i = 0; i < 3; ++i)
		pRes->AddFrame("YetiIdleRD", Vector2(96.f * i, 96.f * 4.f), Vector2(96.f * (1 + i), 96.f * 5.f));

	pRes->CreateAni2DSeq("YetiAttackRD", pYetiTex, 0.2f);
	for (int i = 0; i < 3; ++i)
		pRes->AddFrame("YetiAttackRD", Vector2(96.f * (i + 3), 96.f * 4.f), Vector2(96.f * (4 + i), 96.f * 5.f));

	pRes->CreateAni2DSeq("YetiRollRD", pYetiTex, 0.3f);
	for (int i = 0; i < 3; ++i)
		pRes->AddFrame("YetiRollRD", Vector2(96.f * (6.f + i), 96.f * 4.f), Vector2(96.f * (7.f + i), 96.f * 5.f));


	pRes->CreateAni2DSeq("YetiDieStandR", pYetiTex, 0.3f);
	for (int i = 0; i < 3; ++i)
		pRes->AddFrame("YetiDieStandR", Vector2(96.f * i, 96.f * 5.f), Vector2(96.f * (1 + i), 96.f * 6.f));

	pRes->CreateAni2DSeq("YetiDieStandU", pYetiTex, 0.3f);
	for (int i = 0; i < 3; ++i)
		pRes->AddFrame("YetiDieStandU", Vector2(96.f * i, 96.f * 6.f), Vector2(96.f * (1 + i), 96.f * 7.f));

	pRes->CreateAni2DSeq("YetiDieStandD", pYetiTex, 0.3f);
	for (int i = 0; i < 3; ++i)
		pRes->AddFrame("YetiDieStandD", Vector2(96.f * i, 96.f * 7.f), Vector2(96.f * (1 + i), 96.f * 8.f));

	pRes->CreateAni2DSeq("YetiDieStandRU", pYetiTex, 0.3f);
	for (int i = 0; i < 3; ++i)
		pRes->AddFrame("YetiDieStandRU", Vector2(96.f * i, 96.f * 8.f), Vector2(96.f * (1 + i), 96.f * 9.f));

	pRes->CreateAni2DSeq("YetiDieStandRD", pYetiTex, 0.3f);
	for (int i = 0; i < 3; ++i)
		pRes->AddFrame("YetiDieStandRD", Vector2(96.f * i, 96.f * 9.f), Vector2(96.f * (1 + i), 96.f * 10.f));


	pRes->CreateAni2DSeq("YetiDieR", pYetiTex, 0.3f);
	for (int i = 4; i < 7; ++i)
		pRes->AddFrame("YetiDieR", Vector2(96.f * i, 96.f * 5.f), Vector2(96.f * (1 + i), 96.f * 6.f));

	pRes->CreateAni2DSeq("YetiDieU", pYetiTex, 0.3f);
	for (int i = 4; i < 7; ++i)
		pRes->AddFrame("YetiDieU", Vector2(96.f * i, 96.f * 6.f), Vector2(96.f * (1 + i), 96.f * 7.f));

	pRes->CreateAni2DSeq("YetiDieD", pYetiTex, 0.3f);
	for (int i = 3; i < 7; ++i)
		pRes->AddFrame("YetiDieD", Vector2(96.f * i, 96.f * 7.f), Vector2(96.f * (1 + i), 96.f * 8.f));

	pRes->CreateAni2DSeq("YetiDieRU", pYetiTex, 0.3f);
	for (int i = 4; i < 7; ++i)
		pRes->AddFrame("YetiDieRU", Vector2(96.f * i, 96.f * 8.f), Vector2(96.f * (1 + i), 96.f * 9.f));

	pRes->CreateAni2DSeq("YetiDieRD", pYetiTex, 0.3f);
	for (int i = 4; i < 7; ++i)
		pRes->AddFrame("YetiDieRD", Vector2(96.f * i, 96.f * 9.f), Vector2(96.f * (1 + i), 96.f * 10.f));


	pRes->CreateAni2DSeq("IceBall", pYetiTex, 2.f);

	pRes->AddFrame("IceBall", Vector2(32.f, 1024.f - 32.f), Vector2(64.f, 1024.f));

	pRes->CreateAni2DSeq("IceBallShadow", pYetiTex, 2.f);

	pRes->AddFrame("IceBallShadow", Vector2(0.f, 1024.f - 32.f), Vector2(32.f, 1024.f));

	pRes->CreateAni2DSeq("YetiShadow", pYetiTex, 2.f);

	pRes->AddFrame("YetiShadow", Vector2(0.f, 960.f), Vector2(80.f, 992.f));

	SAFE_RELEASE(pYetiTex);

	GET_SINGLE(CSoundManager)->LoadSound("Slam", SOUND_TYPE::EFFECT, "Eye Cube\\Slam1.mp3");
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

	CClientButton* pBtn = m_pScene->CreateObject<CClientButton>("Button", pUI);

	CUIButton* pButton = (CUIButton*)pBtn->FindSceneComponent("Button");

	pButton->SetCallBack<CMainGameMode>(BUTTON_STATE::PUSH, this, &CMainGameMode::StartScene);

	SAFE_RELEASE(pButton);

	SAFE_RELEASE(pBtn);

	CUIBar* pBar = m_pScene->CreateObject<CUIBar>("Bar", pUI);

	pBar->SetWorldPos(200.f, 200.f, 0.f);

	SAFE_RELEASE(pBar);

	CCheckObj* pChk = m_pScene->CreateObject<CCheckObj>("CheckBox", pUI);

	SAFE_RELEASE(pChk);

	CRadioObj* pRadio = m_pScene->CreateObject<CRadioObj>("Radio", pUI);

	SAFE_RELEASE(pRadio);
}

bool CMainGameMode::LoadXml(const char* pFileName, const std::string& strPathKey)
{
	return CTileMode::LoadXml(pFileName, strPathKey);
}

void CMainGameMode::searchXMLData(TiXmlElement* pElem)
{
	CTileMode::searchXMLData(pElem);
}

