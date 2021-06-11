#include "Player.h"
#include "Component/Mesh2DComponent.h"
#include "Resource/Material.h"
#include "Bullet.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "Component/Camera.h"
#include "Camera/CameraManager.h"
#include "InputObj.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderRect.h"
#include "Bow.h"
#include "Arrow.h"
#include "Component/ColliderOBB2D.h"
#include "Component/ColliderLine.h"
#include "Component/ColliderCircle.h"
#include "../UI/Missile.h"
#include "Scene/SceneManager.h"
#include "UI/UIFont.h"
#include "UI/Bar.h"
#include "UI/UIImage.h"
#include "Tile/TileMap.h"
#include "../GameMode/ColossusMode.h"
#include "Tile/Tile.h"
#include "Effect.h"
#include "Resource/ResourceManager.h"
#include "Component/Sound.h"
#include "../GameMode/TileMode.h"
#include "../GameMode/MainGameMode.h"
#include "Resource/ShaderManager.h"
#include "Types.h"
#include "Device.h"
#include "Colossus.h"
#include "Component/Particle.h"
#include "Eyecube.h"
#include "BrainFreeze.h"
#include "../GameMode/BrainMode.h"
#include "Resource/Mesh.h"
#include "Soul.h"
#include "Component/Line.h"
#include "../../State.h"
#include "../GameMode/EyeCubeMode.h"
#include "../GameMode/EndGameMode.h"
#include "../GameMode/StartGameMode.h"
#include "../BossManager.h"
#include "Engine.h"

float CPlayer::m_fPlayTime = 0.f;
int	CPlayer::m_iDeath = 0;
bool CPlayer::m_bMenu = false;
int CPlayer::m_iMenu = 0;
const float CPlayer::m_fWalkSpeed = 80.f;
bool CPlayer::m_bSlow = false;
float CPlayer::m_fSlowTime = 0.f;

CPlayer::CPlayer() :
	m_pMesh(nullptr),
	m_fSpeed(m_fWalkSpeed),
	m_bIdleEnable(true),
	m_fScaleX(0.f),
	m_fScaleY(0.f),
	m_eDir(DIR_8::U),
	m_bCharge(true),
	m_bRolling(false),
	m_pBow(nullptr),
	m_pArrow(nullptr),
	m_pBullet(nullptr),
	m_fBowAngle(0.f),
	m_pTileMap(nullptr),
	m_pCoord(nullptr),
	m_pShadow(nullptr),
	m_pRC(nullptr),
	m_eState(STATE::IDLE),
	m_fPushTime(0.f),
	m_fRollTime(0.f),
	m_fPrevSpeed(0.f),
	m_pSnd(nullptr),
	m_pMtrlMap(nullptr),
	m_fBackTime(0.f),
	m_bBack(false),
	m_fBackLimit(0.5f),
	m_pAirMap(nullptr),
	m_bWalkIn(false),
	m_fWalkTime(1.5f),
	//m_pFade(nullptr),
	m_fDieDelay(2.5f),
	m_bBurn(false),
	m_pShadowDown(nullptr),
	m_fFlyDist(0.f),
	m_bFly(false),
	m_iSouls(0),
	m_fLightTime(0.f),
	m_fLightTimeLimit(0.5f),
	m_iLights(0),
	m_fWhiteTime(0.f),
	m_fWhiteTimeLimit(3.f),
	m_pGrass(nullptr),
	m_eMatType(MAT_TYPE::END),
	m_bReady(true),
	m_cCol(0),
	m_pTileDust(nullptr),
	m_pWave(nullptr),
	m_pSplash(nullptr),
	m_pBlood(nullptr),
	m_bImpact(false),
	m_pBGM(nullptr),
	m_pDebugMap(nullptr),
	m_bStair(false)
{
	m_iObjClassType = (int)OBJ_CLASS_TYPE::OCT_PLAYER;
}

CPlayer::CPlayer(const CPlayer& player) :
	CObj(player)
{
	*this = player;
}

CPlayer::~CPlayer()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pBullet);
	SAFE_RELEASE(m_pBow);
	SAFE_RELEASE(m_pArrow);
	SAFE_RELEASE(m_pTileMap);
	SAFE_RELEASE(m_pCoord);
	SAFE_RELEASE(m_pShadow);
	SAFE_RELEASE(m_pRC);
	SAFE_RELEASE(m_pSnd);
	SAFE_RELEASE(m_pMtrlMap);
	SAFE_RELEASE(m_pAirMap);
	//SAFE_RELEASE(m_pFade);
	SAFE_RELEASE(m_pShadowDown);
	SAFE_RELEASE(m_pGrass);
	SAFE_RELEASE(m_pTileDust);
	SAFE_RELEASE(m_pWave);
	SAFE_RELEASE(m_pSplash);
	SAFE_RELEASE(m_pBlood);
	SAFE_RELEASE(m_pBGM);
	SAFE_RELEASE(m_pDebugMap);
}

void CPlayer::SetMeshComponent(CSpriteComponent* pMesh)
{
	SAFE_RELEASE(m_pMesh);

	m_pMesh = pMesh;

	if (m_pMesh)
		m_pMesh->AddRef();
}

void CPlayer::Notify(const std::string& strTag)
{
	if (strTag == "Shot")
	{
		//Fire1(1.f, 0.f);
	}

	else if (strTag == "Water")
	{
	}
}

void CPlayer::Charge()
{
	m_pArrow->Enable(true);
	m_bIdleEnable = true;
	m_bCharge = true;
	m_bReady = false;
#ifdef _DEBUG
	OutputDebugStringA("Charge");
#endif

	m_pSnd->SetSound("Pickuparrow");
	m_pSnd->SetVol(1.f);
	m_pSnd->Play(0.f);

	m_pSnd->SetSound("arrow_retrieval");

	m_pSnd->Stop();
}

