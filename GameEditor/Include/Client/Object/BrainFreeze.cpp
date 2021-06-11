#include "pch.h"
#include "BrainFreeze.h"
#include "Component/SpriteComponent.h"
#include "InputObj.h"
#include "Component/ColliderOBB2D.h"
#include "Scene/Scene.h"
#include "Component/Mesh2DComponent.h"
#include "Resource/Material.h"
#include "Component/ColliderRect.h"
#include "Component/Transform.h"
#include "UI/UIFont.h"
#include "../GameMode/TileMode.h"
#include "Bullet.h"
#include "Resource/ShaderManager.h"
#include "Component/Particle.h"
#include "../../State.h"
#include "../BossManager.h"
#include "Component/Sound.h"
#include "../GameMode/BrainMode.h"
#include "Engine.h"
#include "Camera/CameraManager.h"
#include "Component/Camera.h"
#include "Effect.h"
#include "Scene/SceneManager.h"

float CBrainFreeze::m_fAttackTimeMax = 5.f;
float CBrainFreeze::m_fAccelTimeMax = 0.5f;
float CBrainFreeze::m_fFlyTimeMax = 1.f;
float CBrainFreeze::m_fMeltSpeed = -32.f;
float CBrainFreeze::m_fMeltTimeMax = 0.5f;
float CBrainFreeze::m_fLightTimeMax = 1.f;

CBrainFreeze::CBrainFreeze()    :
    m_pCube(nullptr)
    , m_pBrain(nullptr)
    , m_pLight(nullptr)
    , m_pShadow(nullptr)
    , m_pOC(nullptr)
    , m_pIceShadow(nullptr)
    , m_pRC(nullptr)
    , m_fAttackTime(5.f)
    , m_eDir(DIR_8::D)
    , m_vAccel()
    , m_vSpeed()
    , m_iPhase(0)
    , m_vPrevQuat()
    , m_vNextQuat()
    , m_fRot(0.f)
    , m_fAccelTime(0.f)
    , m_bAccel(false)
    , m_eNextDir(DIR_8::D)
    , m_iRot(0)
    , m_bFly(0)
    , m_fFlyTime(0.f)
    , m_bFall(false)
    , m_fFlyAccel(-100.f)
    , m_fFlySpeed(0.f)
    , m_fFlyDist(0.f)
    , m_bMelt(false)
    , m_fMeltTime(0.f)
    , m_bBrain(false)
#ifdef _DEBUG
    , m_pDir(nullptr)
    , m_pFly(nullptr)
#endif
    , m_bLight(false)
    , m_fLightTime(0.f)
    , m_bLightOut(false)
    , m_pBGM(nullptr)
    , m_pEft(nullptr)
{
    SetBossType(BOSS_TYPE::BRAINFREEZE);
}

CBrainFreeze::CBrainFreeze(const CBrainFreeze& obj) :
    CSoulMonster(obj)
    , m_pCube((CSpriteComponent*)FindSceneComponent("icecube"))
    , m_pBrain((CSpriteComponent*)FindSceneComponent("brain"))
    , m_pLight((CSpriteComponent*)FindSceneComponent("light"))
    , m_pShadow((CSpriteComponent*)FindSceneComponent("Shadow"))
    , m_pOC((CColliderOBB2D*)FindSceneComponent("IceBody"))
    , m_pIceShadow((CMesh2DComponent*)FindSceneComponent("IceShadow"))
    , m_pRC((CColliderRect*)FindSceneComponent("BrainBody"))
    , m_fAttackTime(obj.m_fAttackTime)
    , m_eDir(obj.m_eDir)
    , m_vAccel(obj.m_vAccel)
    , m_vSpeed(obj.m_vSpeed)
    , m_vNextQuat(m_pCube->GetTransform()->GetQuarternion())
    , m_vPrevQuat(m_vNextQuat)
    , m_iPhase(obj.m_iPhase)
    , m_fRot(obj.m_fRot)
    , m_fAccelTime(obj.m_fAccelTime)
    , m_bAccel(obj.m_bAccel)
    , m_eNextDir(obj.m_eNextDir)
    , m_iRot(obj.m_iRot)
    , m_bFly(obj.m_bFly)
    , m_fFlyTime(obj.m_fFlyTime)
    , m_bFall(obj.m_bFall)
    , m_fFlyAccel(obj.m_fFlyAccel)
    , m_fFlySpeed(obj.m_fFlySpeed)
    , m_fFlyDist(obj.m_fFlyDist)
    , m_bMelt(obj.m_bMelt)
    , m_fMeltTime(obj.m_fMeltTime)
    , m_bBrain(obj.m_bBrain)
#ifdef _DEBUG
    , m_pDir((CUIFont*)FindSceneComponent("Dir"))
    , m_pFly((CUIFont*)FindSceneComponent("Fly"))
#endif
    , m_bLight(obj.m_bLight)
    , m_fLightTime(obj.m_fLightTime)
    , m_bLightOut(obj.m_bLightOut)
    , m_pBGM((CSound*)FindSceneComponent("BGM"))
    , m_pEft((CSound*)FindSceneComponent("Effect"))
{
    m_pRC->SetCallBack<CBrainFreeze>(COLLISION_STATE::INIT, this, &CBrainFreeze::ColInit);
    m_pRC->SetCallBack<CBrainFreeze>(COLLISION_STATE::STAY, this, &CBrainFreeze::ColStay);
    m_pRC->SetCallBack<CBrainFreeze>(COLLISION_STATE::END, this, &CBrainFreeze::ColEnd);

    m_pOC->SetCallBack<CBrainFreeze>(COLLISION_STATE::INIT, this, &CBrainFreeze::ColInit);
    m_pOC->SetCallBack<CBrainFreeze>(COLLISION_STATE::STAY, this, &CBrainFreeze::ColStay);
    m_pOC->SetCallBack<CBrainFreeze>(COLLISION_STATE::END, this, &CBrainFreeze::ColEnd);
#ifdef _DEBUG
    m_pInput->SetAxisFunc("1", this, &CBrainFreeze::RotX);
    m_pInput->SetAxisFunc("2", this, &CBrainFreeze::RotY);
    m_pInput->SetAxisFunc("3", this, &CBrainFreeze::RotZ);
    m_pInput->SetAxisFunc("4", this, &CBrainFreeze::RotX);
    m_pInput->SetAxisFunc("5", this, &CBrainFreeze::RotY);
    m_pInput->SetAxisFunc("6", this, &CBrainFreeze::RotZ);
    m_pInput->SetActionFunc("Z", KEY_TYPE::KT_DOWN,this, &CBrainFreeze::NextBrain);
#endif
}

