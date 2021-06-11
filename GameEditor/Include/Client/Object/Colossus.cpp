#include "Colossus.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderRect.h"
#include "Scene/Scene.h"
#include "Camera/CameraManager.h"
#include "Component/Camera.h"
#include "../GameMode/TileMode.h"
#include "Scene/SceneManager.h"
#include "Component/Sound.h"
#include "Resource/Material.h"
#include "Resource/ShaderManager.h"
#include "Component/Particle.h"
#include "../../State.h"
#include "Component/Mesh2DComponent.h"
#include "Bullet.h"
#include "Player.h"
#include "../BossManager.h"
#include "InputObj.h"
#include "../GameMode/ColossusMode.h"
#include "Engine.h"

float CColossus::m_fLimitDist = 80.f;	//	손이 안따라가면서 내려가는 최대 거리
float CColossus::m_fLimitDist2 = 160.f;	//	손이 내려가는 최대 거리
float CColossus::m_fHandSpeed = 200.f;	//	내려갈때 초기 속도
float CColossus::m_fInertiaLimit = 0.5f;
float CColossus::m_fMoveSpeed = 150.f;	//	손이 움직이는 속도
float CColossus::m_fAccel = 3000.f;	//	손이 내려갈때 가속도
const float CColossus::m_fReturnSpeed = 400.f;
const float CColossus::m_fLiftSpeed = 150.f;	//	손이 올라가는 속도

CColossus::CColossus()	:
	m_pHead(nullptr),
	m_pBody(nullptr),
	m_pLeftHand(nullptr),
	m_pRightHand(nullptr),
	m_pLight(nullptr),
	m_pLeftHandShadow(nullptr),
	m_pRightHandShadow(nullptr),
	m_bRising(false),
	m_bLeft(false),
	m_eAttackStage(ATTACK_STAGE::IDLE),
	m_fReadyTime(0.f),
	m_fDownDist(m_fLimitDist2),
	m_fInertiaTime(0.f),
	m_pParticleLeft(nullptr),
	m_pParticleRight(nullptr),
	m_bSleep(true),
	m_pBGM(nullptr),
	m_pBGM2(nullptr),
	m_fSpeed(0.f),
	m_bFirst(true)
{
	SetBossType(BOSS_TYPE::COLOSSUS);
}

CColossus::CColossus(const CColossus& obj)	:
	CSoulMonster(obj)
	, m_pHead(static_cast<CSpriteComponent*>(FindSceneComponent("Head")))
	, m_pBody(static_cast<CSpriteComponent*>(FindSceneComponent("Body")))
	, m_pLeftHand(static_cast<CSpriteComponent*>(FindSceneComponent("LeftHand")))
	, m_pRightHand(static_cast<CSpriteComponent*>(FindSceneComponent("RightHand")))
	, m_pLight(static_cast<CSpriteComponent*>(FindSceneComponent("Light")))
	, m_pLeftHandShadow(static_cast<CSpriteComponent*>(FindSceneComponent("LeftHandShadow")))
	, m_pRightHandShadow(static_cast<CSpriteComponent*>(FindSceneComponent("RightHandShadow")))
	, m_bRising(false)
	, m_eAttackStage(ATTACK_STAGE::IDLE)
	, m_bLeft(false)
	, m_fReadyTime(0.f)
	, m_fDownDist(obj.m_fDownDist)
	, m_fInertiaTime(0.f)
	, m_pParticleLeft((CParticle*)FindSceneComponent("DustLeft"))
	, m_pParticleRight((CParticle*)FindSceneComponent("DustRight"))
	, m_bSleep(obj.m_bSleep)
	, m_pBGM((CSound*)FindSceneComponent("BGM"))
	, m_pBGM2((CSound*)FindSceneComponent("BGM2"))
	, m_fSpeed(obj.m_fSpeed)
	, m_bFirst(obj.m_bFirst)
{
	CColliderRect* pBodyRC = (CColliderRect*)FindSceneComponent("ColossusBody");
	pBodyRC->SetCallBack<CColossus>(COLLISION_STATE::INIT, this, &CColossus::ColStart);
	pBodyRC->SetCallBack<CColossus>(COLLISION_STATE::STAY, this, &CColossus::ColStay);
	pBodyRC->SetCallBack<CColossus>(COLLISION_STATE::END, this, &CColossus::ColLast);
	SAFE_RELEASE(pBodyRC);

	CColliderRect* pHeadRC = (CColliderRect*)FindSceneComponent("HeadBody");
	pHeadRC->SetCallBack<CColossus>(COLLISION_STATE::INIT, this, &CColossus::ColStart);
	pHeadRC->SetCallBack<CColossus>(COLLISION_STATE::STAY, this, &CColossus::ColStay);
	pHeadRC->SetCallBack<CColossus>(COLLISION_STATE::END, this, &CColossus::ColLast);
	SAFE_RELEASE(pHeadRC);

	m_pHead->AddCallBack("Cry", "Cry", this, &CColossus::Cry);
	m_pHead->AddCallBack("Cry", "CryEnd", this, &CColossus::ReturnCam);
	m_pHead->AddCallBack("Cry", "ShakeEnd", this, &CColossus::StopShaking);

	CColliderRect* pLHRC = m_pLeftHand->FindComByType<CColliderRect>();

	pLHRC->SetCallBack<CColossus>(COLLISION_STATE::INIT, this, &CColossus::ColStart);
	pLHRC->SetCallBack<CColossus>(COLLISION_STATE::STAY, this, &CColossus::ColStay);
	pLHRC->SetCallBack<CColossus>(COLLISION_STATE::END, this, &CColossus::ColLast);

	SAFE_RELEASE(pLHRC);

	CColliderRect* pRHRC = m_pRightHand->FindComByType<CColliderRect>();

	pRHRC->SetCallBack<CColossus>(COLLISION_STATE::INIT, this, &CColossus::ColStart);
	pRHRC->SetCallBack<CColossus>(COLLISION_STATE::STAY, this, &CColossus::ColStay);
	pRHRC->SetCallBack<CColossus>(COLLISION_STATE::END, this, &CColossus::ColLast);

	SAFE_RELEASE(pRHRC);
#ifdef _DEBUG
	m_pInput->SetActionFunc<CColossus>("Z", KEY_TYPE::KT_DOWN, this, &CColossus::Cheat);
#endif
}