void CPlayer::SetFly(bool bFly)
{
	m_bFly = bFly;

	CSceneComponent* pCom = FindSceneComponent("Dust");

	if (!pCom)
	{
		CParticle* pDust = CreateComponent<CParticle>("Dust", m_pScene->FindLayer("Ground"));

		pDust->AddParticle("Dust");
		pDust->AddParticle("DustShare");
		pDust->SetUVStart(64.f, 96.f);
		pDust->SetUVEnd(96.f, 128.f);
		pDust->SetUVSize(256.f, 256.f);
		pDust->SetWorldPos(GetWorldPos());
		pDust->AddRenderState("DepthNoWrite");
		pDust->AddRenderState("NoCullBack");
		pDust->SetSpawnLimit(0.01f);
		pDust->SetInheritScale(false);
		pDust->SetRelativeScale(1.f, 1.f, 1.f);

		pDust->SetInheritPos(false);

		CMaterial* pMtrl = pDust->GetMaterial();

		pMtrl->SetTexture(REGISTER_TYPE::RT_DIF, "Effect", 0, 1,
			(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

		SAFE_RELEASE(pMtrl);

		m_pMesh->AddChild(pDust);

		SAFE_RELEASE(pDust);
	}

	else
	{
		pCom->Release();
	}

	ChangeSequenceAll("Turn");
	SetDefaultClipAll("Turn");

	m_pShadow->Enable(false);

	m_pMesh->DeleteRenderState("Character");
	m_pMesh->AddRenderState("ForeGround");
}

void CPlayer::AddSoul()
{
	if (++m_iSouls >= 6)
	{
		CMaterial* pMtrl  =m_pMesh->GetMaterial();

		pMtrl->SetAmbientColor(0.f, 0.f, 0.f, 0.f);

		SAFE_RELEASE(pMtrl);

		m_pBGM->SetSound("LightDrumLoop");
		m_pBGM->Stop();

		m_pBGM->SetSoundAndPlay("HeavyDrumLoop");
	}
}

CSpriteComponent* CPlayer::GetMesh() const
{
	if (m_pMesh)
		m_pMesh->AddRef();

	return m_pMesh;
}

void CPlayer::SetBow(CBow* pObj)
{
	SAFE_RELEASE(m_pBow);

	m_pBow = pObj;

	if (m_pBow)
		m_pBow->AddRef();
}

void CPlayer::SetArrow(CArrow* pObj)
{
	SAFE_RELEASE(m_pArrow);

	m_pArrow = pObj;

	if (m_pArrow)
		m_pArrow->AddRef();
}

void CPlayer::SetDir(DIR_8 eDir)
{
	m_eDir = eDir;

	switch (m_eDir)
	{
	case DIR_8::U:
		SetDefaultClipAll("IdleU");
		break;
	case DIR_8::RU:
		SetDefaultClipAll("IdleRU");
		break;
	case DIR_8::R:
		SetDefaultClipAll("IdleR");
		break;
	case DIR_8::RD:
		SetDefaultClipAll("IdleRD");
		break;
	case DIR_8::D:
		SetDefaultClipAll("IdleD");
		break;
	case DIR_8::LD:
		SetDefaultClipAll("IdleLD");
		break;
	case DIR_8::L:
		SetDefaultClipAll("IdleL");
		break;
	case DIR_8::LU:
		SetDefaultClipAll("IdleLU");
		break;
	}
}

bool CPlayer::Init()
{
	m_pMesh = CreateComponent<CSpriteComponent>("Mesh", m_pLayer);

	m_pRC = CreateComponent<CColliderRect>("PlayerBody", m_pLayer);

	m_pRC->SetInheritScale(false);
	m_pRC->SetWorldScale(16.f, 16.f, 1.f);
	m_pRC->SetExtent(10.f, 14.f);
	m_pRC->SetProfile("Player");

	m_pRC->SetCallBack<CPlayer>(COLLISION_STATE::INIT, this, &CPlayer::ColStart);
	m_pRC->SetCallBack<CPlayer>(COLLISION_STATE::STAY, this, &CPlayer::ColStay);
	m_pRC->SetCallBack<CPlayer>(COLLISION_STATE::END, this, &CPlayer::ColEnd);

	m_pMesh->AddChild(m_pRC);

	m_pMesh->AddRenderState("Character");
	m_pMesh->AddRenderState("AlphaBlend");
	m_pMesh->AddRenderState("NoCullBack");

	CMaterial* pMat = m_pMesh->GetMaterial();

	//pMat->SetGray(false);

	SAFE_RELEASE(pMat);

	m_pShadow = CreateComponent<CSpriteComponent>("Shadow", m_pScene->FindLayer("Speacial"));

	m_pShadow->SetShader("Shadow");

	CMaterial* pMtrl = m_pShadow->GetMaterial();

	pMtrl->SetDiffuseColor(0.f, 0.f, 0.f, 0.5f);

	SAFE_RELEASE(pMtrl);

	m_pShadow->AddRenderState("Silhouette");
	m_pShadow->AddRenderState("AlphaBlend");
	m_pShadow->AddRenderState("NoCullBack");
	m_pShadow->SetInheritScale(false);
	m_pShadow->SetRelativeScale(16.f, 16.f, 1.f);
	m_pShadow->SetPivot(0.5f, 0.5f, 0.f);

	m_pShadow->CreateSprite("IdleU", "ProIdleU", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("IdleRU", "ProIdleRU", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("IdleR", "ProIdleR", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("IdleRD", "ProIdleRD", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("IdleD", "ProIdleD", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("WalkU", "ProWalkU", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("WalkRU", "ProWalkRU", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("WalkR", "ProWalkR", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("WalkRD", "ProWalkRD", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("WalkD", "ProWalkD", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("RunU", "ProRunU", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("RunRU", "ProRunRU", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("RunR", "ProRunR", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("RunRD", "ProRunRD", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("RunD", "ProRunD", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("AttackU", "ProAttackU", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("AttackRU", "ProAttackRU", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("AttackR", "ProAttackR", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("AttackRD", "ProAttackRD", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("AttackD", "ProAttackD", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("CallU", "ProCallU", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("CallRU", "ProCallRU", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("CallR", "ProCallR", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("CallRD", "ProCallRD", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("CallD", "ProCallD", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("RollU", "ProRollU", LOOP_OPTION::ONCE_RETURN);
	m_pShadow->CreateSprite("RollRU", "ProRollRU", LOOP_OPTION::ONCE_RETURN);
	m_pShadow->CreateSprite("RollR", "ProRollR", LOOP_OPTION::ONCE_RETURN);
	m_pShadow->CreateSprite("RollRD", "ProRollRD", LOOP_OPTION::ONCE_RETURN);
	m_pShadow->CreateSprite("RollD", "ProRollD", LOOP_OPTION::ONCE_RETURN);
	m_pShadow->CreateSprite("SwimU", "ProSwimU", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("SwimRU", "ProSwimRU", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("SwimR", "ProSwimR", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("SwimRD", "ProSwimRD", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("SwimD", "ProSwimD", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("DieU",	"ProDieU", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("DieRU",	"ProDieRU", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("DieR",	"ProDieR", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("DieRD",	"ProDieRD", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("DieD",	"ProDieD", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("Turn", "ProTurn", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("TurnIdle", "ProTurnIdle", LOOP_OPTION::LOOP);
	m_pShadow->CreateSprite("Randing", "ProRanding", LOOP_OPTION::ONCE_RETURN);
	m_pShadow->CreateSprite("RandingIdle", "ProRandingIdle", LOOP_OPTION::LOOP);
	
	m_pMesh->AddChild(m_pShadow);

	m_pMesh->CreateSprite("IdleU", "ProIdleU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("IdleRU", "ProIdleRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("IdleR", "ProIdleR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("IdleRD", "ProIdleRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("IdleD", "ProIdleD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("WalkU", "ProWalkU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("WalkRU", "ProWalkRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("WalkR", "ProWalkR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("WalkRD", "ProWalkRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("WalkD", "ProWalkD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RunU", "ProRunU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RunRU", "ProRunRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RunR", "ProRunR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RunRD", "ProRunRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RunD", "ProRunD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("AttackU", "ProAttackU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("AttackRU", "ProAttackRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("AttackR", "ProAttackR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("AttackRD", "ProAttackRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("AttackD", "ProAttackD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("CallU", "ProCallU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("CallRU", "ProCallRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("CallR", "ProCallR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("CallRD", "ProCallRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("CallD", "ProCallD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RollU", "ProRollU", LOOP_OPTION::ONCE_RETURN);
	m_pMesh->CreateSprite("RollRU", "ProRollRU", LOOP_OPTION::ONCE_RETURN);
	m_pMesh->CreateSprite("RollR", "ProRollR", LOOP_OPTION::ONCE_RETURN);
	m_pMesh->CreateSprite("RollRD", "ProRollRD", LOOP_OPTION::ONCE_RETURN);
	m_pMesh->CreateSprite("RollD", "ProRollD", LOOP_OPTION::ONCE_RETURN);
	m_pMesh->CreateSprite("SwimU", "ProSwimU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("SwimRU", "ProSwimRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("SwimR", "ProSwimR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("SwimRD", "ProSwimRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("SwimD", "ProSwimD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("DieU",	"ProDieU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("DieRU",	"ProDieRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("DieR",	"ProDieR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("DieRD",	"ProDieRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("DieD", "ProDieD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("Turn", "ProTurn", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("TurnIdle", "ProTurnIdle", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("Randing", "ProRanding", LOOP_OPTION::ONCE_RETURN);
	m_pMesh->CreateSprite("RandingIdle", "ProRandingIdle", LOOP_OPTION::LOOP);

	m_pMesh->AddNotify("SwimU", "Water", 2);
	m_pMesh->AddNotify("SwimD", "Water", 2);
	m_pMesh->AddNotify("SwimR", "Water", 2);
	m_pMesh->AddNotify("SwimRU", "Water", 2);
	m_pMesh->AddNotify("SwimRD", "Water", 2);

	m_pMesh->AddNotify("IdleU", "Water", 0.5f);
	m_pMesh->AddNotify("IdleD", "Water", 0.5f);
	m_pMesh->AddNotify("IdleR", "Water", 0.5f);
	m_pMesh->AddNotify("IdleRU", "Water", 0.5f);
	m_pMesh->AddNotify("IdleRD", "Water", 0.5f);

	m_pMesh->AddNotify("DieU",	"Die", 0.5f);
	m_pMesh->AddNotify("DieD",	"Die", 0.5f);
	m_pMesh->AddNotify("DieR",	"Die", 0.5f);
	m_pMesh->AddNotify("DieRU", "Die", 0.5f);
	m_pMesh->AddNotify("DieRD", "Die", 0.5f);

	/*m_pMesh->AddCallBack("SwimU", "Water", this, &CPlayer::Water);
	m_pMesh->AddCallBack("SwimRU", "Water", this, &CPlayer::Water);
	m_pMesh->AddCallBack("SwimR", "Water", this, &CPlayer::Water);
	m_pMesh->AddCallBack("SwimRD", "Water", this, &CPlayer::Water);
	m_pMesh->AddCallBack("SwimD", "Water", this, &CPlayer::Water);

	m_pMesh->AddCallBack("IdleU", "Water", this, &CPlayer::Water);
	m_pMesh->AddCallBack("IdleRU", "Water", this, &CPlayer::Water);
	m_pMesh->AddCallBack("IdleR", "Water", this, &CPlayer::Water);
	m_pMesh->AddCallBack("IdleRD", "Water", this, &CPlayer::Water);
	m_pMesh->AddCallBack("IdleD", "Water", this, &CPlayer::Water);*/

	m_pMesh->AddCallBack("DieU",	"Die", this, &CPlayer::Die);
	m_pMesh->AddCallBack("DieRU",	"Die", this, &CPlayer::Die);
	m_pMesh->AddCallBack("DieR",	"Die", this, &CPlayer::Die);
	m_pMesh->AddCallBack("DieRD",	"Die", this, &CPlayer::Die);
	m_pMesh->AddCallBack("DieD",	"Die", this, &CPlayer::Die);

	m_pMesh->SetEndFunc("RollU", this, &CPlayer::RollEnd);
	m_pMesh->SetEndFunc("RollRU", this, &CPlayer::RollEnd);
	m_pMesh->SetEndFunc("RollR", this, &CPlayer::RollEnd);
	m_pMesh->SetEndFunc("RollRD", this, &CPlayer::RollEnd);
	m_pMesh->SetEndFunc("RollD", this, &CPlayer::RollEnd);

	m_pMesh->SetEndFunc("SwimU", this, &CPlayer::RollEnd);
	m_pMesh->SetEndFunc("SwimRU", this, &CPlayer::RollEnd);
	m_pMesh->SetEndFunc("SwimR", this, &CPlayer::RollEnd);
	m_pMesh->SetEndFunc("SwimRD", this, &CPlayer::RollEnd);
	m_pMesh->SetEndFunc("SwimD", this, &CPlayer::RollEnd);

	const char* strDir[5] =
	{
		"WalkU",
		"WalkRU",
		"WalkR",
		"WalkRD",
		"WalkD"
	};

	for (int i = 0; i < 5; ++i)
	{
		//m_pMesh->AddNotify(strDir[i], "Water", 2);
		//m_pMesh->AddCallBack(strDir[i], "Water", this, &CPlayer::Water);

		for (int j = 0; j < 8; ++j)
		{
			char strNum[32] = {};

			sprintf_s(strNum, "Step%d", j + 1);

			m_pMesh->AddNotify(strDir[i], strNum, j+1);
			m_pMesh->AddCallBack(strDir[i], strNum, this, &CPlayer::Step);
		}
	}

	m_pMesh->AddCallBack("WalkU", "Step1", this, &CPlayer::Die);
	m_pMesh->AddCallBack("DieD", "Die", this, &CPlayer::Die);

	m_pMesh->SetRelativeScale(16.f, 16.f, 1.f);
	m_pMesh->SetPivot(0.5f, 0.5f, 0.f);

	SetRootComponent(m_pMesh);

	GET_SINGLE(CInput)->AddAxisKey("Up&Down", DIK_UP, 1.f);
	GET_SINGLE(CInput)->AddAxisKey("Up&Down", DIK_DOWN, -1.f);
	GET_SINGLE(CInput)->AddAxisKey("Left&Right", DIK_LEFT, -1.f);
	GET_SINGLE(CInput)->AddAxisKey("Left&Right", DIK_RIGHT, 1.f);

	m_pInput->SetAxisFunc("Up&Down", this, &CPlayer::MoveV);
	m_pInput->SetAxisFunc("Left&Right", this, &CPlayer::MoveH);
	m_pInput->SetActionFunc("LeftArrow", KEY_TYPE::KT_DOWN, this, &CPlayer::MenuLeft);
	m_pInput->SetActionFunc("RightArrow", KEY_TYPE::KT_DOWN, this, &CPlayer::MenuRight);
	m_pInput->SetActionFunc("C", KEY_TYPE::KT_DOWN, this, &CPlayer::CloseUp);
	m_pInput->SetActionFunc("C", KEY_TYPE::KT_PRESS, this, &CPlayer::ReturnArrow);
	m_pInput->SetActionFunc("C", KEY_TYPE::KT_UP, this, &CPlayer::CloseUpEnd);
	m_pInput->SetActionFunc("X", KEY_TYPE::KT_DOWN, this, &CPlayer::ShiftDown);
	m_pInput->SetActionFunc("X", KEY_TYPE::KT_UP, this, &CPlayer::ShiftUp);
	m_pInput->SetActionFunc("ESC", KEY_TYPE::KT_DOWN, this, &CPlayer::Option);
	m_pInput->SetActionFunc("TAB", KEY_TYPE::KT_DOWN, this, &CPlayer::TileMatToggle);
	m_pInput->SetActionFunc("F1", KEY_TYPE::KT_DOWN, this, &CPlayer::Stage1);
	m_pInput->SetActionFunc("F2", KEY_TYPE::KT_DOWN, this, &CPlayer::Stage2);
	m_pInput->SetActionFunc("F3", KEY_TYPE::KT_DOWN, this, &CPlayer::Stage3);

	CLayer* pLayer = GET_SINGLE(CSceneManager)->GetNextScene()->FindLayer("Default");

	m_pCoord = CreateComponent<CUIFont>("Coord", m_pLayer);

	m_pCoord->SetRect(0.f, 20.f, 500.f, 0.f);

	m_pMesh->AddChild(m_pCoord);

	m_pSnd = CreateComponent<CSound>("Sound");

	m_pSnd->SetSound("arrow_retrieval");
	m_pSnd->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	m_pSnd->SetMaxVol(100.f);
	m_pSnd->SetMinVol(100.f);
	m_pSnd->SetVol(100.f);

	m_pMesh->AddChild(m_pSnd);

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	pCam->SetWorldPos(m_pMesh->GetWorldPos());

	pCam->SetTarget(this);

	SAFE_RELEASE(pCam);

	CLayer* pUI = m_pScene->FindLayer("UI");

	Resolution tRS = RESOLUTION;

	m_pShadowDown = CreateComponent<CSpriteComponent>("ShadowDown", m_pScene->FindLayer("Ground"));

	m_pShadowDown->SetWorldScale(16.f, 16.f, 0.f);
	m_pShadowDown->SetPivot(0.5f, 0.5f, 0.f);
	m_pShadowDown->SetInheritScale(false);
	m_pShadowDown->CreateSprite("Idle", "ArrowShadow", LOOP_OPTION::LOOP);
	m_pShadowDown->SetRelativePos(0.f, -8.f, 0.f);
	m_pShadowDown->AddRenderState("AlphaBlend");
	m_pShadowDown->AddRenderState("DepthNoWrite");
	m_pShadowDown->AddRenderState("NoCullBack");

	CMaterial* pShadowDownMtrl = m_pShadowDown->GetMaterial();

	pShadowDownMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.5f);

	SAFE_RELEASE(pShadowDownMtrl);

	m_pMesh->AddChild(m_pShadowDown);

	m_pGrass = CreateComponent<CParticle>("GrassParticle", m_pLayer);

	m_pGrass->SetWorldScale(1.f, 1.f, 1.f);
	m_pGrass->AddParticle("Grass");
	m_pGrass->AddParticle("GrassShare");
	m_pGrass->SetUVStart(64.f, 16.f);
	m_pGrass->SetUVEnd(80.f, 32.f);
	m_pGrass->SetUVSize(256.f, 256.f);
	m_pGrass->SetSpawnLimit(0.25f);
	m_pGrass->SetInheritScale(false);
	m_pGrass->AddRenderState("DepthNoWrite");
	m_pGrass->SetInheritRotZ(false);
	m_pGrass->Stop();

	CMaterial* pGrassMtrl = m_pGrass->GetMaterial();

	pGrassMtrl->SetTexture(REGISTER_TYPE::RT_DIF, "Effect", 0, 1,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	SAFE_RELEASE(pGrassMtrl);

	m_pShadowDown->AddChild(m_pGrass);

	m_pTileDust = CreateComponent<CParticle>("TileDustParticle", m_pLayer);

	m_pTileDust->SetWorldScale(1.f, 1.f, 1.f);
	m_pTileDust->AddParticle("TileDust");
	m_pTileDust->AddParticle("TileDustShare");
	m_pTileDust->SetUVStart(144.f, 0.f);
	m_pTileDust->SetUVEnd(160.f, 16.f);
	m_pTileDust->SetUVSize(256.f, 256.f);
	m_pTileDust->SetSpawnLimit(0.25f);
	m_pTileDust->SetInheritScale(false);
	m_pTileDust->AddRenderState("DepthNoWrite");
	m_pTileDust->SetInheritRotZ(false);
	m_pTileDust->Stop();

	CMaterial* pTileMtrl = m_pTileDust->GetMaterial();

	pTileMtrl->SetTexture(REGISTER_TYPE::RT_DIF, "Effect", 0, 1,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	SAFE_RELEASE(pTileMtrl);

	m_pShadowDown->AddChild(m_pTileDust);

	CParticle* pBlueDust = CreateComponent<CParticle>("BlueDust", m_pLayer);

	pBlueDust->SetWorldScale(1.f, 1.f, 1.f);
	pBlueDust->AddParticle("BlueDust");
	pBlueDust->AddParticle("BlueDustShare");
	pBlueDust->SetUVStart(144.f, 0.f);
	pBlueDust->SetUVEnd(160.f, 16.f);
	pBlueDust->SetUVSize(256.f, 256.f);
	pBlueDust->SetSpawnLimit(0.25f);
	pBlueDust->SetInheritScale(false);
	pBlueDust->AddRenderState("DepthNoWrite");
	pBlueDust->SetInheritRotZ(false);
	pBlueDust->Stop();

	pTileMtrl = pBlueDust->GetMaterial();

	pTileMtrl->SetTexture(REGISTER_TYPE::RT_DIF, "Effect", 0, 1,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	SAFE_RELEASE(pTileMtrl);

	m_pShadowDown->AddChild(pBlueDust);

	SAFE_RELEASE(pBlueDust);

	m_pWave = CreateComponent<CParticle>("WaveParticle", m_pLayer);

	m_pWave->SetWorldScale(1.f, 1.f, 1.f);
	m_pWave->AddParticle("Water");
	m_pWave->AddParticle("WaterShare");
	m_pWave->SetUVStart(352.f, 0.f);
	m_pWave->SetUVEnd(384.f, 32.f);
	m_pWave->SetUVSize(512.f, 512.f);
	m_pWave->SetSpawnLimit(0.25f);
	m_pWave->SetInheritScale(false);
	m_pWave->AddRenderState("DepthNoWrite");
	m_pWave->SetInheritRotZ(false);
	m_pWave->Stop();

	CMaterial* pWaveMtrl = m_pWave->GetMaterial();

	pWaveMtrl->SetTexture(REGISTER_TYPE::RT_DIF, "Pro", 0, 1,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	SAFE_RELEASE(pWaveMtrl);

	m_pMesh->AddChild(m_pWave);

	m_pSplash = CreateComponent<CParticle>("SplashParticle", m_pLayer);

	m_pSplash->SetWorldScale(1.f, 1.f, 1.f);
	m_pSplash->AddParticle("WaterSplash");
	m_pSplash->AddParticle("WaterSplashShare");
	m_pSplash->SetUVStart(0.f, 48.f);
	m_pSplash->SetUVEnd(16.f, 64.f);
	m_pSplash->SetUVSize(256.f, 256.f);
	m_pSplash->SetSpawnLimit(0.f);
	m_pSplash->SetInheritScale(false);
	m_pSplash->AddRenderState("DepthNoWrite");
	m_pSplash->SetInheritRotZ(false);
	m_pSplash->Stop();

	CMaterial* pSplashMtrl = m_pSplash->GetMaterial();

	pSplashMtrl->SetTexture(REGISTER_TYPE::RT_DIF, "Effect", 0, 1,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	SAFE_RELEASE(pSplashMtrl);

	m_pMesh->AddChild(m_pSplash);

	m_pBlood = CreateComponent<CParticle>("BloodParticle", m_pScene->FindLayer("Temp"));

	m_pBlood->SetWorldScale(1.f, 1.f, 1.f);
	m_pBlood->AddParticle("Blood");
	m_pBlood->AddParticle("BloodShare");
	m_pBlood->SetUVStart(0.f, 48.f);
	m_pBlood->SetUVEnd(16.f, 64.f);
	m_pBlood->SetUVSize(256.f, 256.f);
	m_pBlood->SetSpawnLimit(0.f);
	m_pBlood->SetInheritScale(false);
	m_pBlood->AddRenderState("DepthNoWrite");
	m_pBlood->SetInheritRotZ(false);
	m_pBlood->Stop();

	CMaterial* pBloodMtrl = m_pBlood->GetMaterial();

	pBloodMtrl->SetTexture(REGISTER_TYPE::RT_DIF, "Effect", 0, 1,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	SAFE_RELEASE(pBloodMtrl);

	m_pMesh->AddChild(m_pBlood);

	m_pBGM = CreateComponent<CSound>("BGM", m_pLayer);

	m_pBGM->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

	m_pMesh->AddChild(m_pBGM);

	m_vMovePos = GetWorldPos();

	return true;
}

void CPlayer::Start()
{
	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	pCam->SetWorldPos(m_pMesh->GetWorldPos());

	pCam->SetTarget(this);

	SAFE_RELEASE(pCam);

	CLayer* pMtrlLayer = m_pScene->FindLayer("1_MAT");

	if (pMtrlLayer)
	{
		SAFE_RELEASE(m_pMtrlMap);
		m_pMtrlMap = pMtrlLayer->GetTileMap();
	}

	CLayer* pColLayer = m_pScene->FindLayer("1_COL");

	if (pColLayer)
	{
		SAFE_RELEASE(m_pTileMap);
		m_pTileMap = pColLayer->GetTileMap();
	}

	CLayer* pAirLayer = m_pScene->FindLayer("1_COLA");

	if (pAirLayer)
	{
		SAFE_RELEASE(m_pAirMap);
		m_pAirMap = pAirLayer->GetTileMap();
	}

	CLayer* pAnimLayer = m_pScene->FindLayer("1_BG2");

	if (pAnimLayer)
	{
		SAFE_RELEASE(m_pDebugMap);
		m_pDebugMap = pAnimLayer->GetTileMap();
	}

	m_bStart = true;
}

void CPlayer::Input(float fTime)
{
}

void CPlayer::Update(float fTime)
{
	CObj::Update(fTime);

	m_fPlayTime += fTime;

#ifdef _DEBUG

	if (m_pBullet)
	{
		OutputDebugStringA("Bullet");
	}

	else
	{
		OutputDebugStringA("NoBullet");
	}

	if (m_bCharge)
	{
		OutputDebugStringA("Charge\n");
	}

	else
	{
		OutputDebugStringA("Not Charge\n");
	}

#endif

	TCHAR strPos[32] = {};

	Vector3 vPos = GetWorldPos();
	SetWorldPos(vPos.x, vPos.y, -300.f);
	swprintf_s(strPos, TEXT("X: %.5f, Y: %.5f"), vPos.x, vPos.y);

	m_pCoord->SetText(strPos);
	m_pCoord->SetWorldPos(980.f, 5.f, 0.f);

	m_pBow->SetWorldPos(vPos);
	m_pArrow->SetWorldPos(vPos);

	if (m_eState == STATE::DIE)
	{
		Die(fTime);
		return;
	}

	if (m_bWalkIn)
	{
		if (m_bRolling)
		{
			m_bRolling = false;
			m_pGrass->SetSpawnLimit(0.25f);
			m_pTileDust->SetSpawnLimit(0.25f);
		}

		m_fWalkTime += fTime;

		if (m_fWalkTime >= 1.5f)
		{
			CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

			pCam->SetZoom(false);

			SAFE_RELEASE(pCam);
			ChangeStage(m_strMap.c_str());
		}

		SetState(STATE::WALK);

		m_bIdleEnable = false;

		CTileMode* pMode = (CTileMode*)m_pScene->GetGameMode();

		Vector3 vDir = pMode->GetDoorDir();

		m_pMesh->AddWorldPos(-vDir * 20.f * fTime);
	}

	else if(m_fWalkTime > 0.f)
	{
		m_fWalkTime -= fTime;

		if (m_fWalkTime < 0.f)
		{
			m_fWalkTime = 0.f;
		}
	}

	else if (!m_bRolling)
	{
		if (m_fScaleX == -1.f)	//	걷는 상태
		{
			if (m_fScaleY == 1.f)
			{
				m_eDir = DIR_8::LU;
				if (m_bIdleEnable)
				{
					if (m_eState != STATE::SWIM &&
						m_eState != STATE::RUN)
					{
						SetState(STATE::WALK);
					}

					m_fBowAngle = 45.f;
				}
				else
				{
					Attack(0.f, fTime);
				}
			}

			else if (m_fScaleY == -1.f)
			{
				m_eDir = DIR_8::LD;
				if (m_bIdleEnable)
				{
					if (m_eState != STATE::SWIM &&
						m_eState != STATE::RUN)
					{
						SetState(STATE::WALK);
					}

					m_fBowAngle = 135.f;
				}
				else
				{
					Attack(0.f, fTime);
				}
			}

			else
			{
				m_eDir = DIR_8::L;
				if (m_bIdleEnable)
				{
					if (m_eState != STATE::SWIM &&
						m_eState != STATE::RUN)
					{
						SetState(STATE::WALK);
					}

					m_fBowAngle = 90.f;
				}
				else
				{
					Attack(0.f, fTime);
				}
			}
		}

		else if (m_fScaleX == 1.f)
		{
			if (m_fScaleY == 1.f)
			{
				m_eDir = DIR_8::RU;
				if (m_bIdleEnable)
				{
					if (m_eState != STATE::SWIM &&
						m_eState != STATE::RUN)
					{
						SetState(STATE::WALK);
					}

					m_fBowAngle = -45.f;
				}
				else
				{
					Attack(0.f, fTime);
				}
			}

			else if (m_fScaleY == -1.f)
			{
				m_eDir = DIR_8::RD;
				if (m_bIdleEnable)
				{
					if (m_eState != STATE::SWIM &&
						m_eState != STATE::RUN)
					{
						SetState(STATE::WALK);
					}

					m_fBowAngle = -135.f;
				}
				else
				{
					Attack(0.f, fTime);
				}					
			}

			else
			{
				m_eDir = DIR_8::R;
				if (m_bIdleEnable)
				{
					if (m_eState != STATE::SWIM &&
						m_eState != STATE::RUN)
					{
						SetState(STATE::WALK);
					}

					m_fBowAngle = -90.f;
				}
				else
				{
					Attack(0.f, fTime);
				}
					
			}
		}

		else
		{
			if (m_fScaleY == 1.f)
			{
				m_eDir = DIR_8::U;
				if (m_bIdleEnable)
				{
					if (m_eState != STATE::SWIM &&
						m_eState != STATE::RUN)
					{
						SetState(STATE::WALK);
					}
						
					m_fBowAngle = 0.f;
				}
				else
				{
					Attack(0.f, fTime);
				}
			}

			else if (m_fScaleY == -1.f)
			{
				m_eDir = DIR_8::D;
				if (m_bIdleEnable)
				{
					if (m_eState != STATE::SWIM &&
						m_eState != STATE::RUN)
					{
						SetState(STATE::WALK);
					}

					m_fBowAngle = 180.f;
				}
				else
				{
					Attack(0.f, fTime);
				}
			}

			else
			{
				if (m_bIdleEnable)
				{
					m_pShadow->ReturnClip();
					m_pMesh->ReturnClip();
					m_pBow->ReturnClip();
					m_pArrow->ReturnClip();
				}
			}
		}
	}

	else if (!m_bBack)// 구르는 상태
	{
		m_fRollTime += fTime;

		float fSpeed = m_fWalkSpeed * 2.f * (1 - 0.5f * m_bSlow);

		if (m_eState == STATE::SWIM)
		{
			fSpeed = m_fWalkSpeed;
		}

		Vector3 vPrevPos = m_pMesh->GetWorldPos();

		switch (m_eDir)
		{
		case DIR_8::U:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * fSpeed * fTime);
			break;
		case DIR_8::RU:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * fSpeed * fTime * 0.7071f);
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * fSpeed * fTime * 0.7071f);
			break;
		case DIR_8::R:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * fSpeed * fTime);
			break;
		case DIR_8::RD:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * fSpeed * fTime * 0.7071f);
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * fSpeed * fTime * -0.7071f);
			break;
		case DIR_8::D:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * fSpeed * fTime * -1.f);
			break;
		case DIR_8::LD:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * fSpeed * fTime * 0.7071f);
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * fSpeed * fTime * -0.7071f);
			break;
		case DIR_8::L:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * fSpeed * fTime);
			break;
		case DIR_8::LU:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * fSpeed * fTime * 0.7071f);
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * fSpeed * fTime * 0.7071f);
			break;
		}

		if (m_cCol == 0)
		{
			m_cCol = TileMapCol(m_pTileMap);
		}

		if (m_cCol != 0)
		{
			m_pMesh->SetWorldPos(vPrevPos);
			m_bBack = true;

			LookBack();

			CTileMode* pMode = (CTileMode*)m_pScene->GetGameMode();

			pMode->ShakeCam();

			SetState(m_eState);
		}

		else
		{
			m_cCol = TileMapCol(m_pAirMap);

			if (m_cCol != 0)
			{
				m_pMesh->SetWorldPos(vPrevPos);
				m_bBack = true;

				LookBack();

				CTileMode* pMode = (CTileMode*)m_pScene->GetGameMode();

				pMode->ShakeCam();

				SetState(m_eState);
			}

			else
			{
				m_cCol = TileMapCol(m_pMtrlMap);

				if ((m_cCol == '4' &&
					(m_eDir == DIR_8::LU ||
						m_eDir == DIR_8::RU ||
						m_eDir == DIR_8::U)) ||
					(m_cCol == '1' &&
						(m_eDir == DIR_8::R ||
							m_eDir == DIR_8::RU ||
							m_eDir == DIR_8::U)))
				{
					m_pMesh->SetWorldPos(vPrevPos);
					m_bBack = true;

					LookBack();

					CTileMode* pMode = (CTileMode*)m_pScene->GetGameMode();

					pMode->ShakeCam();

					SetState(m_eState);
				}
			}
		}

		m_cCol = 0;
	}

	if (m_bBack)	// 넉백
	{
		m_fBackTime += fTime;

		if (m_fBackTime >= m_fBackLimit)
		{
			m_fBackTime = 0.f;

			m_bBack = false;

			m_bRolling = false;

			m_pGrass->SetSpawnLimit(0.25f);
			m_pTileDust->SetSpawnLimit(0.25f);

#ifdef _DEBUG
			OutputDebugStringA("Stop Rolling");
#endif

			m_bIdleEnable = true;
		}

		float fSpeed = m_fWalkSpeed * 2.f / 5.f;

		Vector3 vPrevPos = m_pMesh->GetWorldPos();

		switch (m_eDir)
		{
		case DIR_8::U:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * fSpeed * fTime);
			break;
		case DIR_8::RU:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * fSpeed * fTime * 0.7071f);
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * fSpeed * fTime * 0.7071f);
			break;
		case DIR_8::R:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * fSpeed * fTime);
			break;
		case DIR_8::RD:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * fSpeed * fTime * 0.7071f);
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * fSpeed * fTime * -0.7071f);
			break;
		case DIR_8::D:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * fSpeed * fTime * -0.7071f);
			break;
		case DIR_8::LD:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * fSpeed * fTime * 0.7071f);
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * fSpeed * fTime * -0.7071f);
			break;
		case DIR_8::L:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * fSpeed * fTime);
			break;
		case DIR_8::LU:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * fSpeed * fTime* 0.7071f);
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * fSpeed * fTime* 0.7071f);
			break;
		}

		char cCol = TileMapCol(m_pTileMap);

		if (cCol != 0)
		{
			m_pMesh->SetWorldPos(vPrevPos);
			m_bBack = true;

			LookBack();

			SetState(m_eState);
		}
	}

	if (m_fRollTime >= 0.f)
	{
		m_fRollTime -= fTime;
	}

	if (m_bFly)
	{
		((CTileMode*)m_pScene->GetGameMode())->SetFadeColor(1.f, 1.f, 1.f);

		m_fFlyDist += fTime * 8.f;

		CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

		pCam->SetZoom(true, m_fFlyDist / 512.f);
		pCam->SetMovePos(GetWorldPos());

		SAFE_RELEASE(pCam);

		AddWorldPos(0.f, fTime * 8.f, 0.f);

		if (m_fFlyDist >= 16.f)
		{
			static bool bCheck = false;

			if (!bCheck)
			{
				m_pSnd->SetSoundAndPlay("Ascension");
				m_pSnd->SetSoundAndPlay("IntroLayer2");
				bCheck = true;
			}
		}

		m_pShadowDown->SetRelativePos(0.f, -m_fFlyDist - 8.f, 0.f);

		CSoul* pSoul = (CSoul*)m_pLayer->FindObj("Soul1");

		float fScale = (0.5f - pSoul->GetScale()) * 2.f;

		SAFE_RELEASE(pSoul);

		if (m_iSouls < 6)
		{
			CMaterial* pMtrl = m_pMesh->GetMaterial();

			pMtrl->SetAmbientColor(fScale, fScale, fScale, 0.f);

			SAFE_RELEASE(pMtrl);
		}

		m_pMesh->SetPlayRate(fScale * 3.f + 1.f);
	}

	else if (m_fFlyDist > 0.f)
	{
		AddWorldPos(0.f, fTime * -24.f, 0.f);

		m_pShadowDown->SetRelativePos(0.f, -m_fFlyDist - 8.f, 0.f);

		m_fFlyDist -= fTime * 24.f;

		if (m_fFlyDist <= 8.f)
		{
			bool bShadow = m_pShadow->IsEnable();

			if (!bShadow)
			{
				ChangeSequenceAll("Randing");
				SetDefaultClipAll("Randing");

				m_pMesh->DeleteRenderState("ForeGround");
				m_pMesh->AddRenderState("Character");
				m_pShadow->Enable(true);

				m_pBow->Enable(true);
			}
		}

		else
		{
			ChangeSequenceAll("RandingIdle");
		}
	}

	else if (m_fFlyDist < 0.f)
	{
		m_fFlyDist = 0.f;
		ChangeSequenceAll("IdleD");
		SetDefaultClipAll("IdleD");
		m_bCharge = true;
		((CTileMode*)m_pScene->GetGameMode())->Lock(false);
	}

	if (m_iSouls >= 6)
	{
		m_fLightTime += fTime;

		if (m_fLightTime >= m_fLightTimeLimit)
		{
			m_fLightTime -= m_fLightTimeLimit;

			m_iLights++;

			CLine* pLine = CreateComponent<CLine>("Line", m_pScene->FindLayer("Speacial"));

			pLine->SetAngle((float)(rand() % 360));
			pLine->SetAngleSpeed((float)(120 * (1 - (rand() % 2) * 2)));
			pLine->AddRenderState("DepthNoWrite");
			pLine->AddRenderState("NoCullBack");
			pLine->SetPivot(0.5f, 0.5f, 0.f);
			pLine->SetWorldScale(400.f, 400.f, 0.f);
			pLine->SetInheritScale(false);
			pLine->Update(fTime);

			CMaterial* pLineMtrl = pLine->GetMaterial();

			pLineMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.5f);

			SAFE_RELEASE(pLineMtrl);

			m_pMesh->AddChild(pLine);

			SAFE_RELEASE(pLine);

			((CTileMode*)m_pScene->GetGameMode())->ShakeCam(150.f + m_iLights * 20.f, 5.f + m_iLights /2.f, 0.1f);

			if (m_iLights == 3)
			{
				m_fLightTimeLimit = 0.25f;
			}
			else if (m_iLights == 8)
			{
				m_fLightTimeLimit = 0.0625f;
			}

			else if(m_iLights == 12)
			{
				m_fLightTimeLimit = 0.03125;
				((CTileMode*)m_pScene->GetGameMode())->ShakeCam(300.f, 10.f, 0.5f);

				((CTileMode*)m_pScene->GetGameMode())->SetFadeColor(1.f, 1.f, 1.f);
				((CTileMode*)m_pScene->GetGameMode())->SetFade(0.f, 1.f, 0.5f);
			}
		}

		if (m_iLights >= 12)
		{
			m_fWhiteTime += fTime;

			if (m_fWhiteTime >= m_fWhiteTimeLimit)
			{
				m_fWhiteTime = 0.f;

				ChangeSequenceAll("RandingIdle");
				SetDefaultClipAll("RandingIdle");

				m_bFly = false;

				CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

				pCam->SetZoomDist(0.f);
				pCam->SetZoom(false);

				SAFE_RELEASE(pCam);

				((CTileMode*)m_pScene->GetGameMode())->SetFadeColor(1.f, 1.f, 1.f);
				((CTileMode*)m_pScene->GetGameMode())->SetFade(1.f, 0.f, -0.5f);

				CMaterial* pMtrl = m_pMesh->GetMaterial();

				pMtrl->SetAmbientColor(0.f, 0.f, 0.f, 0.f);

				SAFE_RELEASE(pMtrl);

				CSceneComponent* pLine = FindComByType<CLine>();

				while (pLine)
				{
					m_pMesh->DeleteChild(pLine);

					pLine->Destroy();

					SAFE_RELEASE(pLine);

					pLine = FindComByType<CLine>();
				}

				m_iLights = 0;
				m_iSouls = 0;

				m_pBGM->SetSound("HeavyDrumLoop");
				m_pBGM->Stop();

				CSceneComponent* pDust = FindSceneComponent("Dust");

				m_pMesh->DeleteChild(pDust);

				if (pDust)
				{
					pDust->Destroy();
				}

				SAFE_RELEASE(pDust);
			}
		}

		else
		{
			ChangeSequenceAll("TurnIdle");
			SetDefaultClipAll("TurnIdle");
		}
	}

	if (m_bFly && m_iLights < 12)
	{
		((CTileMode*)m_pScene->GetGameMode())->SetFade(0.f, 0.f, 0.f);
	}

	if (m_bSlow)
	{
		m_fSlowTime += fTime;

		if (m_fSlowTime >= 2.5f)
		{
			m_bSlow = false;
			m_fSlowTime = 0.f;

			m_fSpeed *= 2.f;

			CSpriteComponent* pCom = (CSpriteComponent*)FindSceneComponent("Mucus");

			if (pCom)
			{
				pCom->Destroy();
			}

			SAFE_RELEASE(pCom);
		}

		else if (m_fSlowTime >= 2.f)
		{
			CSpriteComponent* pCom = (CSpriteComponent*)FindSceneComponent("Mucus");

			CMaterial* pMtrl = pCom->GetMaterial();

			pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 1.f - (m_fSlowTime - 2.f) * 2.f);

			SAFE_RELEASE(pMtrl);

			SAFE_RELEASE(pCom);
		}
	}

	m_pCoord->Enable(GET_SINGLE(CEngine)->IsImgui());
}

void CPlayer::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);

	char cCol = TileMapCol(m_pTileMap);

	if (cCol != 0)
	{
		if (m_bStair)
		{
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * -m_fSpeed * fTime * m_fScaleX);
			AddWorldPos(-m_fScaleX * m_fSpeed * fTime, -m_fScaleY * m_fSpeed * fTime, 0.f);
		}

		else
		{
			AddWorldPos(-m_fScaleX * m_fSpeed * fTime, -m_fScaleY * m_fSpeed * fTime, 0.f);
		}
	}

	else if( m_pAirMap)
	{
		cCol = TileMapCol(m_pAirMap);

		if (cCol != 0)
		{
			if (m_bStair)
			{
				AddWorldPos(-m_fScaleX * m_fSpeed * fTime, -m_fScaleY * m_fSpeed * fTime, 0.f);
				m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * -m_fSpeed * fTime * m_fScaleX);
			}
			else
			{
				AddWorldPos(-m_fScaleX * m_fSpeed * fTime, -m_fScaleY * m_fSpeed * fTime, 0.f);
			}
		}
	}

	char cMtrl = TileMapCol(m_pMtrlMap);

	char strTile[(int)MAT_TYPE::END] =
	{
		(char)atoi("4098"),
		(char)atoi("4099"),
		(char)atoi("4100"),
		(char)atoi("4101"),
		(char)atoi("4102"),
		(char)atoi("4103"),
		(char)atoi("4104"),
		(char)atoi("4105"),
		(char)atoi("4106"),
		(char)atoi("4107"),
		(char)atoi("4108"),
		(char)atoi("4109"),
		(char)atoi("4110"),
		(char)atoi("4111"),
		(char)atoi("4112"),
		(char)atoi("4116"),
		(char)atoi("4118"),
		(char)atoi("4119"),
		(char)atoi("4125"),
		(char)atoi("4145"),
		(char)atoi("4146"),
		(char)atoi("4147"),
		(char)atoi("4148"),
		(char)atoi("4149"),
		(char)atoi("4140"),
		(char)atoi("4141"),
		(char)atoi("4142"),
		(char)atoi("4153"),
		(char)atoi("4158")
	};

	if (cMtrl == strTile[(int)MAT_TYPE::WATER])
	{
		if (m_eState != STATE::SWIM)
		{
			Water(0.f);

			m_pSplash->Resume();
			m_pSplash->SetSpawnCount(16);
			m_pSplash->SetSpawnLimit(0.f);

			m_pMesh->AddRelativePos(0.f, -8.f, 0.f);
		}

		m_pShadowDown->Enable(false);
		SetState(STATE::SWIM);
	}

	else if (m_eState == STATE::SWIM)
	{
		Water(0.f);

		m_pShadowDown->Enable(true);
		SetState(STATE::IDLE);
		m_fSpeed = m_fWalkSpeed;

		m_pMesh->AddRelativePos(0.f, 8.f, 0.f);
	}

	for (int i = 0; i < (int)MAT_TYPE::END; ++i)
	{
		if (cMtrl == strTile[i])
		{
			m_eMatType = (MAT_TYPE)i;
			break;
		}
	}
}

void CPlayer::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CPlayer::PreRender(float fTime)
{
	m_pGrass->Stop();
	m_pWave->Stop();
	m_pTileDust->Stop();

	CParticle* pBlue = (CParticle*)FindSceneComponent("BlueDust");

	pBlue->Stop();

	if (m_eMatType == MAT_TYPE::WATER_2)
	{
		m_pSplash->Stop();
	}

	if (m_fScaleX != 0.f ||
		m_fScaleY != 0.f ||
		m_bRolling)
	{
		switch (m_eMatType)
		{
		case MAT_TYPE::GRASS:
		case MAT_TYPE::GREEN_GRASS:
			m_pGrass->Resume();
			break;
		case MAT_TYPE::WATER:
			m_pWave->Resume();
			break;
		case MAT_TYPE::WATER_2:
			m_pSplash->Resume();

			if (!m_bRolling)
			{
				m_pSplash->SetSpawnLimit(0.3f);
			}

			m_pSplash->SetSpawnCount(-1);
			
			break;
		case MAT_TYPE::TILE:
			m_pTileDust->Resume();
			break;
		case MAT_TYPE::TILE_BLUE:
			pBlue->Resume();
			break;
		}

		if (m_fScaleX != 0 &&
			m_fScaleY != 0 && 
			m_bIdleEnable)
		{
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * -m_fSpeed * fTime * m_fScaleY * 0.2921f);
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * -m_fSpeed * fTime * m_fScaleX * m_fScaleX * 0.2921f);
		}

		m_fScaleX = 0.f;
		m_fScaleY = 0.f;
	}

	SAFE_RELEASE(pBlue);

	CObj::PreRender(fTime);
}

void CPlayer::Render(float fTime)
{
	CObj::Render(fTime);
}

void CPlayer::PostRender(float fTime)
{
	CObj::PostRender(fTime);

}

CPlayer* CPlayer::Clone()
{
	return new CPlayer(*this);
}

void CPlayer::Save(FILE* pFile)
{
	CObj::Save(pFile);

	int iLength = (int)m_pMesh->GetName().length();

	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_pMesh->GetName().c_str(), 1, iLength, pFile);
}

void CPlayer::Load(FILE* pFile)
{
	CObj::Load(pFile);

	SAFE_RELEASE(m_pMesh);

	int iLength = 0;
	char strTag[256] = {};

	fread(&iLength, 4, 1, pFile);
	if (iLength > 0)
	{
		fread(strTag, 1, iLength, pFile);
	}

	m_pMesh = (CSpriteComponent*)FindSceneComponent(strTag);

	m_pInput->SetAxisFunc("MoveV", this, &CPlayer::MoveV);
	m_pInput->SetAxisFunc("MoveH", this, &CPlayer::MoveH);
}

void CPlayer::MoveV(float fScale, float fTime)
{
	if (m_eState == STATE::DIE ||
		m_bFly ||
		m_fFlyDist !=0.f ||
		m_bBack ||
		m_bMenu ||
		m_bRolling)
	{
		return;
	}

	if (m_bIdleEnable && !m_bRolling)
	{
		char cCol = TileMapCol(m_pMtrlMap);

		if (cCol == '4')
		{
			m_bStair = true;
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * m_fSpeed * fTime * (fScale - abs(fScale) * 0.25f));
		}

		else
		{
			m_bStair = false;
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * m_fSpeed * fTime * fScale);
		}
	}

	if (fScale != 0.f)
	{
		SetState(m_eState);
	}

	m_fScaleY = fScale;
}

void CPlayer::MoveH(float fScale, float fTime)
{
	if (m_eState == STATE::DIE ||
		m_bFly ||
		m_fFlyDist != 0.f ||
		m_bBack ||
		m_bMenu ||
		m_bRolling)
	{
		return;
	}

	m_fScaleX = fScale;

	if (m_fScaleX != 0.f)
	{
		SetWorldRotYAll(-m_fScaleX * 90.f + 90.f);
		SetState(m_eState);
	}

	else if (m_fScaleY == 0.f &&  m_eState == STATE::RUN)
	{
		SetState(STATE::IDLE);
	}

	if (m_bIdleEnable && !m_bRolling)
	{
		char cCol = TileMapCol(m_pMtrlMap);

		if (cCol == '1')
		{
			m_bStair = true;
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * m_fSpeed * fTime * fScale * fScale);
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * m_fSpeed * fTime * fScale);
		}

		else
		{
			m_bStair = false;
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * m_fSpeed * fTime * fScale * fScale);
		}
	}
}

void CPlayer::Attack(float fPushTime, float fTime)
{
	if (m_bRolling ||
		m_bMenu)
	{
		return;
	}

	m_bIdleEnable = false;

	if (m_bCharge)
	{
		CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

		pCam->SetZoom(true);

		if (!m_pBullet)
		{
			m_pBullet = (CBullet*)m_pScene->CreateCloneObj("Bullet", "Bullet", m_pLayer, m_pScene->GetSceneType());
			m_pBullet->SetSpeed(0.f);
		}

		float fDist = m_pBullet->GetWorldScale().x * m_pBullet->GetPivot().x + GetWorldScale().x * GetPivot().x + 1.f;

		pCam->SetTarget(m_pBullet);

		SAFE_RELEASE(pCam);

		m_pBullet->SetRelativePos(GetWorldPos() + Vector3(fDist * -sinf(DirectX::XMConvertToRadians(m_fBowAngle)),
			fDist * cosf(DirectX::XMConvertToRadians(m_fBowAngle)), 0.f));
		m_pBullet->SetWorldRotZ(m_fBowAngle);

		if (m_fBowAngle > 180.f)
		{
			m_fBowAngle -= 360.f;
		}

		else if (m_fBowAngle < -180.f)
		{
			m_fBowAngle += 360.f;
		}

		switch (m_eDir)
		{
		case DIR_8::U:
			SetWorldRotYAll(0.f);
			ChangeSequenceAll("AttackU");
			SetDefaultClipAll("IdleU");

			m_fBowAngle = 0.f;

			break;
		case DIR_8::RU:
			SetWorldRotYAll(0.f);
			ChangeSequenceAll("AttackRU");
			SetDefaultClipAll("IdleRU");

			m_fBowAngle = -45.f;

			break;
		case DIR_8::R:
			SetWorldRotYAll(0.f);
			ChangeSequenceAll("AttackR");
			SetDefaultClipAll("IdleR");

			m_fBowAngle = -90.f;

			break;
		case DIR_8::RD:
			SetWorldRotYAll(0.f);
			ChangeSequenceAll("AttackRD");
			SetDefaultClipAll("IdleRD");

			m_fBowAngle = -135.f;

			break;
		case DIR_8::D:
			SetWorldRotYAll(0.f);
			ChangeSequenceAll("AttackD");
			SetDefaultClipAll("IdleD");

			m_fBowAngle = 180.f;

			break;
		case DIR_8::LD:
			SetWorldRotYAll(180.f);
			ChangeSequenceAll("AttackRD");
			SetDefaultClipAll("IdleRD");

			m_fBowAngle = 135.f;

			break;
		case DIR_8::L:
			SetWorldRotYAll(180.f);
			ChangeSequenceAll("AttackR");
			SetDefaultClipAll("IdleR");

			m_fBowAngle = 90.f;
			break;
		case DIR_8::LU:
			SetWorldRotYAll(180.f);
			ChangeSequenceAll("AttackRU");
			SetDefaultClipAll("IdleRU");

			m_fBowAngle = 45.f;
			break;
		}
	}

	else
	{	
		switch (m_eDir)
		{
		case DIR_8::U:
			SetWorldRotYAll(0.f);
			ChangeSequenceAll("CallU");
			SetDefaultClipAll("IdleU");
			break;
		case DIR_8::RU:
			SetWorldRotYAll(0.f);
			ChangeSequenceAll("CallRU");
			SetDefaultClipAll("IdleRU");
			break;
		case DIR_8::R:
			SetWorldRotYAll(0.f);
			ChangeSequenceAll("CallR");
			SetDefaultClipAll("IdleR");
			break;
		case DIR_8::RD:
			SetWorldRotYAll(0.f);
			ChangeSequenceAll("CallRD");
			SetDefaultClipAll("IdleRD");
			break;
		case DIR_8::D:
			SetWorldRotYAll(0.f);
			ChangeSequenceAll("CallD");
			SetDefaultClipAll("IdleD");
			break;
		case DIR_8::LD:
			SetWorldRotYAll(180.f);
			ChangeSequenceAll("CallRD");
			SetDefaultClipAll("IdleRD");
			break;
		case DIR_8::L:
			SetWorldRotYAll(180.f);
			ChangeSequenceAll("CallR");
			SetDefaultClipAll("IdleR");
			break;
		case DIR_8::LU:
			SetWorldRotYAll(180.f);
			ChangeSequenceAll("CallRU");
			SetDefaultClipAll("IdleRU");
			break;
		}
	}
}

void CPlayer::AttackFire(float fPushTime, float fTime)
{
	if (m_bRolling ||
		m_bMenu)
	{
		return;
	}

	m_bIdleEnable = true;

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	pCam->SetZoom(false);
	pCam->SetTarget(this);

	SAFE_RELEASE(pCam);

	if (!m_pBullet)
	{
		return;
	}

	m_pArrow->Enable(false);

#ifdef _DEBUG
	char strTime[32] = {};
	sprintf_s(strTime, "PT: %.5f\n", m_fPushTime);
	OutputDebugStringA(strTime);
#endif

	if (m_pBullet)
	{
		if (m_fPushTime >= 0.25f)
		{
			m_pBullet->SetDist(0.f);
			m_pBullet->SetSpeed(1000.f * m_fPushTime);

			m_bCharge = false;

			CSound* pSnd = m_pBullet->FindComByType<CSound>();

			pSnd->SetSound("Arrow_windloop");
			pSnd->Play(fTime);

			SAFE_RELEASE(pSnd);

			m_pSnd->SetSound("arrow_shoot");
			m_pSnd->SetVol(1.f);
			m_pSnd->Play(fTime);
		}
		else
		{
			m_pBullet->Destroy();
		}
	}

	SAFE_RELEASE(m_pBullet);
}

void CPlayer::ShiftDown(float fPushTime, float fTime)
{
	if (m_bRolling ||
		m_eState == STATE::DIE ||
		m_fRollTime > 0.f ||
		!m_bIdleEnable ||
		m_bFly ||
		m_fFlyDist != 0.f ||
		m_bMenu)
	{
		return;
	}

	m_fRollTime = 0.5f;

	SetState(STATE::ROLL);
}

void CPlayer::ShiftUp(float fPushTIme, float fTime)
{
	if (m_bMenu)
	{
		Option(0.f, 0.f);
		return;
	}

	m_fSpeed = m_fWalkSpeed *(1 - m_bSlow * 0.5f);

	if (!m_bRolling)
	{
		SetState(STATE::WALK);
	}
}

float CPlayer::GetAngle()
{
	Vector3 vCurAxis = GetWorldAxis(WORLD_AXIS::AXIS_Y);
	Vector3 vAxis = Vector3::Axis[(int)WORLD_AXIS::AXIS_Y];

	float fCos = vAxis.Dot(vCurAxis);
	float fTheta = acosf(fCos);

	if (vCurAxis.x)
		fTheta *= -1.f;

	return fTheta;
}

void CPlayer::ReturnArrow(float fPushTime, float fTime)
{
	if (m_bFly || m_bRolling)
	{
		return;
	}

	m_fPushTime = fPushTime;

	if (m_bCharge)
	{
		if (!m_pBullet && m_bReady)
		{
			Attack(fPushTime, fTime);
		}

		Vector3 vPos = GetWorldPos();

		CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

		switch (m_eDir)
		{
		case DIR_8::U:
			pCam->SetMovePos(vPos + Vector3(0.f, 50.f, 0.f));
			break;
		case DIR_8::RU:
			pCam->SetMovePos(vPos + Vector3(50.f / sqrtf(2), 50.f / sqrtf(2), 0.f));
			break;
		case DIR_8::R:
			pCam->SetMovePos(vPos + Vector3(50.f, 0.f, 0.f));
			break;
		case DIR_8::RD:
			pCam->SetMovePos(vPos + Vector3(50.f / sqrtf(2), -50.f / sqrtf(2), 0.f));
			break;
		case DIR_8::D:
			pCam->SetMovePos(vPos + Vector3(0.f, -50.f, 0.f));
			break;
		case DIR_8::LD:
			pCam->SetMovePos(vPos + Vector3(-50.f / sqrtf(2), -50.f / sqrtf(2), 0.f));
			break;
		case DIR_8::L:
			pCam->SetMovePos(vPos + Vector3(-50.f, 0.f, 0.f));
			break;
		case DIR_8::LU:
			pCam->SetMovePos(vPos + Vector3(-50.f / sqrtf(2), 50.f / sqrtf(2), 0.f));
			break;
		}

		SAFE_RELEASE(pCam);

		return;
	}

	CLayer* pLayer = m_pScene->FindLayer("Default");
	CObj* pArw = pLayer->FindObj("Bullet");

	if (pArw)
	{
		float fSpeed = ((CBullet*)pArw)->GetSpeed();

		if (fSpeed == 0.f)
		{
			if (((CBullet*)pArw)->IsFixed())
			{
				static float fFixTimeMax = 2.f;

				if (m_fPushTime >= fFixTimeMax)
				{
					FADE tFade = {};

					tFade.fGray = 0.f;
					
					GET_SINGLE(CShaderManager)->UpdateCBuffer("Fade", &tFade);

					((CBullet*)pArw)->SetFix(false);

					CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

					pCam->SetZoom(false);

					SAFE_RELEASE(pCam);

					Vector3 vPos = pArw->GetWorldPos();

					pArw->Destroy();

					vPos.z = 0.f;

					CSoulMonster* pMon = (CSoulMonster*)((CBullet*)pArw)->GetFixObj();

					pMon->SetState(State::DIE);

					SAFE_RELEASE(pMon);

					for (int i = 0; i < 6; ++i)
					{
						char strName[32] = {};

						sprintf_s(strName, "Soul%d", i);

						CObj* pObj = m_pScene->CreateCloneObj(strName, "Soul", m_pLayer, m_pScene->GetSceneType());						

						pObj->SetWorldPos(vPos);
						pObj->SetWorldRotZ(10.f * i + 60.f);

						((CSoul*)pObj)->SetTarget(this);
						((CSoul*)pObj)->SetTargetPos(vPos);
						((CSoul*)pObj)->SetOffset(i - 2.5f);

						SAFE_RELEASE(pObj);

						((CTileMode*)m_pScene->GetGameMode())->ShakeCam(150.f, 5.f, 3.f);
					}

					m_pSnd->SetSoundAndPlay("Release");

					m_pSnd->SetSoundAndPlay("IntroLayer1");

					m_pBow->Enable(false);
				}

				else
				{
					static bool bCheck = false;

					if (!bCheck)
					{
						bCheck = true;
						m_pBGM->SetSoundAndPlay("LightDrumLoop");
					}

					CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

					pCam->SetZoom(true, 0.0625f);

					SAFE_RELEASE(pCam);

					((CTileMode*)m_pScene->GetGameMode())->SetFadeColor(0.f, 0.f, 0.f);
					((CTileMode*)m_pScene->GetGameMode())->SetFade(fPushTime / 4.f, fPushTime / 4.f, 0.f);
				}
			}

			else
			{
				if (fPushTime < 2.f)
				{
					((CTileMode*)m_pScene->GetGameMode())->SetFadeColor(0.f, 0.f, 0.f);
					((CTileMode*)m_pScene->GetGameMode())->SetFade(fPushTime / 4.f, fPushTime / 4.f, 0.f);
				}

				Vector3 vPos = GetWorldPos();

				Vector3 vArwPos = pArw->GetWorldPos();

				Vector3 vDir = vPos - vArwPos;

				Vector3 vAxisY = Vector3(0.f, 1.f, 0.f);

				vDir.Normalize();

				/*vArwPos += vDir * 100.f * fTime * (fPushTime + 1.f) * (fPushTime + 1.f);

				char cCol = m_pTileMap->GetTileCol(Vector2(vArwPos.x, vArwPos.y));

				if (cCol == 0.f)
				{
					int iCol = ((CBullet*)pArw)->GetCol();

					if (iCol == 0)
					{
						pArw->AddWorldPos(vDir * 100.f * fTime * (fPushTime + 1.f) * (fPushTime + 1.f));
					}
				}*/

				((CBullet*)pArw)->SetSpeed(100.f);

				float fDot = vAxisY.Dot(vDir);

				float fAngle = DirectX::XMConvertToDegrees(acosf(fDot));

				if (vDir.x > 0.f)
					fAngle *= -1.f;

				pArw->SetWorldRotZ(fAngle);
			}
		}

		else
		{
			if (fPushTime < 2.f)
			{
				((CTileMode*)m_pScene->GetGameMode())->SetFadeColor(0.f, 0.f, 0.f);
				((CTileMode*)m_pScene->GetGameMode())->SetFade(fPushTime / 4.f, fPushTime / 4.f, 0.f);
			}

			Vector3 vPos = GetWorldPos();

			Vector3 vArwPos = pArw->GetWorldPos();

			Vector3 vDir = vPos - vArwPos;

			Vector3 vAxisY = Vector3(0.f, 1.f, 0.f);

			vDir.Normalize();

			((CBullet*)pArw)->AddSpeed((1700.f) * fTime);

			float fDot = vAxisY.Dot(vDir);

			float fAngle = DirectX::XMConvertToDegrees(acosf(fDot));

			if (vDir.x > 0.f)
				fAngle *= -1.f;

			pArw->SetWorldRotZ(fAngle);
		}
	}

	SAFE_RELEASE(pArw);
}

void CPlayer::RollEnd()
{
	if (m_bRolling)
	{
		m_bIdleEnable = true;
		m_bRolling = false;

		m_pGrass->SetSpawnLimit(0.2f);
		m_pTileDust->SetSpawnLimit(0.2f);
		CParticle* pBlue = (CParticle*)FindSceneComponent("BlueDust");

		pBlue->SetSpawnLimit(0.2f);

		SAFE_RELEASE(pBlue);

		if (m_fSpeed == m_fWalkSpeed * 2.f * (1.f - m_bSlow * 0.5f))
		{
			m_fSpeed = m_fWalkSpeed * 1.5f * (1.f - m_bSlow * 0.5f);
			SetState(STATE::RUN);
		}			

		else if (m_fSpeed == m_fWalkSpeed && m_eState == STATE::SWIM)
			m_fSpeed = m_fWalkSpeed * 0.75f;
	}
}

void CPlayer::ColStart(CCollider* pSrc, CCollider* pDst, float fTime)
{
	std::string strSrc = pSrc->GetName();

	std::string strDst = pDst->GetName();

	if (strDst == "JAM/colossus" || 
		strDst == "JAM/brainfreeze" ||
		strDst == "JAM/eyecube" ||
		strDst == "JAM/JAM" ||
		strDst == "JAM/jam")
	{
		if (((CTileMode*)m_pScene->GetGameMode())->GetLock())
			return;

		m_strMap = strDst + ".tmx";

		m_bWalkIn = true;
		((CTileMode*)m_pScene->GetGameMode())->SetFadeColor(0.f, 0.f, 0.f);
		((CTileMode*)m_pScene->GetGameMode())->SetFade(0.f, 1.f, 2.f / 3.f);
		CObj* pObj = m_pScene->FindLayer("Default")->FindObj("music_area");

		if (pObj)
		{
			CSound* pSnd = (CSound*)pObj->FindSceneComponent("Hub1");

			pSnd->SetFade(true, false);

			SAFE_RELEASE(pSnd);

			pObj->Release();
		}

		BOSS_TYPE eType = ((CTileMode*)m_pScene->GetGameMode())->GetType();

		if (eType == BOSS_TYPE::NONE)
		{
			m_pSnd->SetSoundAndPlay("EnterDoor");
		}
	}

	else if (strDst == "chamber")
	{
		m_strMap = "Ending";

		m_bWalkIn = true;

		((CTileMode*)m_pScene->GetGameMode())->SetFadeColor(0.f, 0.f, 0.f);
		((CTileMode*)m_pScene->GetGameMode())->SetFade(0.f, 1.f, 2.f / 3.f);

		CObj* pObj = m_pScene->FindLayer("Default")->FindObj("music_area");

		if (pObj)
		{
			CSound* pSnd = (CSound*)pObj->FindSceneComponent("Hub1");

			pSnd->SetFade(true, false);

			SAFE_RELEASE(pSnd);

			pObj->Release();
		}
	}

	else if (strDst == "RightHandBody" ||
		strDst == "LeftHandBody")
	{
		CColossus* pCol = (CColossus*)pDst->GetObj();

		ATTACK_STAGE eStage = pCol->GetStage();

		if (eStage == ATTACK_STAGE::DOWN)
		{
			SetState(STATE::DIE);
		}

		else
		{
			if (m_fScaleX != 0.f)
			{
				m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * -m_fSpeed * fTime * m_fScaleX * m_fScaleX);
			}

			if (m_fScaleY != 0.f)
			{
				m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * -m_fSpeed * fTime * m_fScaleY);
			}
		}

		m_cCol = -1;
	}

	else if (strDst == "CubeBody")
	{
		CEyecube* pEye = (CEyecube*)pDst->GetObj();

		if (!pEye->IsTurned())
		{
			SetState(STATE::DIE);
		}

		else
		{
			if (m_fScaleX != 0.f)
			{
				m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * -m_fSpeed * fTime * m_fScaleX * m_fScaleX);
			}

			if (m_fScaleY != 0.f)
			{
				m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * -m_fSpeed * fTime * m_fScaleY);
			}
		}

		m_cCol = -1;
	}

	else if (strDst == "IceBody" ||
		strDst == "BrainBody")
	{

		CBrainFreeze* pBrain = (CBrainFreeze*)pDst->GetObj();

		State eState = pBrain->GetState();

		if (eState == State::ATTACK)
		{
			if (strDst == "IceBody")
			{
				m_bImpact = true;
			}

			SetState(STATE::DIE);
		}

		else
		{
			if (m_fScaleX != 0.f)
			{
				m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * -m_fSpeed * fTime * m_fScaleX * m_fScaleX);
			}

			if (m_fScaleY != 0.f)
			{
				m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * -m_fSpeed * fTime * m_fScaleY);
			}
		}
		m_cCol = -1;
	}

	else if (strDst == "FireBody")
	{
		SetBurn();
		SetState(STATE::DIE);
	}

	else if (strDst == "DoorLBody" ||
		strDst == "DoorRBody")
	{
		if (m_fScaleX != 0.f)
		{
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * -m_fSpeed * fTime * m_fScaleX * m_fScaleX);
		}

		if (m_fScaleY != 0.f)
		{
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * -m_fSpeed * fTime * m_fScaleY);
		}

		m_cCol = -1;
	}

	else if (strDst == "MucusBody")
	{
		Mucus();
	}
}

void CPlayer::ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
{
	std::string strDst = pDst->GetName();

	if (strDst == "LeftHandBody" ||
		strDst == "RightHandBody")
	{
		RectInfo tInfo = ((CColliderRect*)pSrc)->GetInfo();

		Vector3 vCross = pSrc->GetCross();

		Vector3 vPos = GetWorldPos();

		if (m_fScaleX != 0.f)
		{
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * -m_fSpeed * fTime * m_fScaleX * m_fScaleX);
		}

		if (m_fScaleY != 0.f)
		{
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * -m_fSpeed * fTime * m_fScaleY);
		}

		m_cCol = -1;
	}

	else if (strDst == "CubeBody")
	{
		CEyecube* pEye = (CEyecube*)pDst->GetObj();

			if (m_fScaleX != 0.f)
			{
				m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * -m_fSpeed * fTime * m_fScaleX * m_fScaleX);
			}

			if (m_fScaleY != 0.f)
			{
				m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * -m_fSpeed * fTime * m_fScaleY);
			}

			m_cCol = -1;
	}

	else if (strDst == "IceBody" ||
		strDst == "BrainBody")
	{
		CBrainFreeze* pBrain = (CBrainFreeze*)pDst->GetObj();

		State eState = pBrain->GetState();

		if (eState == State::ATTACK)
		{
			SetState(STATE::DIE);
		}

		else
		{
			if (m_fScaleX != 0.f)
			{
				m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * -m_fSpeed * fTime * m_fScaleX * m_fScaleX);
			}

			if (m_fScaleY != 0.f)
			{
				m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * -m_fSpeed * fTime * m_fScaleY);
			}
		}

		m_cCol = -1;
	}

	else if (strDst == "FireBody")
	{
		SetBurn();
		SetState(STATE::DIE);
	}

	else if (strDst == "DoorLBody" ||
		strDst == "DoorRBody")
	{
		if (m_fScaleX != 0.f)
		{
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * -m_fSpeed * fTime * m_fScaleX * m_fScaleX);
		}

		if (m_fScaleY != 0.f)
		{
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * -m_fSpeed * fTime * m_fScaleY);
		}

		m_cCol = -1;
	}
}

void CPlayer::ColEnd(CCollider* pSrc, CCollider* pDst, float fTime)
{
	m_cCol = 0;
}

void CPlayer::Option(float fPushTime, float fTime)
{
	CLayer * pUI = m_pScene->FindLayer("UI");

	CObj* pObj = pUI->FindObj("Button");

	if (pObj)
	{
		bool bEnable = pObj->IsEnable();

		m_bMenu = !bEnable;

		pObj->Enable(!bEnable);

		if (!bEnable)
		{
			((CTileMode*)m_pScene->GetGameMode())->SetFadeColor(0.f, 0.f, 0.f);
			((CTileMode*)m_pScene->GetGameMode())->SetFade(0.f, 0.5f, 1.f);
		}

		else
		{
			((CTileMode*)m_pScene->GetGameMode())->SetFadeColor(0.f, 0.f, 0.f);
			((CTileMode*)m_pScene->GetGameMode())->SetFade(0.5f, 0.f, -1.f);
		}
	}

	SAFE_RELEASE(pObj);
}

void CPlayer::TileMatToggle(float fScale, float fTime)
{
	CLayer* pLayer = m_pScene->FindLayer("1_COL");

	if (pLayer)
	{
		CObj* pObj = pLayer->FindObj("1_COL");

		CSceneComponent* pCom = pObj->FindSceneComponent("1_COL");

		pCom->Enable(!pCom->IsEnable());

		SAFE_RELEASE(pCom);

		SAFE_RELEASE(pObj);
	}

	pLayer = m_pScene->FindLayer("1_COLA");

	if (pLayer)
	{
		CObj* pObj = pLayer->FindObj("1_COLA");

		CSceneComponent* pCom = pObj->FindSceneComponent("1_COLA");

		pCom->Enable(!pCom->IsEnable());

		SAFE_RELEASE(pCom);

		SAFE_RELEASE(pObj);
	}

	pLayer = m_pScene->FindLayer("2_COL");

	if (pLayer)
	{
		CObj* pObj = pLayer->FindObj("2_COL");

		CSceneComponent* pCom = pObj->FindSceneComponent("2_COL");

		pCom->Enable(!pCom->IsEnable());

		SAFE_RELEASE(pCom);

		SAFE_RELEASE(pObj);
	}

	pLayer = m_pScene->FindLayer("1_MAT");

	if (pLayer)
	{
		CObj* pObj = pLayer->FindObj("1_MAT");

		CSceneComponent* pCom = pObj->FindSceneComponent("1_MAT");

		pCom->Enable(!pCom->IsEnable());

		SAFE_RELEASE(pCom);

		SAFE_RELEASE(pObj);
	}
}

void CPlayer::Stage1(float fScale, float fTime)
{
	ChangeStage("JAM\\colossus.tmx");
}

void CPlayer::Stage2(float fScale, float fTime)
{
	ChangeStage("JAM\\brainfreeze.tmx");
}

void CPlayer::Stage3(float fScale, float fTime)
{
	ChangeStage("JAM\\eyecube.tmx");
}

void CPlayer::CloseUp(float fScale, float fTime)
{
	if (m_bMenu)
	{
		if (m_iMenu == 0)
		{
			Option(0.f, 0.f);
		}

		else if (m_iMenu == 1)
		{

		}
		else
		{
			m_bMenu = false;

			m_pBGM->SetSound("Hub1");
			m_pBGM->Stop();

			GET_SINGLE(CBossManager)->Save();

			GET_SINGLE(CSceneManager)->CreateNextScene();

			GET_SINGLE(CSceneManager)->SetGameMode<CStartGameMode>(false);

			return;
		}
	}

	if (m_bFly ||
		m_bRolling)
	{
		return;
	}

	if (m_bCharge)
	{
		CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

		pCam->SetZoom(true);

		m_pSnd->SetSoundAndPlay("arrow_charge");

		Vector3 vPos = GetWorldPos();

		switch (m_eDir)
		{
		case DIR_8::U:
			pCam->SetMovePos(vPos + Vector3(0.f, 100.f, 0.f));
			break;
		case DIR_8::RU:
			pCam->SetMovePos(vPos + Vector3(70.7f, 70.7f, 0.f));
			break;
		case DIR_8::R:
			pCam->SetMovePos(vPos + Vector3(100.f, 0.f, 0.f));
			break;
		case DIR_8::RD:
			pCam->SetMovePos(vPos + Vector3(70.7f, -70.7f, 0.f));
			break;
		case DIR_8::D:
			pCam->SetMovePos(vPos + Vector3(0.f, -100.f, 0.f));
			break;
		case DIR_8::LD:
			pCam->SetMovePos(vPos + Vector3(-70.7f, -70.7f, 0.f));
			break;
		case DIR_8::L:
			pCam->SetMovePos(vPos + Vector3(-100.f, 0.f, 0.f));
			break;
		case DIR_8::LU:
			pCam->SetMovePos(vPos + Vector3(-70.7f, 70.7f, 0.f));
			break;
		}

		SAFE_RELEASE(pCam);
	}

	else
	{
		m_pSnd->SetSoundAndPlay("arrow_retrieval");
	}

	Attack(fScale, fTime);
}

void CPlayer::CloseUpEnd(float fScale, float fTime)
{
	if (m_bFly)
	{
		return;
	}

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	pCam->SetZoom(false);

	m_pSnd->SetSound("arrow_charge");
	m_pSnd->Stop();

	m_pSnd->SetSound("arrow_retrieval");
	m_pSnd->Stop();

	SAFE_RELEASE(pCam);

	((CTileMode*)m_pScene->GetGameMode())->SetFade(0.f, 0.f, 0.f);
	
	AttackFire(fScale, fTime);

	m_bReady = true;
}

char CPlayer::TileMapCol(CTileMap* pMap)
{
	if (!pMap)
		return 0;

	Vector2 vExtent = m_pRC->GetExtent();

	Vector3 vScale = Vector3(vExtent.x, vExtent.y, 0.f);
	Vector3 vPos = GetWorldPos() - GetPivot() * vScale + Vector3(8.f, 8.f, 0.f);

	if (m_eState == STATE::SWIM)
	{
		vPos += Vector3(0.f, 8.f, 0.f);
	}

	char cCol = pMap->GetTileCol(Vector2(vPos.x, vPos.y));

	if (cCol == 0)
	{
		cCol = pMap->GetTileCol(Vector2(vPos.x, vPos.y) + Vector2(0.f, vScale.y));

		if (cCol == 0)
		{
			cCol = pMap->GetTileCol(Vector2(vPos.x, vPos.y) + Vector2(vScale.x, 0.f));

			if (cCol == 0)
			{
				cCol = pMap->GetTileCol(Vector2(vPos.x, vPos.y) + Vector2(vScale.x, vScale.y));
			}
		}
	}

	return cCol;
}

void CPlayer::ChangeSequenceAll(const char* pSeq)
{
	m_pShadow->ChangeSequence(pSeq);
	m_pMesh->ChangeSequence(pSeq);
	m_pBow->ChangeClip(pSeq);
	m_pArrow->ChangeClip(pSeq);
}

void CPlayer::SetDefaultClipAll(const char* pSeq)
{
	m_pShadow->SetDefaultSeq(pSeq);
	m_pMesh->SetDefaultSeq(pSeq);
	m_pBow->SetDefaultClip(pSeq);
	m_pArrow->SetDefaultClip(pSeq);
}

void CPlayer::SetWorldRotYAll(float fRot)
{
	m_pShadow->SetWorldRotY(fRot);
	m_pMesh->SetWorldRotY(fRot);
	m_pBow->SetWorldRotY(fRot);
	m_pArrow->SetWorldRotY(fRot);
}

void CPlayer::SetState(STATE eState)
{
	if (m_bRolling && m_eState != eState && !m_bBack)
	{
		m_bRolling = false;
		m_bIdleEnable = true;
		//m_fSpeed = m_fPrevSpeed;
	}

	if (m_eState == STATE::DIE &&
		eState != STATE::DIE)
		return;

	STATE ePrevState = m_eState;

	m_eState = eState;

	switch (m_eState)
	{
	case STATE::IDLE:
	{
		switch (m_eDir)
		{
		case DIR_8::U:
		{
			SetDefaultClipAll("IdleU");
		}
		break;
		case DIR_8::RU:
		{
			SetDefaultClipAll("IdleRU");
		}
		break;
		case DIR_8::R:
		{
			SetDefaultClipAll("IdleR");
		}
		break;
		case DIR_8::RD:
		{
			SetDefaultClipAll("IdleRD");
		}
		break;
		case DIR_8::D:
		{
			SetDefaultClipAll("IdleD");
		}
		break;
		case DIR_8::LD:
		{
			SetDefaultClipAll("IdleRD");
		}
		break;
		case DIR_8::L:
		{
			SetDefaultClipAll("IdleR");
		}
		break;
		case DIR_8::LU:
		{
			SetDefaultClipAll("IdleRU");
		}
		break;
		}
	}
		break;
	case STATE::WALK:
	{
		switch (m_eDir)
		{
		case DIR_8::U:
		{
			ChangeSequenceAll("WalkU");
			SetDefaultClipAll("IdleU");
		}
		break;
		case DIR_8::RU:
		{
			ChangeSequenceAll("WalkRU");
			SetDefaultClipAll("IdleRU");
		}
		break;
		case DIR_8::R:
		{
			ChangeSequenceAll("WalkR");
			SetDefaultClipAll("IdleR");
		}
		break;
		case DIR_8::RD:
		{
			ChangeSequenceAll("WalkRD");
			SetDefaultClipAll("IdleRD");
		}
		break;
		case DIR_8::D:
		{
			ChangeSequenceAll("WalkD");
			SetDefaultClipAll("IdleD");
		}
		break;
		case DIR_8::LD:
		{
			ChangeSequenceAll("WalkRD");
			SetDefaultClipAll("IdleRD");
		}
		break;
		case DIR_8::L:
		{
			ChangeSequenceAll("WalkR");
			SetDefaultClipAll("IdleR");
		}
		break;
		case DIR_8::LU:
		{
			ChangeSequenceAll("WalkRU");
			SetDefaultClipAll("IdleRU");
		}
		break;
		}
	}
		break;
	case STATE::ROLL:
	{
		m_fPrevSpeed = m_fSpeed;
		m_fSpeed *= 2.f;

		if (ePrevState == STATE::SWIM)
		{
			m_bRolling = true;
			m_bIdleEnable = false;
			Water(0.f);
		}

		else
		{
			m_pSnd->SetSound("Roll");

			m_pSnd->Play(0.f);

			m_pGrass->SetSpawnLimit(0.01f);
			m_pTileDust->SetSpawnLimit(0.01f);

			if (m_eMatType == MAT_TYPE::WATER_2)
			{
				m_pSplash->SetSpawnLimit(0.03f);
			}

			CParticle* pBlue = (CParticle*)FindSceneComponent("BlueDust");

			pBlue->SetSpawnLimit(0.01f);

			SAFE_RELEASE(pBlue);

			m_bRolling = true;
			m_bIdleEnable = false;

			switch (m_eDir)
			{
			case DIR_8::U:
				SetWorldRotYAll(0.f);
				ChangeSequenceAll("RollU");
				break;
			case DIR_8::RU:
				SetWorldRotYAll(0.f);
				ChangeSequenceAll("RollRU");
				break;
			case DIR_8::R:
				SetWorldRotYAll(0.f);
				ChangeSequenceAll("RollR");
				break;
			case DIR_8::RD:
				SetWorldRotYAll(0.f);
				ChangeSequenceAll("RollRD");
				break;
			case DIR_8::D:
				SetWorldRotYAll(0.f);
				ChangeSequenceAll("RollD");
				break;
			case DIR_8::LD:
				SetWorldRotYAll(180.f);
				ChangeSequenceAll("RollRD");
				break;
			case DIR_8::L:
				SetWorldRotYAll(180.f);
				ChangeSequenceAll("RollR");
				break;
			case DIR_8::LU:
				SetWorldRotYAll(180.f);
				ChangeSequenceAll("RollRU");
				break;
			}
		}

		m_eState = ePrevState;
	}
		break;
	case STATE::SWIM:
	{
		m_fSpeed = m_fWalkSpeed / 2.f;
		switch (m_eDir)
		{
		case DIR_8::U:
		{
			ChangeSequenceAll("SwimU");
			SetDefaultClipAll("SwimU");
		}
		break;
		case DIR_8::RU:
		{
			ChangeSequenceAll("SwimRU");
			SetDefaultClipAll("SwimRU");
		}
		break;
		case DIR_8::R:
		{
			ChangeSequenceAll("SwimR");
			SetDefaultClipAll("SwimR");
		}
		break;
		case DIR_8::RD:
		{
			ChangeSequenceAll("SwimRD");
			SetDefaultClipAll("SwimRD");
		}
		break;
		case DIR_8::D:
		{
			ChangeSequenceAll("SwimD");
			SetDefaultClipAll("SwimD");
		}
		break;
		case DIR_8::LD:
		{

			ChangeSequenceAll("SwimRD");
			SetDefaultClipAll("SwimRD");
		}
		break;
		case DIR_8::L:
		{
			ChangeSequenceAll("SwimR");
			SetDefaultClipAll("SwimR");
		}
		break;
		case DIR_8::LU:
		{
			ChangeSequenceAll("SwimRU");
			SetDefaultClipAll("SwimRU");
		}
		break;
		}
	}
		break;
	case STATE::ATTACK:
		break;
	case STATE::CALL:
		break;
	case STATE::DIE:
	{
		if (ePrevState != STATE::DIE)
		{
			m_fDieDelay = 1.f;
			((CTileMode*)m_pScene->GetGameMode())->SetFadeColor(0.f, 0.f, 0.f);

			if (!m_bBurn)
			{
				m_pSnd->SetSound("Crush");

				m_pBlood->Resume();
			}

			else if(m_bImpact)
			{
				m_pSnd->SetSound("Impact");
			}

			else
			{
				m_pSnd->SetSound("Laser");
			}

			m_pSnd->SetVol(0.5f);
			m_pSnd->Play(0.f);

			m_pBow->Enable(false);
			m_pArrow->Enable(false);
		}

		if (!m_bBurn)
		{
			switch (m_eDir)
			{
			case DIR_8::U:
			{
				ChangeSequenceAll("DieU");
				SetDefaultClipAll("DieU");
			}
			break;
			case DIR_8::RU:
			{
				ChangeSequenceAll("DieRU");
				SetDefaultClipAll("DieRU");
			}
			break;
			case DIR_8::R:
			{
				ChangeSequenceAll("DieR");
				SetDefaultClipAll("DieR");
			}
			break;
			case DIR_8::RD:
			{
				ChangeSequenceAll("DieRD");
				SetDefaultClipAll("DieRD");
			}
			break;
			case DIR_8::D:
			{
				ChangeSequenceAll("DieD");
				SetDefaultClipAll("DieD");
			}
			break;
			case DIR_8::LD:
			{

				ChangeSequenceAll("DieRD");
				SetDefaultClipAll("DieRD");
			}
			break;
			case DIR_8::L:
			{
				ChangeSequenceAll("DieR");
				SetDefaultClipAll("DieR");
			}
			break;
			case DIR_8::LU:
			{
				ChangeSequenceAll("DieRU");
				SetDefaultClipAll("DieRU");
			}
			break;
			}
		}

		else
		{
			CMaterial* pMtrl = m_pMesh->GetMaterial();

			pMtrl->SetDiffuseColor(0.f, 0.f, 0.f, 1.f);

			SAFE_RELEASE(pMtrl);

			switch (m_eDir)
			{
			case DIR_8::U:
			{
				ChangeSequenceAll("IdleU");
				SetDefaultClipAll("IdleU");
			}
			break;
			case DIR_8::RU:
			{
				ChangeSequenceAll("IdleRU");
				SetDefaultClipAll("IdleRU");
			}
			break;
			case DIR_8::R:
			{
				ChangeSequenceAll("IdleR");
				SetDefaultClipAll("IdleR");
			}
			break;
			case DIR_8::RD:
			{
				ChangeSequenceAll("IdleRD");
				SetDefaultClipAll("IdleRD");
			}
			break;
			case DIR_8::D:
			{
				ChangeSequenceAll("IdleD");
				SetDefaultClipAll("IdleD");
			}
			break;
			case DIR_8::LD:
			{

				ChangeSequenceAll("IdleRD");
				SetDefaultClipAll("IdleRD");
			}
			break;
			case DIR_8::L:
			{
				ChangeSequenceAll("IdleR");
				SetDefaultClipAll("IdleR");
			}
			break;
			case DIR_8::LU:
			{
				ChangeSequenceAll("IdleRU");
				SetDefaultClipAll("IdleRU");
			}
			break;
			}
		}
	}
		break;
	case STATE::RUN:
	{
		switch (m_eDir)
		{
		case DIR_8::U:
		{
			ChangeSequenceAll("RunU");
			SetDefaultClipAll("RunU");
		}
		break;
		case DIR_8::RU:
		{
			ChangeSequenceAll("RunRU");
			SetDefaultClipAll("RunRU");
		}
		break;
		case DIR_8::R:
		{
			ChangeSequenceAll("RunR");
			SetDefaultClipAll("RunR");
		}
		break;
		case DIR_8::RD:
		{
			ChangeSequenceAll("RunRD");
			SetDefaultClipAll("RunRD");
		}
		break;
		case DIR_8::D:
		{
			ChangeSequenceAll("RunD");
			SetDefaultClipAll("RunD");
		}
		break;
		case DIR_8::LD:
		{
			ChangeSequenceAll("RunRD");
			SetDefaultClipAll("RunRD");
		}
		break;
		case DIR_8::L:
		{
			ChangeSequenceAll("RunR");
			SetDefaultClipAll("RunR");
		}
		break;
		case DIR_8::LU:
		{
			ChangeSequenceAll("RunRU");
			SetDefaultClipAll("RunRU");
		}
		break;
		}
	}
		break;
	}
}

void CPlayer::Water(float fTime)
{
	char cMtrl = TileMapCol(m_pMtrlMap);

	char cWater = atoi("4116");

	char cHalfWater = atoi("4100");

	if (cWater == cMtrl ||
		cHalfWater == cMtrl)
	{
		CLayer* pGround = m_pScene->FindLayer("Ground");

		CObj* pWater = m_pScene->CreateCloneObj("Water", "Water", pGround, m_pScene->GetSceneType());

		pWater->SetWorldPos(GetWorldPos());

		if (cHalfWater == cMtrl)
		{
			pWater->AddWorldPos(Vector3(0.f, -GetWorldScale().y * GetPivot().y, 0.f));
		}

		CSpriteComponent* pCom = pWater->FindComByType<CSpriteComponent>();

		pCom->AddCallBack<CEffect>("Water", "Water", (CEffect*)pWater, &CEffect::Destroy);

		SAFE_RELEASE(pCom);

		SAFE_RELEASE(pWater);
	}
}

void CPlayer::LookBack()
{
	if (m_eDir != DIR_8::U && m_eDir != DIR_8::D)
	{
		Vector3 vRot = m_pMesh->GetWorldRot();

		SetWorldRotYAll(vRot.y + 180.f);
	}

	m_eDir = (DIR_8)(((int)m_eDir + 4)%(int)DIR_8::END);

	m_pSnd->SetSound("Roll");
	m_pSnd->Stop();

	m_pSnd->SetSoundAndPlay("Roll_bash");
}

void CPlayer::ChangeStage(const char* pStage)
{
	m_pBGM->SetSound("Hub1");
	m_pBGM->Stop();

	if (strcmp(pStage, "JAM/JAM.tmx") == 0 ||
		strcmp(pStage, "JAM/jam.tmx")== 0)
	{
		GET_SINGLE(CSceneManager)->CreateNextScene();

		GET_SINGLE(CSceneManager)->SetGameMode<CMainGameMode>(false);

		CObj* pObj = GET_SINGLE(CSceneManager)->GetNextScene()->GetGameMode()->GetPlayer();

		BOSS_TYPE eType = ((CTileMode*)m_pScene->GetGameMode())->GetType();

		switch (eType)
		{
		case BOSS_TYPE::COLOSSUS:
			pObj->SetWorldPos(2464.f, 944.f, 0.f);
			break;
		case BOSS_TYPE::EYECUBE:
			pObj->SetWorldPos(544.f, 1360.f, 0.f);
			break;
		case BOSS_TYPE::BRAINFREEZE:
			pObj->SetWorldPos(544.f, 448.f, 0.f);
			break;
		}

		((CPlayer*)pObj)->SetDir(DIR_8::D);

		SAFE_RELEASE(pObj);
	}

	else if (strcmp(pStage, "JAM\\brainfreeze.tmx") == 0 ||
		strcmp(pStage, "JAM/brainfreeze.tmx") == 0)
	{
		GET_SINGLE(CSceneManager)->CreateNextScene();

		GET_SINGLE(CSceneManager)->SetGameMode<CBrainMode>(false);

		CScene* pScene = GET_SINGLE(CSceneManager)->GetNextScene();

		CTileMode* pMode = (CTileMode*)pScene->GetGameMode();

		pMode->LoadXml(pStage, MAP_PATH);
	}

	else if (strcmp(pStage, "JAM\\colossus.tmx") == 0 ||
		strcmp(pStage, "JAM/colossus.tmx") == 0)
	{
		GET_SINGLE(CSceneManager)->CreateNextScene();

		GET_SINGLE(CSceneManager)->SetGameMode<CColossusMode>(false);

		CScene* pScene = GET_SINGLE(CSceneManager)->GetNextScene();

		CTileMode* pMode = (CTileMode*)pScene->GetGameMode();

		pMode->LoadXml(pStage, MAP_PATH);
	}

	else if (strcmp(pStage, "JAM\\eyecube.tmx") == 0 ||
		strcmp(pStage, "JAM/eyecube.tmx") == 0)
	{
		GET_SINGLE(CSceneManager)->CreateNextScene();

		GET_SINGLE(CSceneManager)->SetGameMode<CEyeCubeMode>(false);

		CScene* pScene = GET_SINGLE(CSceneManager)->GetNextScene();

		CTileMode* pMode = (CTileMode*)pScene->GetGameMode();

		pMode->LoadXml(pStage, MAP_PATH);
	}

	else if (strcmp(pStage, "Ending") == 0)
	{
		GET_SINGLE(CSceneManager)->CreateNextScene();

		GET_SINGLE(CSceneManager)->SetGameMode<CEndGameMode>(false);
	}

	else
	{
		GET_SINGLE(CSceneManager)->CreateNextScene();

		GET_SINGLE(CSceneManager)->SetGameMode<CTileMode>(false);

		CScene* pScene = GET_SINGLE(CSceneManager)->GetNextScene();

		CTileMode* pMode = (CTileMode*)pScene->GetGameMode();

		pMode->LoadXml(pStage, MAP_PATH);
	}
}

void CPlayer::Die(float fTime)
{
	if (m_fDieDelay > 0.f)
	{
		m_fDieDelay -= fTime;
		return;
	}

	m_fDieDelay -= fTime;

	((CTileMode*)m_pScene->GetGameMode())->SetFadeColor(0.f, 0.f, 0.f);
	((CTileMode*)m_pScene->GetGameMode())->FadeOut(fTime);

	if (m_fDieDelay <= -1.f)
	{		
		GET_SINGLE(CSceneManager)->CreateNextScene();

		GET_SINGLE(CSceneManager)->SetGameMode<CMainGameMode>(false);

		SetWorldPos(1376.f, 160.f * 16.f - 1359.f, 0.f);

		++m_iDeath;
	}
}

void CPlayer::Step(int iFrame, float fTime)
{
	char strMat[32] = {};

	switch (m_eMatType)
	{
	case MAT_TYPE::GRASS:
		sprintf_s(strMat, "GrassStep%d", iFrame + 1);
		break;
	case MAT_TYPE::DIRT:
		sprintf_s(strMat, "GloopStep%d", iFrame + 1);
		break;
	case MAT_TYPE::WATER_2:
		sprintf_s(strMat, "WaterStep%d", iFrame + 1);
		break;
	case MAT_TYPE::PURPLE:
		sprintf_s(strMat, "StoneStep%d", iFrame + 1);
		break;
	case MAT_TYPE::CROSS:
		sprintf_s(strMat, "StoneStep%d", iFrame + 1);
		break;
	case MAT_TYPE::FOREST:
		sprintf_s(strMat, "GrassStep%d", iFrame + 1);
		break;
	case MAT_TYPE::RHOMBUS:
		sprintf_s(strMat, "StoneStep%d", iFrame + 1);
		break;
	case MAT_TYPE::SNOW:
		sprintf_s(strMat, "SnowStep%d", iFrame + 1);
		break;
	case MAT_TYPE::ICE:
		sprintf_s(strMat, "IceStep%d", iFrame + 1);
		break;
	case MAT_TYPE::MUD:
		sprintf_s(strMat, "GloopStep%d", iFrame + 1);
		break;
	case MAT_TYPE::SOIL:
		sprintf_s(strMat, "SandStep%d", iFrame + 1);
		break;
	case MAT_TYPE::TILE:
		sprintf_s(strMat, "StoneStep%d", iFrame + 1);
		break;
	case MAT_TYPE::L:
		sprintf_s(strMat, "StoneStep%d", iFrame + 1);
		break;
	case MAT_TYPE::R:
		sprintf_s(strMat, "StoneStep%d", iFrame + 1);
		break;
	case MAT_TYPE::O:
		sprintf_s(strMat, "StoneStep%d", iFrame + 1);
		break;
	case MAT_TYPE::WATER:
		sprintf_s(strMat, "ShallowStep%d", iFrame + 1);
		break;
	case MAT_TYPE::MAGMA:
		sprintf_s(strMat, "GloopStep%d", iFrame + 1);
		break;
	case MAT_TYPE::GREEN_GRASS:
		sprintf_s(strMat, "GrassStep%d", iFrame + 1);
		break;
	case MAT_TYPE::TILE_BLUE:
		sprintf_s(strMat, "StoneStep%d", iFrame + 1);
		break;
	case MAT_TYPE::RIGHT_UP:
		sprintf_s(strMat, "StoneStep%d", iFrame + 1);
		break;
	case MAT_TYPE::LEFT_UP:
		sprintf_s(strMat, "StoneStep%d", iFrame + 1);
		break;
	case MAT_TYPE::UP_DOWN:
		sprintf_s(strMat, "StoneStep%d", iFrame + 1);
		break;
	case MAT_TYPE::UP:
		sprintf_s(strMat, "StoneStep%d", iFrame + 1);
		break;
	case MAT_TYPE::DOWN:
		sprintf_s(strMat, "StoneStep%d", iFrame + 1);
		break;
	case MAT_TYPE::RAIN_W:
		sprintf_s(strMat, "StoneStep%d", iFrame + 1);
		break;
	case MAT_TYPE::UP_DOWN_BLUE:
		sprintf_s(strMat, "StoneStep%d", iFrame + 1);
		break;
	case MAT_TYPE::LIFT_UP:
		sprintf_s(strMat, "StoneStep%d", iFrame + 1);
		break;
	case MAT_TYPE::LIFT_UP_BLUE:
		sprintf_s(strMat, "StoneStep%d", iFrame + 1);
		break;
	default:
		return;
	}

	m_pSnd->SetSoundAndPlay(strMat);
}

void CPlayer::MenuLeft(float fPushTime, float fTime)
{
	if (m_bMenu)
	{
		m_iMenu = (m_iMenu + 2) % 3;

		MenuUpdate();
	}
}

void CPlayer::MenuRight(float fPushTime, float fTime)
{
	if (m_bMenu)
	{
		m_iMenu = (m_iMenu + 4) % 3;

		MenuUpdate();
	}
}

void CPlayer::MenuUpdate()
{
	CObj* pObj = m_pScene->FindLayer("UI")->FindObj("Button");

	if (pObj)
	{
		CSpriteComponent* pCom = (CSpriteComponent*)pObj->FindSceneComponent("Return");
		CSpriteComponent* pCom2 = (CSpriteComponent*)pObj->FindSceneComponent("Option");
		CSpriteComponent* pCom3 = (CSpriteComponent*)pObj->FindSceneComponent("Exit");

		if (m_iMenu == 0)
		{
			pCom->ChangeSequence("On");
			pCom2->ChangeSequence("Off");
			pCom3->ChangeSequence("Off");
		}

		else if (m_iMenu == 1)
		{
			pCom->ChangeSequence("Off");
			pCom2->ChangeSequence("On");
			pCom3->ChangeSequence("Off");
		}

		else
		{
			pCom->ChangeSequence("Off");
			pCom2->ChangeSequence("Off");
			pCom3->ChangeSequence("On");
		}

		SAFE_RELEASE(pCom);
		SAFE_RELEASE(pCom2);
		SAFE_RELEASE(pCom3);

		pObj->Release();
	}
}

void CPlayer::SpawnWindow()
{
	if (ImGui::Begin("Player"))
	{	
		int iCol = m_cCol;
		ImGui::SliderInt("Col", &iCol, 0, 255);
		m_cCol = iCol;
		ImGui::SliderFloat("Speed", &m_fSpeed, 0.f, 200.f);

		Vector3 vPos = GetWorldPos();
		CTile* pTile = m_pDebugMap->GetTile(Vector2(vPos.x, vPos.y));

		Vector2 vIdx = pTile->GetTexIdx();

		ImGui::SliderFloat2("Tile Texture Index", &vIdx.x, 0.f, 4096.f);
		ImGui::SliderFloat("MoveScaleX", &m_fScaleX, -1.f, 1.f);
		ImGui::SliderFloat("MoveScaleY", &m_fScaleY, -1.f, 1.f);
		switch (m_eState)
		{
		case STATE::IDLE:
			ImGui::Text("Idle");
			break;
		case STATE::WALK:
			ImGui::Text("Walk");
			break;
		case STATE::ROLL:
			ImGui::Text("Roll");
			break;
		case STATE::SWIM:
			ImGui::Text("Swim");
			break;
		case STATE::ATTACK:
			ImGui::Text("Attack");
			break;
		case STATE::CALL:
			ImGui::Text("Call");
			break;
		case STATE::DIE:
			ImGui::Text("Die");
			break;
		case STATE::RUN:
			ImGui::Text("Run");
			break;
		}
	}
	ImGui::End();
}

void CPlayer::Mucus()
{
	m_fSlowTime = 0.f;

	if (!m_bSlow)
	{
		m_fSpeed /= 2.f;
		m_bSlow = true;

		CSpriteComponent* pCom = CreateComponent<CSpriteComponent>("Mucus", m_pScene->FindLayer("Fore"));

		pCom->SetWorldScale(16.f, 16.f, 0.f);
		pCom->SetPivot(0.5f, 0.5f, 0.f);
		pCom->CreateSprite("Idle", "ProMucus", LOOP_OPTION::LOOP);
		pCom->AddRenderState("DepthNoWrite");
		pCom->AddRenderState("NoCullBack");
		pCom->SetInheritScale(false);

		m_pMesh->AddChild(pCom);

		SAFE_RELEASE(pCom);
	}
}