CBrainFreeze::~CBrainFreeze()
{
    m_pBGM->SetSound("NormalMovementLoop");
    m_pBGM->Stop();

    m_pBGM->SetSound("FastMovementLoop");
    m_pBGM->Stop();

    SAFE_RELEASE(m_pCube);
    SAFE_RELEASE(m_pBrain);
    SAFE_RELEASE(m_pLight);
    SAFE_RELEASE(m_pShadow);
    SAFE_RELEASE(m_pOC);
    SAFE_RELEASE(m_pIceShadow);
    SAFE_RELEASE(m_pRC);
#ifdef _DEBUG
    SAFE_RELEASE(m_pDir);
    SAFE_RELEASE(m_pFly);
#endif
    SAFE_RELEASE(m_pBGM);
    SAFE_RELEASE(m_pEft);
}

void CBrainFreeze::SetState(State eStat)
{
    CSoulMonster::SetState(eStat);

    switch (eStat)
    {
    case State::SLEEP:
        break;
    case State::IDLE:
        break;
    case State::ATTACK:
    {
        m_pBGM->SetSoundAndPlay("BrainA");

        CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

        pCam->SetFocus(this);
        pCam->SetMax(500.f);
        pCam->SetMin(50.f);

        SAFE_RELEASE(pCam);
    }
        break;
    case State::STOP:
        m_pBrain->SetShader("GrayShader");

        m_pBGM->SetSound("BrainB");
        m_pBGM->Stop();

        break;
    case State::DIE:
        GET_SINGLE(CBossManager)->AddMonster(this);

        ((CBrainMode*)m_pScene->GetGameMode())->Clear();

        CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

        pCam->SetFocus(nullptr);

        SAFE_RELEASE(pCam);

        m_pBrain->SetWorldScale(32.f, 24.f, 0.f);
        m_pShadow->SetWorldScale(32.f, 16.f, 0.f);
        m_pShadow->AddRelativePos(0.f, 16.f, 0.f);

        break;
    }
}