CColossus::~CColossus()
{
	SAFE_RELEASE(m_pHead);
	SAFE_RELEASE(m_pBody);
	SAFE_RELEASE(m_pLeftHand);
	SAFE_RELEASE(m_pRightHand);
	SAFE_RELEASE(m_pLight);
	SAFE_RELEASE(m_pLeftHandShadow);
	SAFE_RELEASE(m_pRightHandShadow);
	SAFE_RELEASE(m_pParticleLeft);
	SAFE_RELEASE(m_pParticleRight);
	SAFE_RELEASE(m_pBGM);
	SAFE_RELEASE(m_pBGM2);
}

bool CColossus::Init()
{
	if(!CSoulMonster::Init())
		return false;

#ifdef _DEBUG
	m_pInput->SetActionFunc<CColossus>("Z", KEY_TYPE::KT_DOWN, this, &CColossus::Cheat);
#endif

	CLayer* pFore = m_pScene->FindLayer("Fore");

	m_pLight = CreateComponent<CSpriteComponent>("Light", m_pScene->FindLayer("Temp"));
	m_pLight->CreateSprite("Idle", "ColossusLight", LOOP_OPTION::LOOP);

	m_pLight->SetPivot(0.5f, 0.5f, 0.f);
	m_pLight->SetRelativePos(0.f, -56.f, 0.f);
	m_pLight->SetWorldScale(96.f, 48.f, 0.f);
	m_pLight->SetInheritScale(false);

	m_pLight->AddRenderState("AlphaBlend");
	m_pLight->AddRenderState("Silhouette");

	CLayer* pGround = m_pScene->FindLayer("Ground");

	m_pBody = CreateComponent<CSpriteComponent>("Body", m_pScene->FindLayer("Temp"));
	m_pBody->CreateSprite("Idle", "ColossusBodyIdle", LOOP_OPTION::LOOP);

	m_pBody->SetPivot(0.5f, 0.5f, 0.f);
	m_pBody->SetWorldScale(96.f, 128.f, 0.f);
	m_pBody->SetRelativePos(0.f, -64.f, 0.f);
	m_pBody->SetInheritScale(false);

	m_pBody->AddRenderState("Silhouette");

	CColliderRect* pBodyRC = CreateComponent<CColliderRect>("ColossusBody",m_pLayer);

	pBodyRC->SetExtent(28.f, 12.f);
	pBodyRC->SetRelativePos(0.f, 8.f, 0.f);
	pBodyRC->SetCallBack<CColossus>(COLLISION_STATE::INIT, this, &CColossus::ColStart);
	pBodyRC->SetCallBack<CColossus>(COLLISION_STATE::STAY, this, &CColossus::ColStay);
	pBodyRC->SetCallBack<CColossus>(COLLISION_STATE::END, this, &CColossus::ColLast);
	pBodyRC->Enable(false);

	m_pBody->AddChild(pBodyRC);

	SAFE_RELEASE(pBodyRC);

	m_pHead = CreateComponent<CSpriteComponent>("Head", m_pScene->FindLayer("Ground"));
	m_pHead->CreateSprite("Sleep", "ColossusHeadSleep", LOOP_OPTION::LOOP);
	m_pHead->CreateSprite("Idle", "ColossusHeadIdle", LOOP_OPTION::LOOP);
	m_pHead->CreateSprite("Cry", "ColossusHeadCry", LOOP_OPTION::ONCE_RETURN);
	m_pHead->CreateSprite("Stop", "ColossusHeadStop", LOOP_OPTION::LOOP);
	m_pHead->AddNotify("Cry", "Cry", 0.1f);
	m_pHead->AddNotify("Cry", "ShakeEnd", 1.9f);
	m_pHead->AddNotify("Cry", "CryEnd", 1.9f);

	m_pHead->AddCallBack("Cry", "Cry", this, &CColossus::Cry);
	m_pHead->AddCallBack("Cry", "CryEnd", this, &CColossus::ReturnCam);
	m_pHead->AddCallBack("Cry", "ShakeEnd", this, &CColossus::StopShaking);

	m_pHead->SetPivot(0.5f, 0.5f, 0.f);
	m_pHead->SetWorldScale(64.f, 64.f, 0.f);
	m_pHead->SetInheritScale(false);
	m_pHead->AddRenderState("Silhouette");
	m_pHead->AddWorldPos(0.f, -40.f, 0.f);

	m_pHead->AddChild(m_pBody);

	SetRootComponent(m_pHead);

	CColliderRect* pHeadRC = CreateComponent<CColliderRect>("HeadBody", m_pLayer);

	pHeadRC->SetExtent(48.f, 48.f);

	pHeadRC->SetCallBack<CColossus>(COLLISION_STATE::INIT, this, &CColossus::ColStart);
	pHeadRC->SetCallBack<CColossus>(COLLISION_STATE::STAY, this, &CColossus::ColStay);
	pHeadRC->SetCallBack<CColossus>(COLLISION_STATE::END, this, &CColossus::ColLast);

	m_pHead->AddChild(pHeadRC);

	SAFE_RELEASE(pHeadRC);

	CLayer* pZero = m_pScene->FindLayer("BackDefault");

	m_pLeftHand = CreateComponent<CSpriteComponent>("LeftHand", pZero);

	m_pLeftHand->CreateSprite("Down", "ColossusHandDown", LOOP_OPTION::LOOP);
	//m_pLeftHand->CreateSprite("Idle", "ColossusHandIdle", LOOP_OPTION::LOOP);
	//m_pLeftHand->CreateSprite("Up", "ColossusHandUp", LOOP_OPTION::LOOP);
	m_pLeftHand->CreateSprite("Defence", "ColossusHandDeffence", LOOP_OPTION::LOOP);
	m_pLeftHand->CreateSprite("Rock", "ColossusHandRock", LOOP_OPTION::ONCE_RETURN);

	m_pLeftHand->SetPivot(0.5f, 0.25f, 0.f);
	m_pLeftHand->SetRelativePos(400.f, 576.f, 0.f);
	m_pLeftHand->SetWorldScale(48.f, 64.f, 0.f);
	m_pLeftHand->SetInheritScale(false);
	m_pLeftHand->SetInheritPos(false);

	m_pLeftHand->AddRenderState("ForeGround");
	
	CColliderRect* pLHRC = CreateComponent<CColliderRect>("LeftHandBody", m_pLayer);

	pLHRC->SetExtent(40.f, 20.f);

	pLHRC->SetCallBack<CColossus>(COLLISION_STATE::INIT, this, &CColossus::ColStart);
	pLHRC->SetCallBack<CColossus>(COLLISION_STATE::STAY, this, &CColossus::ColStay);
	pLHRC->SetCallBack<CColossus>(COLLISION_STATE::END, this, &CColossus::ColLast);

	m_pLeftHand->AddChild(pLHRC);

	SAFE_RELEASE(pLHRC);

	m_pLeftHandShadow = CreateComponent<CSpriteComponent>("LeftHandShadow", pGround);

	m_pLeftHandShadow->SetWorldScale(48.f, 32.f, 0.f);
	m_pLeftHandShadow->SetPivot(0.5f, 0.5f, 0.f);
	m_pLeftHandShadow->SetRelativePos(0.f, 16.f, 0.f);

	m_pLeftHandShadow->CreateSprite("Down", "ColossusHandDownShadow", LOOP_OPTION::LOOP);
	//m_pLeftHandShadow->CreateSprite("Up", "ColossusHandUpShadow", LOOP_OPTION::LOOP);
	m_pLeftHandShadow->CreateSprite("Defence", "ColossusHandDeffenceShadow", LOOP_OPTION::LOOP);
	m_pLeftHandShadow->CreateSprite("Rock", "ColossusHandRockShadow", LOOP_OPTION::ONCE_RETURN);

	m_pLeftHandShadow->SetInheritScale(false);
	m_pLeftHandShadow->AddRenderState("AlphaBlend");
	m_pLeftHandShadow->AddRenderState("DepthNoWrite");

	CMaterial* pLHSMtrl = m_pLeftHandShadow->GetMaterial();

	pLHSMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.75f);

	SAFE_RELEASE(pLHSMtrl);

	m_pLeftHand->AddChild(m_pLeftHandShadow);

	m_pHead->AddChild(m_pLeftHand);

	m_pRightHand = CreateComponent<CSpriteComponent>("RightHand", pZero);

	m_pRightHand->CreateSprite("Down", "ColossusHandDown", LOOP_OPTION::LOOP);
	//m_pRightHand->CreateSprite("Idle", "ColossusHandIdle", LOOP_OPTION::LOOP);
	//m_pRightHand->CreateSprite("Up", "ColossusHandUp", LOOP_OPTION::LOOP);
	m_pRightHand->CreateSprite("Defence", "ColossusHandDeffence", LOOP_OPTION::LOOP);
	m_pRightHand->CreateSprite("Rock", "ColossusHandRock", LOOP_OPTION::ONCE_RETURN);

	m_pRightHand->SetPivot(0.5f, 0.25f, 0.f);
	m_pRightHand->SetRelativePos(560.f , 576.f, 0.f);
	m_pRightHand->SetWorldScale(48.f, 64.f, 0.f);
	m_pRightHand->SetInheritScale(false);
	m_pRightHand->SetWorldRotY(180.f);
	m_pRightHand->SetInheritPos(false);

	m_pRightHand->AddRenderState("ForeGround");
	m_pRightHand->AddRenderState("NoCullBack");

	m_pHead->AddChild(m_pRightHand);

	CColliderRect* pRHRC = CreateComponent<CColliderRect>("RightHandBody", m_pLayer);

	pRHRC->SetExtent(40.f, 20.f);

	pRHRC->SetCallBack<CColossus>(COLLISION_STATE::INIT, this, &CColossus::ColStart);
	pRHRC->SetCallBack<CColossus>(COLLISION_STATE::STAY, this, &CColossus::ColStay);
	pRHRC->SetCallBack<CColossus>(COLLISION_STATE::END, this, &CColossus::ColLast);

	m_pRightHand->AddChild(pRHRC);

	SAFE_RELEASE(pRHRC);

	m_pRightHandShadow = CreateComponent<CSpriteComponent>("RightHandShadow", pGround);

	m_pRightHandShadow->SetWorldScale(48.f, 32.f, 0.f);
	m_pRightHandShadow->SetPivot(0.5f, 0.5f, 0.f);
	m_pRightHandShadow->SetRelativePos(0.f, 16.f, 0.f);

	m_pRightHandShadow->CreateSprite("Down", "ColossusHandDownShadow", LOOP_OPTION::LOOP);
	//m_pRightHandShadow->CreateSprite("Up", "ColossusHandUpShadow", LOOP_OPTION::LOOP);
	m_pRightHandShadow->CreateSprite("Defence", "ColossusHandDeffenceShadow", LOOP_OPTION::LOOP);
	m_pRightHandShadow->CreateSprite("Rock", "ColossusHandRockShadow", LOOP_OPTION::ONCE_RETURN);

	m_pRightHandShadow->SetInheritScale(false);
	m_pRightHandShadow->AddRenderState("AlphaBlend");
	m_pRightHandShadow->AddRenderState("NoCullBack");
	m_pRightHandShadow->AddRenderState("DepthNoWrite");

	CMaterial* pRHSMtrl = m_pRightHandShadow->GetMaterial();

	pRHSMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.75f);

	SAFE_RELEASE(pRHSMtrl);

	m_pRightHand->AddChild(m_pRightHandShadow);

	CSpriteComponent* pLeftShoulder = CreateComponent<CSpriteComponent>("LeftShoulder", m_pScene->FindLayer("Temp"));
	pLeftShoulder->CreateSprite("Idle", "ColossusShoulderLeft", LOOP_OPTION::LOOP);

	pLeftShoulder->SetPivot(0.5f, 0.5f, 0.f);
	pLeftShoulder->SetRelativePos(-40.f, -32.f, 0.f);
	pLeftShoulder->SetWorldScale(48.f, 48.f, 0.f);
	pLeftShoulder->SetInheritScale(false);
	pLeftShoulder->AddRenderState("Silhouette");

	m_pHead->AddChild(pLeftShoulder);

	SAFE_RELEASE(pLeftShoulder);

	CSpriteComponent* pRightShoulder = CreateComponent<CSpriteComponent>("RightShoulder", m_pScene->FindLayer("Temp"));
	pRightShoulder->CreateSprite("Idle", "ColossusShoulderRight", LOOP_OPTION::LOOP);

	pRightShoulder->SetPivot(0.5f, 0.5f, 0.f);
	pRightShoulder->SetRelativePos(40.f, -32.f, 0.f);
	pRightShoulder->SetWorldScale(48.f, 48.f, 0.f);
	pRightShoulder->SetInheritScale(false);
	pRightShoulder->AddRenderState("Silhouette");

	m_pHead->AddChild(pRightShoulder);

	SAFE_RELEASE(pRightShoulder);

	m_pHead->AddChild(m_pLight);

	m_pParticleLeft = CreateComponent<CParticle>("DustLeft", m_pScene->FindLayer("Ground"));

	m_pParticleLeft->AddParticle("DustCol");
	m_pParticleLeft->AddParticle("DustColShare");
	m_pParticleLeft->SetUVStart(64.f, 96.f);
	m_pParticleLeft->SetUVEnd(96.f, 128.f);
	m_pParticleLeft->SetUVSize(256.f, 256.f);
	m_pParticleLeft->AddRenderState("DepthNoWrite");
	m_pParticleLeft->SetSpawnLimit(0.f);
	m_pParticleLeft->SetInheritScale(false);
	m_pParticleLeft->SetWorldScale(1.f, 1.f, 1.f);
	m_pParticleLeft->SetSpawnCount(0);

	CMaterial* pMtrl = m_pParticleLeft->GetMaterial();

	pMtrl->SetTexture(REGISTER_TYPE::RT_DIF, "Effect", 0, 1,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	SAFE_RELEASE(pMtrl);

	m_pLeftHand->AddChild(m_pParticleLeft);

	CParticle* pTileDustLeft = CreateComponent<CParticle>("TileDustLeft", m_pScene->FindLayer("Ground"));

	pTileDustLeft->AddParticle("TileDustCol");
	pTileDustLeft->AddParticle("TileDustColShare");
	pTileDustLeft->SetSpawnLimit(0.f);
	pTileDustLeft->SetSpawnCount(0);
	pTileDustLeft->SetUVStart(176.f, 0.f);
	pTileDustLeft->SetUVEnd(192.f, 16.f);
	pTileDustLeft->SetUVSize(256.f, 256.f);
	pTileDustLeft->SetInheritScale(false);
	pTileDustLeft->SetWorldScale(1.f, 1.f, 1.f);
	pTileDustLeft->AddRenderState("DepthNoWrite");

	pMtrl = pTileDustLeft->GetMaterial();

	pMtrl->SetTexture(REGISTER_TYPE::RT_DIF, "Effect", 0, 1,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	SAFE_RELEASE(pMtrl);

	m_pLeftHand->AddChild(pTileDustLeft);
	SAFE_RELEASE(pTileDustLeft);

	m_pParticleRight = CreateComponent<CParticle>("DustRight", m_pScene->FindLayer("Ground"));

	m_pParticleRight->AddParticle("DustCol");
	m_pParticleRight->AddParticle("DustColShare");
	m_pParticleRight->SetUVStart(64.f, 96.f);
	m_pParticleRight->SetUVEnd(96.f, 128.f);
	m_pParticleRight->SetUVSize(256.f, 256.f);
	m_pParticleRight->SetSpawnLimit(0.f);
	m_pParticleRight->SetSpawnCount(0);
	m_pParticleRight->SetInheritScale(false);
	m_pParticleRight->SetWorldScale(1.f, 1.f, 1.f);
	m_pParticleRight->AddRenderState("DepthNoWrite");
	m_pParticleRight->AddRenderState("NoCullBack");

	pMtrl = m_pParticleRight->GetMaterial();

	pMtrl->SetTexture(REGISTER_TYPE::RT_DIF, "Effect", 0, 1,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	SAFE_RELEASE(pMtrl);

	m_pRightHand->AddChild(m_pParticleRight);

	CParticle* pTileDustRight = CreateComponent<CParticle>("TileDustRight", m_pScene->FindLayer("Ground"));

	pTileDustRight->AddParticle("TileDustCol");
	pTileDustRight->AddParticle("TileDustColShare");
	pTileDustRight->SetSpawnLimit(0.f);
	pTileDustRight->SetSpawnCount(0);
	pTileDustRight->SetUVStart(176.f, 0.f);
	pTileDustRight->SetUVEnd(192.f, 16.f);
	pTileDustRight->SetUVSize(256.f, 256.f);
	pTileDustRight->SetInheritScale(false);
	pTileDustRight->SetWorldScale(1.f, 1.f, 1.f);
	pTileDustRight->AddRenderState("DepthNoWrite");
	pTileDustRight->AddRenderState("NoCullBack");

	pMtrl = pTileDustRight->GetMaterial();

	pMtrl->SetTexture(REGISTER_TYPE::RT_DIF, "Effect", 0, 1,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	SAFE_RELEASE(pMtrl);

	m_pRightHand->AddChild(pTileDustRight);
	SAFE_RELEASE(pTileDustRight);

	CMesh2DComponent* pStencil = CreateComponent<CMesh2DComponent>("Stencil", m_pScene->FindLayer("Back"));

	pStencil->SetWorldScale(112.f, 128.f, 0.f);
	pStencil->SetPivot(0.5f, 0.5f, 0.f);
	pStencil->AddRenderState("ForeGround");
	pStencil->SetInheritScale(false);
	pStencil->SetWorldPos(480.f, 680.f, 0.f);
	pStencil->SetInheritPos(false);

	m_pBody->AddChild(pStencil);
	
	CMaterial* pStencilMtrl = pStencil->GetMaterial();

	pStencilMtrl->SetDiffuseColor(1.f, 1.f, 0.f, 0.5f);

	SAFE_RELEASE(pStencilMtrl);

	SAFE_RELEASE(pStencil);

	m_pBGM = CreateComponent<CSound>("BGM", m_pLayer);

	m_pBGM->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

	m_pHead->AddChild(m_pBGM);

	m_pBGM2 = CreateComponent<CSound>("BGM2", m_pLayer);

	m_pBGM2->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

	m_pHead->AddChild(m_pBGM2);

	CParticle* pDownDust = CreateComponent<CParticle>("ColDown", m_pScene->FindLayer("Ground"));

	pDownDust->AddParticle("ColDown");
	pDownDust->AddParticle("ColDownShare");
	pDownDust->SetSpawnLimit(0.01f);
	pDownDust->SetUVStart(192.f, 0.f);
	pDownDust->SetUVEnd(256.f, 64.f);
	pDownDust->SetUVSize(256.f, 256.f);
	pDownDust->SetInheritScale(false);
	pDownDust->SetInheritPos(false);
	pDownDust->SetWorldScale(1.f, 1.f, 1.f);
	pDownDust->AddRenderState("DepthNoWrite");
	pDownDust->AddRenderState("NoCullBack");
	pDownDust->SetWorldPos(480.f, 624.f, 0.f);
	pDownDust->Stop();

	pMtrl = pDownDust->GetMaterial();

	pMtrl->SetTexture(REGISTER_TYPE::RT_DIF, "Effect", 0, 1,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	SAFE_RELEASE(pMtrl);

	m_pHead->AddChild(pDownDust);

	SAFE_RELEASE(pDownDust);

	return true;
}

void CColossus::Start()
{
	CSoulMonster::Start();
}

void CColossus::Update(float fTime)
{
	CSoulMonster::Update(fTime);

	if (m_bRising)
	{
		Vector3 vPos = m_pHead->GetWorldPos();

		static float fDelayTime = 0.f;

		if (vPos.y >= 718.f || fDelayTime > 0.f)
		{
			fDelayTime += fTime;

			if (fDelayTime >= 0.5f)
			{
				fDelayTime = 0.f;
				m_bRising = false;

				m_pHead->SetWorldPos(vPos.x, 718.f, vPos.z);
				m_pHead->ChangeSequence("Cry");
				m_pHead->SetDefaultSeq("Idle");

				m_bSleep = false;
			}

			else if (fDelayTime < 0.25f)
			{
				m_pHead->AddRelativePos(0.f, -32.f * fTime, 0.f);
			}
		}

		else
		{
			fDelayTime = 0.f;

			m_pHead->AddRelativePos(0.f, 128.f * fTime, 0.f);

			Vector3 vBodyDir = m_pBody->GetWorldPos() + Vector3(0.f, -32.f, 0.f) - m_pLeftHand->GetWorldPos();

			vBodyDir.z = 0.f;

			float fBodyDist = vBodyDir.Length();

			vBodyDir /= fBodyDist;

			if (fBodyDist > 5.f)
			{
				m_pLeftHand->AddWorldPos(vBodyDir * m_fReturnSpeed * fTime);
			}

			else
			{
				m_pLeftHand->AddWorldPos(vBodyDir * 5.f * fTime);
			}

			m_pLeftHand->ChangeSequence("Defence");
		}
	}

	else if(!m_bSleep)
	{
		Vector3 vPos = m_pHead->GetWorldPos();

		static bool bCheck = false;

		if (vPos.y >= 702.f)
		{
			m_pHead->AddWorldPos(0.f, -128.f * fTime, 0.f);
			bCheck = true;
		}

		else if(bCheck)
		{
			bCheck = false;
			m_pBGM->SetSoundAndPlay("RiseBodyLand");

			((CTileMode*)m_pScene->GetGameMode())->ShakeCam();

			CColliderRect* pRC = m_pBody->FindComByType<CColliderRect>();

			pRC->Enable(true);

			SAFE_RELEASE(pRC);
		}

		State eStat = GetState();
		switch (eStat)
		{
		case State::ATTACK:
		{
			switch (m_eAttackStage)
			{
			case ATTACK_STAGE::IDLE:
			{
				m_fReadyTime += fTime;

				if (m_fReadyTime >= 0.5f)
				{
					((CTileMode*)m_pScene->GetGameMode())->SetText(false);

					m_pLeftHand->SetWorldScale(48.f, 64.f, 0.f);
					m_pRightHand->SetWorldScale(48.f, 64.f, 0.f);

					CObj* pObj = m_pScene->GetGameMode()->GetPlayer();

					Vector3 vPos = pObj->GetWorldPos();

					if (vPos.x > GetWorldPos().x 
						|| m_bFirst)
					{
						if (m_bLeft )
						{
							m_pRightHand->ChangeSequence("Rock");

							m_pRightHandShadow->ChangeSequence("Rock");

							m_bLeft = false;
						}

						if (m_bFirst)
						{
							m_bFirst = false;
						}
					}

					else
					{
						if (!m_bLeft)
						{
							m_bLeft = true;

							m_pLeftHand->ChangeSequence("Rock");

							m_pLeftHandShadow->ChangeSequence("Rock");
						}
					}

					SAFE_RELEASE(pObj);

					m_eAttackStage = ATTACK_STAGE::FIND;
					m_fReadyTime = 0.f;
				}
			}
			break;
			case ATTACK_STAGE::FIND:
			{
				m_fReadyTime += fTime;

				CObj* pObj = m_pScene->GetGameMode()->GetPlayer();

				Vector3 vObjPos = pObj->GetWorldPos() + Vector3(0.f, m_fLimitDist2, 0.f);

				Vector3 vPos = {};

				if (m_bLeft)
				{
					m_pRightHand->ChangeSequence("Defence");
					m_pLeftHand->AddWorldPos(0.f, m_fLiftSpeed * fTime, 0.f);

					m_fDownDist -= fTime * m_fLiftSpeed;

					if (m_fDownDist <= m_fLimitDist2 - m_fLimitDist)
					{
						m_fDownDist = m_fLimitDist2 - m_fLimitDist;
						m_eAttackStage = ATTACK_STAGE::READY;

						m_vTargetPos = pObj->GetWorldPos();
					}

					else
					{
						m_pLeftHandShadow->AddRelativePos(0.f, -m_fLiftSpeed * fTime, 0.f);

						m_pLeftHandShadow->AddRelativeScale(-60.f * fTime, -40.f * fTime, 0.f);

						if (m_fDownDist >= 16.f)
						{
							CColliderRect* pRC = m_pLeftHand->FindComByType<CColliderRect>();

							pRC->Enable(false);

							SAFE_RELEASE(pRC);
						}
					}
				}

				else
				{
					m_pLeftHand->ChangeSequence("Defence");
					m_pRightHand->AddWorldPos(0.f, m_fLiftSpeed* fTime, 0.f);

					m_fDownDist -= fTime * m_fLiftSpeed;

					if (m_fDownDist <= m_fLimitDist2 - m_fLimitDist)
					{
						m_fDownDist = m_fLimitDist2 - m_fLimitDist;
						m_eAttackStage = ATTACK_STAGE::READY;

						m_vTargetPos = pObj->GetWorldPos();
					}

					else
					{
						m_pRightHandShadow->AddRelativePos(0.f, -m_fLiftSpeed * fTime, 0.f);

						m_pRightHandShadow->AddRelativeScale(-60.f * fTime, -40.f * fTime, 0.f);

						if (m_fDownDist >= 16.f)
						{
							CColliderRect* pRC = m_pRightHand->FindComByType<CColliderRect>();

							pRC->Enable(false);

							SAFE_RELEASE(pRC);
						}
					}
				}

				SAFE_RELEASE(pObj);
			}
			break;
			case ATTACK_STAGE::READY:
			{
				Vector3 vPos = {};

				if (m_bLeft)
				{
					vPos = m_pLeftHand->GetWorldPos();

					Vector3 vDir = m_vTargetPos - (vPos - Vector3(0.f, m_fLimitDist2 - m_fDownDist, 0.f));

					vDir.z = 0.f;

					float fLength = vDir.Length();

					vDir /= fLength;

					if (fLength > 10.f)
					{
						m_pLeftHand->AddWorldPos(vDir * 600.f * fTime);
					}

					else
					{
						m_pLeftHand->AddWorldPos(vDir * 5.f * fTime);
					}

					m_pLeftHand->AddWorldPos(0.f, m_fLiftSpeed* fTime, 0.f);

					m_fDownDist -= fTime * m_fLiftSpeed;

					if (m_fDownDist <= 0.f)
					{
						m_fDownDist = 0.f;

						m_eAttackStage = ATTACK_STAGE::DOWN;

						m_fSpeed = m_fHandSpeed;

						CColliderRect* pRC = m_pLeftHand->FindComByType<CColliderRect>();

						pRC->Enable(false);

						SAFE_RELEASE(pRC);
					}

					else
					{
						m_pLeftHandShadow->AddRelativePos(0.f, -m_fLiftSpeed * fTime, 0.f);

						m_pLeftHandShadow->AddRelativeScale(-60.f * fTime, -40.f * fTime, 0.f);
					}

					Vector3 vBodyDir = m_pBody->GetWorldPos() - m_pRightHand->GetWorldPos();

					float fBodyDist = vBodyDir.Length();

					vBodyDir /= fBodyDist;

					if (fBodyDist > 5.f)
					{
						m_pRightHand->AddWorldPos(vBodyDir * m_fMoveSpeed * fTime);
					}

					else
					{
						m_pRightHand->AddWorldPos(vBodyDir * 5.f * fTime);
					}
				}

				else
				{
					vPos = m_pRightHand->GetWorldPos();

					Vector3 vDir = m_vTargetPos - (vPos - Vector3(0.f, m_fLimitDist2 - m_fDownDist, 0.f));

					vDir.z = 0.f;

					float fLength = vDir.Length();

					vDir /= fLength;

					if (fLength > 10.f)
					{
						m_pRightHand->AddWorldPos(vDir * 600.f * fTime);
					}

					else
					{
						m_pRightHand->AddWorldPos(vDir * 5.f * fTime);
					}

					m_pRightHand->AddWorldPos(0.f, m_fLiftSpeed* fTime, 0.f);

					m_fDownDist -= fTime * m_fLiftSpeed;

					if (m_fDownDist <= 0.f)
					{
						m_fDownDist = 0.f;

						m_eAttackStage = ATTACK_STAGE::DOWN;

						m_fSpeed = m_fHandSpeed;

						CColliderRect* pRC = m_pRightHand->FindComByType<CColliderRect>();

						pRC->Enable(false);

						SAFE_RELEASE(pRC);
					}

					else
					{
						m_pRightHandShadow->AddRelativePos(0.f, -m_fLiftSpeed * fTime, 0.f);

						m_pRightHandShadow->AddRelativeScale(-60.f * fTime, -40.f * fTime, 0.f);
					}

					Vector3 vBodyDir = m_pBody->GetWorldPos() - m_pLeftHand->GetWorldPos();

					vBodyDir.z = 0.f;

					float fBodyDist = vBodyDir.Length();

					vBodyDir /= fBodyDist;

					if (fBodyDist > 5.f)
					{
						m_pLeftHand->AddWorldPos(vBodyDir * m_fMoveSpeed * fTime);
					}

					else
					{
						m_pLeftHand->AddWorldPos(vBodyDir * 5.f * fTime);
					}
				}
			}
			break;
			case ATTACK_STAGE::DOWN:
			{
				m_fSpeed += m_fAccel * fTime;
				m_fDownDist += fTime * m_fSpeed;

				if (m_fDownDist >= m_fLimitDist2)
				{
					m_eAttackStage = ATTACK_STAGE::IDLE;

					static int iCount = 0;

					char strNum[32] = {};

					sprintf_s(strNum, "Hit%d", iCount + 1);

					m_pBGM->SetSoundAndPlay(strNum);

					((CTileMode*)m_pScene->GetGameMode())->ShakeCam(600.f, 20.f, 0.25f);

					if (m_bLeft)
					{
						m_pLeftHandShadow->SetRelativePos(0.f, 0.f, 0.f);
						m_pLeftHandShadow->SetRelativeScale(48.f, 32.f, 0.f);
						m_pParticleLeft->SetSpawnCount(16);

						CParticle* pLeft = (CParticle*)m_pLeftHand->FindComponent("TileDustLeft");

						pLeft->SetSpawnCount(16);

						SAFE_RELEASE(pLeft);

						CSceneComponent* pCol = FindSceneComponent("ColossusBody");

						m_pRightHand->SetWorldPos(pCol->GetWorldPos() + Vector3( 0.f, -16.f, 0.f));

						SAFE_RELEASE(pCol);
					}

					else
					{
						m_pRightHandShadow->SetRelativePos(0.f, 0.f, 0.f);
						m_pRightHandShadow->SetRelativeScale(48.f, 32.f, 0.f);
						m_pParticleRight->SetSpawnCount(16);

						CParticle* pRight = (CParticle*)m_pRightHand->FindComponent("TileDustRight");

						pRight->SetSpawnCount(16);

						SAFE_RELEASE(pRight);

						CSceneComponent* pCol = FindSceneComponent("ColossusBody");

						m_pLeftHand->SetWorldPos(pCol->GetWorldPos() + Vector3(0.f, -16.f, 0.f));

						SAFE_RELEASE(pCol);
					}

					++iCount %= 4;
				}

				else if (m_fLimitDist2 - m_fDownDist <= 16.f)
				{
					if (m_bLeft)
					{
						CColliderRect* pRC = m_pLeftHand->FindComByType<CColliderRect>();

						pRC->Enable(true);

						SAFE_RELEASE(pRC);
					}

					else
					{
						CColliderRect* pRC = m_pRightHand->FindComByType<CColliderRect>();

						pRC->Enable(true);

						SAFE_RELEASE(pRC);
					}
				}

				else if (m_fDownDist <m_fLimitDist)
				{
					CObj* pObj = m_pScene->GetGameMode()->GetPlayer();

					m_vTargetPos = pObj->GetWorldPos();

					SAFE_RELEASE(pObj);

					if (m_bLeft)
					{
						vPos = m_pLeftHand->GetWorldPos();

						Vector3 vDir = m_vTargetPos - (vPos - Vector3(0.f, m_fLimitDist2 - m_fDownDist, 0.f));

						vDir.z = 0.f;

						float fLength = vDir.Length();

						vDir /= fLength;

						if (fLength > 10.f)
						{
							m_pLeftHand->AddWorldPos(vDir * 200.f * fTime);
						}

						else
						{
							m_pLeftHand->AddWorldPos(vDir * 5.f * fTime);
						}
					}

					else
					{
						vPos = m_pRightHand->GetWorldPos();

						Vector3 vDir = m_vTargetPos - (vPos - Vector3(0.f, m_fLimitDist2 - m_fDownDist, 0.f));

						vDir.z = 0.f;

						float fLength = vDir.Length();

						vDir /= fLength;

						if (fLength > 10.f)
						{
							m_pRightHand->AddWorldPos(vDir * 200.f * fTime);
						}

						else
						{
							m_pRightHand->AddWorldPos(vDir * 5.f * fTime);
						}
					}
				}

				if (m_bLeft)
				{
					m_pLeftHand->AddWorldPos(0.f, -m_fSpeed * fTime, 0.f);

					m_pLeftHandShadow->AddRelativePos(0.f, m_fSpeed * fTime, 0.f);

					m_pLeftHandShadow->AddRelativeScale(60.f * fTime, 40.f * fTime, 0.f);

					Vector3 vBodyDir = m_pBody->GetWorldPos() - m_pRightHand->GetWorldPos();

					float fBodyDist = vBodyDir.Length();

					vBodyDir /= fBodyDist;

					if (fBodyDist > 5.f)
					{
						m_pRightHand->AddWorldPos(vBodyDir * m_fReturnSpeed* fTime);
					}

					else
					{
						m_pRightHand->AddWorldPos(vBodyDir * 5.f * fTime);
					}
				}

				else
				{
					m_pRightHand->AddWorldPos(0.f, -m_fSpeed * fTime, 0.f);

					m_pRightHandShadow->AddRelativePos(0.f, m_fSpeed * fTime, 0.f);

					m_pRightHandShadow->AddRelativeScale(60.f * fTime, 40.f * fTime, 0.f);

					Vector3 vBodyDir = m_pBody->GetWorldPos() - m_pLeftHand->GetWorldPos();

					float fBodyDist = vBodyDir.Length();

					vBodyDir /= fBodyDist;

					if (fBodyDist > 5.f)
					{
						m_pLeftHand->AddWorldPos(vBodyDir * m_fReturnSpeed* fTime);
					}

					else
					{
						m_pLeftHand->AddWorldPos(vBodyDir * 5.f * fTime);
					}
				}
			}
			break;
			}
		}
			break;
		case State::STOP:
			break;
		case State::DIE:
			Vector3 vPos = m_pHead->GetWorldPos();

			if (vPos.y > 718.f - 80.f)
			{
				m_pHead->AddWorldPos(0.f, -16.f * fTime, 0.f);
				((CTileMode*)m_pScene->GetGameMode())->ShakeCam();
			}

			else
			{
				CParticle* pParticle = (CParticle*)FindSceneComponent("ColDown");

				pParticle->Stop();

				SAFE_RELEASE(pParticle);

				m_pHead->ChangeSequence("Sleep");
				m_pHead->SetDefaultSeq("Sleep");
			}

			if (m_fDownDist < m_fLimitDist2)
			{
				m_fDownDist += fTime * 16.f;

				if (m_bLeft)
				{
					m_pLeftHand->AddWorldPos(0.f, fTime * -16.f, 0.f);
					m_pLeftHandShadow->AddRelativePos(0.f, 16.f * fTime, 0.f);
					m_pLeftHandShadow->AddRelativeScale(6.f * fTime, 4.f * fTime, 0.f);
				}

				else
				{
					m_pRightHand->AddWorldPos(0.f, fTime * -16.f, 0.f);
					m_pRightHandShadow->AddRelativePos(0.f, 16.f * fTime, 0.f);
					m_pRightHandShadow->AddRelativeScale(6.f * fTime, 4.f * fTime, 0.f);
				}
			}

			break;
		}
		
	}
}

void CColossus::PostUpdate(float fTime)
{
	CSoulMonster::PostUpdate(fTime);
}

void CColossus::Collision(float fTime)
{
	CSoulMonster::Collision(fTime);
}

void CColossus::PreRender(float fTime)
{
	CSoulMonster::PreRender(fTime);

	if (GET_SINGLE(CEngine)->IsImgui())
	{
		SpawnWindow();
	}
}

void CColossus::Render(float fTime)
{
	CSoulMonster::Render(fTime);
}

void CColossus::PostRender(float fTime)
{
	CSoulMonster::PostRender(fTime);
}

CColossus* CColossus::Clone()
{
	return new CColossus(*this);
}

void CColossus::Save(FILE* pFile)
{
	CSoulMonster::Save(pFile);
}

void CColossus::Load(FILE* pFile)
{
	CSoulMonster::Load(pFile);

	m_pHead = static_cast<CSpriteComponent*>(FindSceneComponent("Head"));
	m_pBody = static_cast<CSpriteComponent*>(FindSceneComponent("Body"));
	m_pLeftHand = static_cast<CSpriteComponent*>(FindSceneComponent("LeftHand"));
	m_pRightHand = static_cast<CSpriteComponent*>(FindSceneComponent("RightHand"));
	m_pLight = static_cast<CSpriteComponent*>(FindSceneComponent("Light"));
	m_pLeftHandShadow = static_cast<CSpriteComponent*>(FindSceneComponent("LeftHandShadow"));
	m_pRightHandShadow = static_cast<CSpriteComponent*>(FindSceneComponent("RightHandShadow"));
	m_pParticleLeft = (CParticle*)FindSceneComponent("DustLeft");
	m_pParticleRight = (CParticle*)FindSceneComponent("DustRight");
	m_pBGM = (CSound*)FindSceneComponent("BGM");
	m_pBGM2 = (CSound*)FindSceneComponent("BGM2");
	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();
	m_pLeftHandShadow->SetLayer(pScene->FindLayer("Ground"));
	m_pRightHandShadow->SetLayer(pScene->FindLayer("Ground"));
	m_pLeftHand->SetLayer(pScene->FindLayer("BackDefault"));
	m_pRightHand->SetLayer(pScene->FindLayer("BackDefault"));
	CMesh2DComponent* pStencil = (CMesh2DComponent*)FindSceneComponent("Stencil");

	pStencil->SetLayer(pScene->FindLayer("Back"));

	SAFE_RELEASE(pStencil);
	m_pBody->SetLayer(pScene->FindLayer("Temp"));
	m_pHead->SetLayer(pScene->FindLayer("Ground"));
}

void CColossus::ColStart(CCollider* pSrc, CCollider* pDst, float fTime)
{
	std::string strSrc = pSrc->GetName();
	std::string strDst = pDst->GetName();

	if (strDst == "BulletBody")
	{
		State eStat = GetState();

		if (m_bSleep && !m_bRising)
		{
			if (((CBullet*)pDst->GetObj())->GetSpeed())
			{
				m_bRising = true;

				CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

				pCam->SetFocus(this);

				SAFE_RELEASE(pCam);

				CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

				((CTileMode*)m_pScene->GetGameMode())->Lock(true);

				CSpriteComponent* pRightShoulder = (CSpriteComponent*)FindSceneComponent("RightShoulder");
				CSpriteComponent* pLeftShoulder = (CSpriteComponent*)FindSceneComponent("LeftShoulder");

				pRightShoulder->SetRelativePos(40.f, -24.f, 0.f);
				pLeftShoulder->SetRelativePos(-40.f, -24.f, 0.f);

				SAFE_RELEASE(pRightShoulder);
				SAFE_RELEASE(pLeftShoulder);

				m_pBGM->SetSoundAndPlay("Rise");
				m_pBGM2->SetSoundAndPlay("Colossus");

				((CTileMode*)m_pScene->GetGameMode())->SetText(true);
			}
		}

		else if (eStat == State::ATTACK)
		{
			if (strSrc == "ColossusBody")
			{
				SetState(State::STOP);

				m_pBGM2->SetSound("Colossus");
				m_pBGM2->Stop();

				CBullet* pBullet = (CBullet*)pDst->GetObj();

				pBullet->SetFix(true);
				pBullet->SetSpeed(0.f);
				pBullet->ChangeSprite("ArrowStop");
				pBullet->SetFixObj(this);

				Vector3 vRot = pBullet->GetWorldRot();

				CSound* pSnd = pBullet->FindComByType<CSound>();

				if (vRot.z < 135.f && vRot.x > 45.f)
				{
					pSnd->SetSound("ArrowImpact1");
				}

				else if (vRot.z > -45.f && vRot.x < 45.f)
				{
					pSnd->SetSound("ArrowImpact2");
				}

				else if (vRot.z > -135.f && vRot.x < -45.f)
				{
					pSnd->SetSound("ArrowImpact3");
				}

				else
				{
					pSnd->SetSound("ArrowImpact4");
				}

				SAFE_RELEASE(pSnd);
			}
		}

	}

	else if (strDst == "PlayerBody")
	{
		if (m_eAttackStage == ATTACK_STAGE::DOWN)
		{
			m_pBGM2->SetSound("Colossus");
			m_pBGM2->Stop();
		}
	}
}

void CColossus::ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
{
}

void CColossus::ColLast(CCollider* pSrc, CCollider* pDst, float fTime)
{
}

void CColossus::SetState(State eState)
{
	State ePrevState = GetState();

	CSoulMonster::SetState(eState);

	switch (eState)
	{
	case State::IDLE:
		break;
	case State::ATTACK:
		break;
	case State::STOP:
	{
		m_pHead->ChangeSequence("Stop");

		/*FADE tFade = {};

		tFade.fGray = 1.f;

		GET_SINGLE(CShaderManager)->UpdateCBuffer("Fade", &tFade);*/
	}
		break;
	case State::DIE:

		CParticle* pParticle = (CParticle*)FindSceneComponent("ColDown");

		pParticle->Resume();

		SAFE_RELEASE(pParticle);

		m_pLight->Enable(false);

		((CColossusMode*)m_pScene->GetGameMode())->Clear();

		CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

		pCam->SetFocus(nullptr);

		SAFE_RELEASE(pCam);

		GET_SINGLE(CBossManager)->AddMonster(this);

		break;
	}
}

void CColossus::Cry(float fTime)
{
	CTileMode* pMode = (CTileMode*)m_pScene->GetGameMode();

	pMode->ShakeCam(1500.f, 10.f, 2.5f);

	CSound* pSnd = CreateComponent<CSound>("Cry");

	pSnd->SetSound("Roar");

	pSnd->Play(fTime);

	SAFE_RELEASE(pSnd);

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	pCam->SetTarget(this);
	pCam->SetZoom(true);

	SAFE_RELEASE(pCam);

	pCam = GET_SINGLE(CCameraManager)->GetUICam();

	pCam->SetZoomCenter(0.5f, 0.2f);
	pCam->SetZoom(true, 0.2f);

	SAFE_RELEASE(pCam);
}

void CColossus::ReturnCam(float fTime)
{
	CObj* pObj = m_pScene->GetGameMode()->GetPlayer();

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	pCam->SetMin(0.f);
	pCam->SetMax(700.f);
	pCam->SetTarget(pObj);
	pCam->SetZoom(false);

	SAFE_RELEASE(pCam);

	SAFE_RELEASE(pObj);

	//SetState(State::IDLE);

	pCam = GET_SINGLE(CCameraManager)->GetUICam();

	pCam->SetZoom(false);

	SAFE_RELEASE(pCam);

	SetState(State::ATTACK);

	m_fReadyTime = 0.f;
}

void CColossus::StopShaking(float fTime)
{
	CTileMode* pMode = (CTileMode*)m_pScene->GetGameMode();

	pMode->StopShaking();
}

void CColossus::Cheat(float, float)
{
	SetState(State::DIE);
}

void CColossus::SpawnWindow()
{	
	if (ImGui::Begin("Colossus"))
	{
		ImGui::SliderFloat("DownDist", &m_fDownDist, 0.f, m_fLimitDist2);
		Vector3 vLeftPos = m_pLeftHand->GetWorldPos();
		ImGui::SliderFloat3("LeftHandPos", &vLeftPos.x, -500.f, 500.f);
		m_pLeftHand->SetWorldPos(vLeftPos);
		Vector3 vRightPos = m_pRightHand->GetWorldPos();
		ImGui::SliderFloat3("RightHandPos", &vRightPos.x, -500.f, 500.f);
		m_pRightHand->SetWorldPos(vRightPos);

		switch (m_eAttackStage)
		{
		case ATTACK_STAGE::IDLE:
			ImGui::Text("IDLE");
			break;
		case ATTACK_STAGE::FIND:
			ImGui::Text("FIND");
			break;
		case ATTACK_STAGE::READY:
			ImGui::Text("READY");
			break;
		case ATTACK_STAGE::DOWN:
			ImGui::Text("DOWN");
			break;
		}

		ImGui::SliderInt("Stage", (int*)&m_eAttackStage, (int)ATTACK_STAGE::IDLE, (int)ATTACK_STAGE::DOWN);
		ImGui::Checkbox("Left", &m_bLeft);
		ImGui::SliderFloat3("TargetPos", &m_vTargetPos.x, 0.f, 500.f);
		ImGui::Checkbox("first", &m_bFirst);
		ImGui::Checkbox("Sleep", &m_bSleep);
		ImGui::Checkbox("Rising", &m_bRising);
	}
	ImGui::End();
}