bool CBrainFreeze::Init()
{
    if (!CSoulMonster::Init())
        return false;

#ifdef _DEBUG
    m_pInput->SetActionFunc("Z", KEY_TYPE::KT_DOWN, this, &CBrainFreeze::NextBrain);
#endif

	m_pCube = CreateComponent<CSpriteComponent>("icecube",
		"IMG\\BOSS\\BRAIN\\icecube.obj",DATA_PATH, m_pScene->FindLayer("Fore"));

	m_pCube->SetInheritScale(false);
	m_pCube->SetRelativeScale(32.f, 32.f, 32.f);
	m_pCube->CreateSprite("Idle", "BrainIdle", LOOP_OPTION::LOOP);
	m_pCube->AddRenderState("DepthNoWrite");
    m_pCube->AddRenderState("NoCullBack");
    m_pCube->AddRenderState("AlphaBlend");
    m_pCube->AddQuaternionRotNorm(Vector4(1.f, 0.f, 0.f, 0.f), -45.f);

    m_vNextQuat = m_vPrevQuat = m_pCube->GetTransform()->GetQuarternion();

	SetRootComponent(m_pCube);

    m_pBrain = CreateComponent<CSpriteComponent>("brain", m_pScene->FindLayer("Ground"));

    m_pCube->AddChild(m_pBrain);

    m_pBrain->SetWorldScale(64.f, 48.f, 0.f);
    m_pBrain->AddRelativePos(0.f, 32.f, 0.f);
    m_pBrain->SetPivot(0.5f, 0.5f, 0.f);
    m_pBrain->SetInheritScale(false);
    m_pBrain->CreateSprite("D", "BrainD", LOOP_OPTION::LOOP);
    m_pBrain->CreateSprite("L", "BrainL", LOOP_OPTION::LOOP);
    m_pBrain->CreateSprite("R", "BrainR", LOOP_OPTION::LOOP);
    m_pBrain->CreateSprite("U", "BrainU", LOOP_OPTION::LOOP);
    m_pBrain->CreateSprite("LD", "BrainLD", LOOP_OPTION::LOOP);
    m_pBrain->CreateSprite("RD", "BrainRD", LOOP_OPTION::LOOP);
    m_pBrain->CreateSprite("LU", "BrainLU", LOOP_OPTION::LOOP);
    m_pBrain->CreateSprite("RU", "BrainRU", LOOP_OPTION::LOOP);
    m_pBrain->CreateSprite("FlyD", "BrainFlyD", LOOP_OPTION::LOOP);
    m_pBrain->CreateSprite("FlyL", "BrainFlyL", LOOP_OPTION::LOOP);
    m_pBrain->CreateSprite("FlyR", "BrainFlyR", LOOP_OPTION::LOOP);
    m_pBrain->CreateSprite("FlyU", "BrainFlyU", LOOP_OPTION::LOOP);
    m_pBrain->AddRenderState("ForeGround");
    m_pBrain->SetInheritRotX(false);

    m_pLight = CreateComponent<CSpriteComponent>("light", m_pScene->FindLayer("Fore"));

    m_pCube->AddChild(m_pLight);

    m_pLight->SetWorldScale(64.f, 48.f, 0.f);
    m_pLight->SetPivot(0.5f, 0.5f, 0.f);
    m_pLight->AddRelativePos(0.f,32.f, 0.f);
    m_pLight->CreateSprite("D", "BrainLightD", LOOP_OPTION::LOOP);
    m_pLight->CreateSprite("L", "BrainLightL", LOOP_OPTION::LOOP);
    m_pLight->CreateSprite("R", "BrainLightR", LOOP_OPTION::LOOP);
    m_pLight->CreateSprite("U", "BrainLightU", LOOP_OPTION::LOOP);
    m_pLight->CreateSprite("LD", "BrainLightLD", LOOP_OPTION::LOOP);
    m_pLight->CreateSprite("RD", "BrainLightRD", LOOP_OPTION::LOOP);
    m_pLight->CreateSprite("LU", "BrainLightLU", LOOP_OPTION::LOOP);
    m_pLight->CreateSprite("RU", "BrainLightRU", LOOP_OPTION::LOOP);
    m_pLight->SetInheritScale(false);
    m_pLight->SetInheritRotX(false);
    m_pLight->AddRenderState("DepthNoWrite");

    CMaterial* pLightMtrl = m_pLight->GetMaterial();

    pLightMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.f);

    SAFE_RELEASE(pLightMtrl);

    m_pShadow = CreateComponent<CSpriteComponent>("Shadow", m_pScene->FindLayer("Temp"));

    m_pShadow->SetWorldScale(64.f, 32.f, 0.f);
    m_pShadow->SetInheritScale(false);
    m_pShadow->SetInheritRotX(false);
    m_pShadow->SetPivot(0.5f, 0.5f, 0.f);
    m_pShadow->CreateSprite("Idle", "BrainShadow", LOOP_OPTION::LOOP);
    m_pShadow->AddRenderState("AlphaBlend");
    m_pShadow->AddRenderState("DepthNoWrite");
    m_pShadow->Enable(false);

    CMaterial* pShadowMtrl = m_pShadow->GetMaterial();

    pShadowMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.5f);

    SAFE_RELEASE(pShadowMtrl);

    m_pCube->AddChild(m_pShadow);

    m_pIceShadow = CreateComponent<CMesh2DComponent>("IceShadow", m_pScene->FindLayer("Ground"));

    m_pIceShadow->SetWorldScale(64.f, 64.f, 0.f);
    m_pIceShadow->SetInheritScale(false);
    m_pIceShadow->SetPivot(0.5f, 0.5f, 0.f);
    m_pIceShadow->AddQuaternionRotNorm(Vector4(1.f, 0.f, 0.f, 0.f), 90.f);
    m_pIceShadow->AddRenderState("AlphaBlend");
    m_pIceShadow->AddRenderState("DepthNoWrite");
    m_pIceShadow->SetTexture(REGISTER_TYPE::RT_DIF, "util");

    CMaterial* pMtrl = m_pIceShadow->GetMaterial();

    pMtrl->SetDiffuseColor(0.f, 0.f, 0.f, 0.5f);

    SAFE_RELEASE(pMtrl);

    m_pCube->AddChild(m_pIceShadow);

    m_pOC = CreateComponent<CColliderOBB2D>("IceBody", m_pLayer);

    m_pOC->SetExtend(32.f, 24.f, 0.f);
    m_pOC->AddQuaternionRotNorm(Vector4(1.f, 0.f, 0.f, 0.f), 90.f);

    m_pOC->SetCallBack<CBrainFreeze>(COLLISION_STATE::INIT, this, &CBrainFreeze::ColInit);
    m_pOC->SetCallBack<CBrainFreeze>(COLLISION_STATE::STAY, this, &CBrainFreeze::ColStay);
    m_pOC->SetCallBack<CBrainFreeze>(COLLISION_STATE::END, this, &CBrainFreeze::ColEnd);

    m_pCube->AddChild(m_pOC);

    m_pRC = CreateComponent<CColliderRect>("BrainBody", m_pLayer);

    m_pRC->SetExtent(48.f, 32.f);
    m_pRC->SetRelativePos(0.f, -8.f, 0.f);
    m_pRC->Enable(false);

    m_pRC->SetCallBack<CBrainFreeze>(COLLISION_STATE::INIT, this, &CBrainFreeze::ColInit);
    m_pRC->SetCallBack<CBrainFreeze>(COLLISION_STATE::STAY, this, &CBrainFreeze::ColStay);
    m_pRC->SetCallBack<CBrainFreeze>(COLLISION_STATE::END, this, &CBrainFreeze::ColEnd);

    m_pBrain->AddChild(m_pRC);
#ifdef _DEBUG
    m_pDir = CreateComponent<CUIFont>("Dir", m_pScene->FindLayer("UI"));

    m_pCube->AddChild(m_pDir);

    m_pDir->SetRect(0.f, 50.f, 300.f, 0.f);
    m_pDir->SetRelativePos(100.f, 100.f, 0.f);
    m_pDir->SetInheritPos(false);

    m_pFly = CreateComponent<CUIFont>("Fly", m_pScene->FindLayer("UI"));

    m_pFly->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
    m_pFly->SetWorldPos(200.f, 200.f, 0.f);
    m_pFly->SetRect(0.f, 50.f, 300.f, 0.f);
    m_pFly->SetInheritPos(false);

    m_pCube->AddChild(m_pFly);
#endif

    CParticle* pIce = CreateComponent<CParticle>("Ice", m_pScene->FindLayer("Fore"));

    pIce->AddParticle("Ice");
    pIce->AddParticle("IceShare");
    pIce->SetSpawnLimit(0.05f);
    pIce->SetUVStart(96.f, 96.f);
    pIce->SetUVEnd(128.f, 128.f);
    pIce->SetUVSize(256.f, 256.f);
    pIce->AddRenderState("DepthNoWrite");    
    pIce->AddRelativePos(0.f, 64.f, 0.f);
    pIce->SetInheritScale(false);
    pIce->SetInheritRotX(false);
    pIce->SetRelativeScale(1.f, 1.f, 1.f);

    CMaterial* pIceMtrl = pIce->GetMaterial();

    pIceMtrl->SetTexture(REGISTER_TYPE::RT_DIF, "Effect", 0, 1,
        (int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

    SAFE_RELEASE(pIceMtrl);

    m_pCube->AddChild(pIce);

    SAFE_RELEASE(pIce);

    CParticle* pSnow = CreateComponent<CParticle>("Snow", m_pScene->FindLayer("Fore"));

    pSnow->AddParticle("Snow");
    pSnow->AddParticle("SnowShare");
    pSnow->SetSpawnLimit(0.08f);
    pSnow->SetUVStart(0.f, 0.f);
    pSnow->SetUVEnd(16.f, 16.f);
    pSnow->SetUVSize(256.f, 256.f);
    pSnow->AddRenderState("DepthNoWrite");
    pSnow->AddRelativePos(0.f, 64.f, 0.f);
    pSnow->SetWorldScale(1.f, 1.f, 1.f);
    pSnow->SetInheritScale(false);
    pSnow->SetInheritRotX(false);

    CMaterial* pSnowMtrl = pSnow->GetMaterial();

    pSnowMtrl->SetTexture(REGISTER_TYPE::RT_DIF, "Effect", 0, 1,
        (int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

    SAFE_RELEASE(pSnowMtrl);

    m_pCube->AddChild(pSnow);

    SAFE_RELEASE(pSnow);

    m_pBGM = CreateComponent<CSound>("BGM", m_pLayer);

    m_pBGM->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

    m_pBrain->AddChild(m_pBGM);

    m_pEft = CreateComponent<CSound>("Effect", m_pLayer);

    m_pEft->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

    m_pBrain->AddChild(m_pEft);

    return true;
}

bool CBrainFreeze::Init(const char* pFileName, const std::string& strPathKey)
{
    return false;
}

void CBrainFreeze::Start()
{
    CSoulMonster::Start();
}

void CBrainFreeze::Update(float fTime)
{
    CSoulMonster::Update(fTime);

    State eStat = GetState();

    if (eStat == State::ATTACK&& !m_bMelt )
    {
        if (!m_bBrain)
        {
            m_fAttackTime += fTime;

            if (m_fAttackTime >= m_fAttackTimeMax)
            {
                m_fAttackTime -= m_fAttackTimeMax;

                m_bLight = true;

                ++m_iPhase;

                CObj* pObj = m_pScene->GetGameMode()->GetPlayer();

                Vector3 vTargetPos = pObj->GetWorldPos();

                SAFE_RELEASE(pObj);

                Vector3 vPos = GetWorldPos();

                Vector3 vDir = vTargetPos - vPos;

                vDir.Normalize();

                Vector3 vAxis(1.f, 0.f, 0.f);

                float fAngle = acosf(vAxis.Dot(vDir));

                if (m_iPhase < 4)
                {
                    if (fAngle < XM_PIDIV4 / 2.f)
                    {
                        m_eNextDir = DIR_8::R;
                    }

                    else if (fAngle > XM_PIDIV4 / 2.f && fAngle < XM_PIDIV4 / 2.f * 3.f)
                    {
                        m_eNextDir = DIR_8::RU;
                    }

                    else if (fAngle > XM_PIDIV4 / 2.f * 3.f && fAngle < XM_PIDIV4 / 2.f * 5.f)
                    {
                        m_eNextDir = DIR_8::U;
                    }

                    else if (fAngle > XM_PIDIV4 / 2.f * 5.f && fAngle < XM_PIDIV4 / 2.f * 7.f)
                    {
                        m_eNextDir = DIR_8::LU;
                    }

                    else
                    {
                        m_eNextDir = DIR_8::L;
                    }
                }

                else
                {
                    if (fAngle < XM_PIDIV4)
                    {
                        m_eNextDir = DIR_8::R;
                    }

                    else if (fAngle > XM_PIDIV4 && fAngle < XM_PIDIV4 * 3.f)
                    {
                        m_eNextDir = DIR_8::U;
                    }

                    else if (fAngle > XM_PIDIV4 * 3.f)
                    {
                        m_eNextDir = DIR_8::L;
                    }
                }

                if (vDir.y < 0.f)
                {
                    m_eNextDir = DIR_8((-(int)m_eNextDir + 12) % 8);
                }

                m_iRot = abs(((int)m_eDir - (int)m_eNextDir));

                m_iRot *= 45;

                Slerp(m_vPrevQuat, m_vNextQuat, 1.f);

                m_fRot = 0.f;

                m_vPrevQuat = m_pCube->GetTransform()->GetQuarternion();

                m_vNextQuat = XMQuaternionMultiply(m_vPrevQuat.Convert(),
                    XMQuaternionRotationAxis(Vector4(0.f, 1.f, -1.f, 0.f).Convert(), DegToRad((float)m_iRot)));

                if (m_iPhase >= 4)
                {
                    m_bFly = true;

                    m_pOC->Enable(false);
                    m_pRC->Enable(false);

                    m_iPhase = 0;

                    m_vSpeed = vDir * 100.f;

                    m_vSpeed.z = 0.f;

                    m_fFlySpeed = 150.f;

                    ((CTileMode*)m_pScene->GetGameMode())->SetFadeColor(0.f, 0.f, 0.f);

                    ((CTileMode*)m_pScene->GetGameMode())->SetFade(0.5f, 0.f, -0.5f);

                    m_pEft->SetSoundAndPlay("TelekinesisJump");
                }

                else
                {
                    m_bAccel = true;
#ifdef _DEBUG
                    m_vAccel = vDir * 1200.f;
#else
                    m_vAccel = vDir * 2000.f;
#endif

                    m_vAccel.z = 0.f;

                    m_pEft->SetSoundAndPlay("TelekinesisSlide");
                }
            }

            if (m_fRot < 1.f)
            {
                m_fRot += fTime;
                Slerp(m_vPrevQuat, m_vNextQuat, m_fRot);

                int iRate = 0;

                if (!m_bFly)
                {
                    iRate = (int)(m_iRot / 45 * (1 - m_fRot));
                }

                else
                {
                    iRate = (int)(m_iRot / 90 * (1 - m_fRot)) * 2;
                }

                m_eDir = (DIR_8)((int)m_eNextDir - iRate);
#ifdef _DEBUG
                TCHAR strDir[32] = {};

                swprintf_s(strDir, L"%d", m_eDir);

                m_pDir->SetText(strDir);
#endif
            }

            else
            {
                Slerp(m_vPrevQuat, m_vNextQuat, 1.f);
            }
        }

        else
        {
            if (!m_bFly)
            {
                m_bFly = true;
                m_fFlySpeed = 150.f;

                m_bLight = true;

                m_pEft->SetSound("BrainTelekinesis");
                m_pEft->SetVol(1.f);
                m_pEft->Play(fTime);
            }

            CObj* pObj = m_pScene->GetGameMode()->GetPlayer();

            Vector3 vTargetPos = pObj->GetWorldPos();

            SAFE_RELEASE(pObj);

            Vector3 vPos = GetWorldPos();

            Vector3 vDir = vTargetPos - Vector3(vPos.x, vPos.y - m_fFlyDist, vPos.z);

            vDir.Normalize();

            m_vSpeed = vDir * 50.f;

            Vector3 vAxis(1.f, 0.f, 0.f);

            float fAngle = acosf(vAxis.Dot(vDir));

            if (fAngle < XM_PIDIV4 / 2.f)
            {
                m_eDir = DIR_8::R;
            }

            else if (fAngle > XM_PIDIV4 / 2.f && fAngle < XM_PIDIV4 / 2.f * 3.f)
            {
                m_eDir = DIR_8::RU;
            }

            else if (fAngle > XM_PIDIV4 / 2.f * 3.f && fAngle < XM_PIDIV4 / 2.f * 5.f)
            {
                m_eDir = DIR_8::U;
            }

            else if (fAngle > XM_PIDIV4 / 2.f * 5.f && fAngle < XM_PIDIV4 / 2.f * 7.f)
            {
                m_eDir = DIR_8::LU;
            }

            else
            {
                m_eDir = DIR_8::L;
            }

            if (vDir.y < 0.f)
            {
                m_eDir = DIR_8((-(int)m_eDir + 12) % 8);
            }
#ifdef _DEBUG
            TCHAR strDir[32] = {};

            swprintf_s(strDir, L"%d", m_eDir);

            m_pDir->SetText(strDir);
#endif

            m_pRC->Enable(m_fFlyDist < 16.f);
        }
    }

    if (m_bAccel)
    {
        m_fAccelTime += fTime;

        if (m_fAccelTime >= m_fAccelTimeMax)
        {
            m_bAccel = false;

            m_fAccelTime = 0.f;
        }

        m_vSpeed += m_vAccel * fTime;
    }

    if (m_bFly)
    {

#ifdef _DEBUG
        m_pFly->SetText(L"Fly");
#endif

        m_fFlySpeed += m_fFlyAccel * fTime;

        if (!m_bFall)
        {
            m_fFlyTime += fTime;

            if (m_fFlySpeed <= 0.f)
            {
                m_bFall = true;
            }

            m_fFlyDist += m_fFlySpeed * fTime;
            AddWorldPos(0.f, m_fFlySpeed * fTime, 0.f);

            m_pIceShadow->AddRelativePos(0.f, -m_fFlySpeed * fTime, 0.f);
            m_pIceShadow->AddRelativeScale(-fTime * 16.f, -fTime * 16.f, 0.f);

            m_pShadow->AddRelativePos(0.f, -m_fFlySpeed * fTime, 0.f);
            m_pShadow->AddRelativeScale(-fTime * 16.f, -fTime * 8.f, 0.f);
        }

        else
        {
            m_fFlyTime -= fTime;

            if (m_fFlyDist <= 0.f)
            {
                m_bFall = false;
                m_bFly = false;

                if (!m_bBrain)
                {
                    m_pOC->Enable(true);
                }

                else
                {
                    m_pRC->Enable(true);

                    for (int i = 0; i < 4; ++i)
                    {
                        char strName[32] = {};

                        sprintf_s(strName, "Mucus%d", rand() % 4 + 1);

                        CObj* pMucus = m_pScene->CreateCloneObj(strName, strName, m_pLayer, m_pScene->GetSceneType());

                        pMucus->SetWorldPos(GetWorldPos() +
                            Vector3((rand() % 6400 - 3200) / 100.f, (rand() % 6400 - 3200) / 100.f, 0.f));

                        CSpriteComponent* pCom = pMucus->FindComByType<CSpriteComponent>();

                        pCom->AddCallBack("idle", "Scale", (CEffect*)pMucus, &CEffect::SetScaleCallBack);
                        pCom->AddCallBack("idle", "Destroy", (CEffect*)pMucus, &CEffect::Destroy);

                        SAFE_RELEASE(pCom);

                        SAFE_RELEASE(pMucus);
                    }
                }

                //m_vSpeed /= 2.f;

                ((CTileMode*)m_pScene->GetGameMode())->ShakeCam(300.f, 10.f);

                switch (m_eDir)
                {
                case DIR_8::U:
                case DIR_8::RU:
                    m_pEft->SetSound("Land1");
                    break;
                case DIR_8::R:
                case DIR_8::RD:
                    m_pEft->SetSound("Land2");
                    break;
                case DIR_8::D:
                case DIR_8::LD:
                    m_pEft->SetSound("Land3");
                    break;
                case DIR_8::L:
                case DIR_8::LU:
                    m_pEft->SetSound("Land4");
                    break;
                }

                m_pEft->SetVol(1.f);
                m_pEft->Play(fTime);
            }

            m_fFlyDist += m_fFlySpeed * fTime;

            AddWorldPos(0.f, m_fFlySpeed * fTime, 0.f);

            m_pIceShadow->AddRelativePos(0.f, -m_fFlySpeed* fTime, 0.f);
            m_pIceShadow->AddRelativeScale(fTime * 16.f, fTime * 16.f, 0.f);

            m_pShadow->AddRelativePos(0.f, -m_fFlySpeed * fTime, 0.f);
            m_pShadow->AddRelativeScale(fTime * 16.f, fTime * 8.f, 0.f);
        }
    }

    else
    {

#ifdef _DEBUG
        m_pFly->SetText(L"NotFly");
#endif
    }

    if (m_bMelt)
    {
        m_fMeltTime += fTime;

        if (m_fMeltTime >= m_fMeltTimeMax)
        {
            m_bBrain = true;
            m_bMelt = false;

            CParticle* pIce = (CParticle*)FindSceneComponent("Ice");

            pIce->Enable(false);

            SAFE_RELEASE(pIce);

            CParticle* pSnow = (CParticle*)FindSceneComponent("Snow");
                        
            pSnow->Enable(false);

            SAFE_RELEASE(pSnow);

            m_pCube->SetWorldScale(0.f, 0.f, 0.f);

            m_pIceShadow->Enable(false);
            m_pShadow->Enable(true);

            m_pBGM->SetSound("BrainA");
            m_pBGM->Stop();

            m_pBGM->SetSoundAndPlay("BrainB");            
        }

        else
        {
            m_pCube->AddWorldScale(m_fMeltSpeed * fTime, m_fMeltSpeed * fTime, m_fMeltSpeed * fTime);
            m_pIceShadow->AddWorldScale(m_fMeltSpeed* fTime, m_fMeltSpeed* fTime, m_fMeltSpeed* fTime);
        }
    }

    if (m_bLight)
    {
        m_fLightTime += fTime;

        if (m_fLightTime >= m_fLightTimeMax)
        {
            m_bLightOut = true;
            m_bLight = false;

            CMaterial* pMtrl = m_pLight->GetMaterial();

            pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 1.f);

            SAFE_RELEASE(pMtrl);
        }

        else
        {
            CMaterial* pMtrl = m_pLight->GetMaterial();

            pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, m_fLightTime / m_fLightTimeMax);

            SAFE_RELEASE(pMtrl);
        }
    }

    if (m_bLightOut)
    {
        m_fLightTime -= fTime /2.f;

        if (m_fLightTime <= 0.f)
        {
            m_fLightTime = 0.f;
            m_bLightOut = false;

            CMaterial* pMtrl = m_pLight->GetMaterial();

            pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.f);

            SAFE_RELEASE(pMtrl);
        }

        else
        {
            CMaterial* pMtrl = m_pLight->GetMaterial();

            pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, m_fLightTime / m_fLightTimeMax);

            SAFE_RELEASE(pMtrl);
        }
    }

    AddWorldPos(m_vSpeed * fTime);

    switch (m_eDir)
    {
    case DIR_8::U:
       /* if (!m_bFly)
        {*/
            m_pBrain->ChangeSequence("U");
       /* }

        else
        {
            m_pBrain->ChangeSequence("FlyU");
        }*/

        m_pLight->ChangeSequence("U");
        break;
    case DIR_8::RU:
        m_pBrain->ChangeSequence("RU");
        m_pLight->ChangeSequence("RU");
        break;
    case DIR_8::R:
            m_pBrain->ChangeSequence("R");
        m_pLight->ChangeSequence("R");
        break;
    case DIR_8::RD:
        m_pBrain->ChangeSequence("RD");
        m_pLight->ChangeSequence("RD");
        break;
    case DIR_8::D:
        /*if (!m_bFly)
        {*/
            m_pBrain->ChangeSequence("D");
        /*}

        else
        {
            m_pBrain->ChangeSequence("FlyD");
        }*/
        m_pLight->ChangeSequence("D");
        break;
    case DIR_8::LD:
        m_pBrain->ChangeSequence("LD");
        m_pLight->ChangeSequence("LD");
        break;
    case DIR_8::L:
        /*if (!m_bFly)
        {*/
            m_pBrain->ChangeSequence("L");
        /*}

        else
        {
            m_pBrain->ChangeSequence("FlyL");
        }*/
        m_pLight->ChangeSequence("L");
        break;
    case DIR_8::LU:
        m_pBrain->ChangeSequence("LU");
        m_pLight->ChangeSequence("LU");
        break;
    }
}

void CBrainFreeze::PostUpdate(float fTime)
{
    CSoulMonster::PostUpdate(fTime);

    static float fPrevSpeed = 0.f;

    float fSpeed = m_vSpeed.Length();

    if (fSpeed >= 150.f && fPrevSpeed < 150.f)
    {
        m_pBGM->SetSound("NormalMovementLoop");
        m_pBGM->Stop();

        m_pBGM->SetSoundAndPlay("FastMovementLoop");
    }

    else if (fSpeed < 150.f && fPrevSpeed >= 150.f)
    {
        m_pBGM->SetSound("FastMovementLoop");
        m_pBGM->Stop();

        m_pBGM->SetSoundAndPlay("NormalMovementLoop");
    }

    fPrevSpeed = fSpeed;
}

void CBrainFreeze::Collision(float fTime)
{
    CSoulMonster::Collision(fTime);

    Vector3 vPos = GetWorldPos();

    if (vPos.x < 296.f || vPos.x > 632.f)
    {

        AddWorldPos(-m_vSpeed * fTime);

        float fSpeed = m_vSpeed.Length();

        if (fSpeed >= 250.f)
        {
            if (vPos.x < 296.f)
            {
                m_pEft->SetSoundAndPlay("HeavyImpact4");
            }

            else
            {
                m_pEft->SetSoundAndPlay("HeavyImpact2");
            }
        }

        else
        {
			((CTileMode*)m_pScene->GetGameMode())->SetText(false);

            if (vPos.x < 296.f)
            {
                m_pEft->SetSoundAndPlay("SoftImpact4");
            }

            else
            {
                m_pEft->SetSoundAndPlay("SoftImpact2");
            }
        }

        m_vSpeed.x /= 3.f;

        m_vSpeed.x *= -1.f;

        ((CTileMode*)m_pScene->GetGameMode())->ShakeCam();

        if (m_eDir == DIR_8::LD ||
            m_eDir == DIR_8::RD ||
            m_eDir == DIR_8::LU ||
            m_eDir == DIR_8::RU)
        {
            m_eDir = (DIR_8)(((int)m_eDir + 1) % (int)DIR_8::END);

#ifdef _DEBUG
            TCHAR strDir[32] = {};

            swprintf_s(strDir, L"%d", m_eDir);

            m_pDir->SetText(strDir);
#endif
            m_vPrevQuat = m_vNextQuat;

            m_vNextQuat = XMQuaternionMultiply(m_vPrevQuat.Convert(),
                XMQuaternionRotationAxis(Vector4(0.f, 1.f, -1.f, 0.f).Convert(), DegToRad(45.f)));

            Slerp(m_vPrevQuat, m_vNextQuat, 1.f);

            m_eNextDir = m_eDir;
        }
    }

    if (!m_bFly)
    {
        if (vPos.y < 312.f || vPos.y > 632.f)
        {
            AddWorldPos(-m_vSpeed * fTime);

            ((CTileMode*)m_pScene->GetGameMode())->ShakeCam();

            float fSpeed = m_vSpeed.Length();

            if (fSpeed >= 150.f)
            {
                if (vPos.y < 312.f)
                {
                    m_pEft->SetSound("HeavyImpact3");
                }

                else
                {
                    m_pEft->SetSound("HeavyImpact1");
                }
            }

            else
            {
				((CTileMode*)m_pScene->GetGameMode())->SetText(false);

                if (vPos.y < 312.f)
                {
                    m_pEft->SetSound("SoftImpact3");
                }

                else
                {
                    m_pEft->SetSound("SoftImpact1");
                }
            }

            m_pEft->SetVol(1.f);
            m_pEft->Play(fTime);

            m_vSpeed.y /= 3.f;

            m_vSpeed.y *= -1.f;

            if (m_eDir == DIR_8::LD ||
                m_eDir == DIR_8::RD ||
                m_eDir == DIR_8::LU ||
                m_eDir == DIR_8::RU)
            {
                m_eDir = (DIR_8)(((int)m_eDir + 1) % (int)DIR_8::END);

#ifdef _DEBUG
                TCHAR strDir[32] = {};

                swprintf_s(strDir, L"%d", m_eDir);

                m_pDir->SetText(strDir);
#endif

                m_vPrevQuat = m_vNextQuat;

                m_vNextQuat = XMQuaternionMultiply(m_vPrevQuat.Convert(),
                    XMQuaternionRotationAxis(Vector4(0.f, 1.f, -1.f, 0.f).Convert(), DegToRad(45.f)));

                Slerp(m_vPrevQuat, m_vNextQuat, 1.f);

                m_eNextDir = m_eDir;
            }
        }
    }
}

void CBrainFreeze::PreRender(float fTime)
{
    CSoulMonster::PreRender(fTime);

    m_pOC->SetWorldRotX(0.f);
    m_pOC->SetWorldRotY(0.f);

    if (GET_SINGLE(CEngine)->IsImgui())
    {
        SpawnWindow();
    }
}

void CBrainFreeze::Render(float fTime)
{
    CSoulMonster::Render(fTime);
}

void CBrainFreeze::PostRender(float fTime)
{
    CSoulMonster::PostRender(fTime);
}

CBrainFreeze* CBrainFreeze::Clone()
{
    return new CBrainFreeze(*this);
}

void CBrainFreeze::Save(FILE* pFile)
{
    CSoulMonster::Save(pFile);
}

void CBrainFreeze::Load(FILE* pFile)
{
    CSoulMonster::Load(pFile);

    m_pCube = (CSpriteComponent*)FindSceneComponent("icecube");
    m_pBrain = (CSpriteComponent*)FindSceneComponent("brain");
    m_pLight = (CSpriteComponent*)FindSceneComponent("light");
    m_pShadow = (CSpriteComponent*)FindSceneComponent("Shadow");
    m_pOC = (CColliderOBB2D*)FindSceneComponent("IceBody");
    m_pIceShadow = (CMesh2DComponent*)FindSceneComponent("IceShadow");
    m_pRC = (CColliderRect*)FindSceneComponent("BrainBody");
#ifdef _DEBUG
    m_pDir = (CUIFont*)FindSceneComponent("Dir");
    m_pFly = (CUIFont*)FindSceneComponent("Fly");
#endif
    m_pBGM = (CSound*)FindSceneComponent("BGM");
    m_pEft = (CSound*)FindSceneComponent("Effect");

    CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();
    m_pShadow->SetLayer(pScene->FindLayer("Temp"));

    m_pRC->SetCallBack<CBrainFreeze>(COLLISION_STATE::INIT, this, &CBrainFreeze::ColInit);
    m_pRC->SetCallBack<CBrainFreeze>(COLLISION_STATE::STAY, this, &CBrainFreeze::ColStay);
    m_pRC->SetCallBack<CBrainFreeze>(COLLISION_STATE::END, this, &CBrainFreeze::ColEnd);

    m_pOC->SetCallBack<CBrainFreeze>(COLLISION_STATE::INIT, this, &CBrainFreeze::ColInit);
    m_pOC->SetCallBack<CBrainFreeze>(COLLISION_STATE::STAY, this, &CBrainFreeze::ColStay);
    m_pOC->SetCallBack<CBrainFreeze>(COLLISION_STATE::END, this, &CBrainFreeze::ColEnd);
}

void CBrainFreeze::RotX(float fScale, float fTime)
{
    m_pCube->AddQuaternionRotNorm(Vector4(1.f, 0.f, 0.f, 0.f), fTime * fScale);
}

void CBrainFreeze::RotY(float fScale, float fTime)
{
    m_pCube->AddQuaternionRotNorm(Vector4( 0.f, 1.f, 0.f, 0.f), fTime* fScale);
}

void CBrainFreeze::RotZ(float fScale, float fTime)
{
    m_pCube->AddQuaternionRotNorm(Vector4(0.f, 0.f, 1.f, 0.f), fTime* fScale);
}

void CBrainFreeze::ColInit(CCollider* pSrc, CCollider* pDst, float fTime)
{
    std::string strSrc = pSrc->GetName();
    std::string strDst = pDst->GetName();

    if (strDst == "BulletBody")
    {
        if (strSrc == "IceBody")
        {
            if (((CBullet*)pDst->GetObj())->GetSpeed())
            {
                State eStat = GetState();
                if (eStat == State::IDLE)
                {
                    SetState(State::ATTACK);

                    ((CTileMode*)m_pScene->GetGameMode())->Lock(true);

					((CTileMode*)m_pScene->GetGameMode())->SetText(true);
                }

                else if (((CBullet*)pDst->GetObj())->IsFire())
                {
                    m_bMelt = true;

                    m_pOC->Enable(false);
                    m_vAccel = Vector3::Zero;
                    m_vSpeed = Vector3::Zero;
                    m_pRC->Enable(true);

                    m_pEft->SetSoundAndPlay("Melt");
                }
            }
        }

        else if (strSrc == "BrainBody")
        {
            if (m_bBrain)
            {
                CBullet* pBullet = (CBullet*)pDst->GetObj();

                float fSpeed = pBullet->GetSpeed();

                if (fSpeed != 0.f)
                {
                    if (GetState() == State::ATTACK)
                    {
                        SetState(State::STOP);

                        FADE tFade = {};

                        tFade.fGray = 1.f;

                        GET_SINGLE(CShaderManager)->UpdateCBuffer("Fade", &tFade);
                        m_vAccel = Vector3::Zero;
                        m_vSpeed = Vector3::Zero;

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
        }
    }

    else if (strDst == "PlayerBody")
    {
        if (!m_bBrain)
        {
            m_pBGM->SetSound("BrainA");
            m_pBGM->Stop();
        }

        else
        {
            m_pBGM->SetSound("BrainB");
            m_pBGM->Stop();
        }

        m_pBGM->SetSound("NormalMovementLoop");
        m_pBGM->Stop();

        m_pBGM->SetSound("FastMovementLoop");
        m_pBGM->Stop();
    }
}

void CBrainFreeze::ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
{

}

void CBrainFreeze::ColEnd(CCollider* pSrc, CCollider* pDst, float fTime)
{
}

void CBrainFreeze::NextBrain(float, float)
{
    m_bMelt = true;
    m_pOC->Enable(false);
    static bool bCheck = false;
    if (!bCheck)
    {
        m_pBGM->SetSound("BrainA");
        m_pBGM->Stop();
        bCheck = true;
    }

	((CTileMode*)m_pScene->GetGameMode())->ShakeCam(200.f, 7.f, 2.f);
}

void CBrainFreeze::Cheat(float, float)
{
    SetState(State::DIE);
}

void CBrainFreeze::SpawnWindow()
{
    if (ImGui::Begin("BrainFreeze"))
    {
        switch (m_eDir)
        {
        case DIR_8::U:
            ImGui::Text("Dir: Up");
            break;
        case DIR_8::RU:
            ImGui::Text("Dir: Right Up");
            break;
        case DIR_8::R:
            ImGui::Text("Dir: Right");
            break;
        case DIR_8::RD:
            ImGui::Text("Dir: Right Down");
            break;
        case DIR_8::D:
            ImGui::Text("Dir: Down");
            break;
        case DIR_8::LD:
            ImGui::Text("Dir: Left Down");
            break;
        case DIR_8::L:
            ImGui::Text("Dir: Left");
            break;
        case DIR_8::LU:
            ImGui::Text("Dir: Left Up");
            break;
        }
        switch (m_eNextDir)
        {
        case DIR_8::U:
            ImGui::Text("Next Dir: Up");
            break;
        case DIR_8::RU:
            ImGui::Text("Next Dir: Right Up");
            break;
        case DIR_8::R:
            ImGui::Text("Next Dir: Right");
            break;
        case DIR_8::RD:
            ImGui::Text("Next Dir: Right Down");
            break;
        case DIR_8::D:
            ImGui::Text("Next Dir: Down");
            break;
        case DIR_8::LD:
            ImGui::Text("Next Dir: Left Down");
            break;
        case DIR_8::L:
            ImGui::Text("Next Dir: Left");
            break;
        case DIR_8::LU:
            ImGui::Text("Next Dir: Left Up");
            break;
        }
        ImGui::SliderInt("Phase", &m_iPhase, 0, 5);
        ImGui::SliderInt("Rot", &m_iRot, -180, 180);
        ImGui::SliderFloat("Rotf", &m_fRot, -180.f, 180.f);
        ImGui::SliderFloat4("PrevQuat", &m_vPrevQuat.x, -1.f, 1.f);
        ImGui::SliderFloat4("NextQuat", &m_vNextQuat.x, -1.f, 1.f);
        int iStat = (int)GetState();
        ImGui::SliderInt("State", &iStat, (int)State::SLEEP, (int)State::END);
        CSoulMonster::SetState((State)iStat);
    }
    ImGui::End();
}
