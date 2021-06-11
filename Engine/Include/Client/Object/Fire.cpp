#include "pch.h"
#include "Fire.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderRect.h"
#include "Bullet.h"
#include "Resource/Material.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Sound.h"

CFire::CFire()  :
    m_pFire(nullptr)
    , m_pRC(nullptr)
    , m_pLight(nullptr)
    , m_pLightBig(nullptr)
    , m_bOn(false)
    , m_fScaleSpeed(10.f)
    , m_bOff(false)
    , m_fScale(0.f)
    , m_fScaleMax(1.f)
    , m_pBGM(nullptr)
    ,m_pEft(nullptr)
{
}

CFire::CFire(const CFire& fire) :
    CObj(fire)
    , m_pFire((CSpriteComponent*)FindSceneComponent("Fire"))
    , m_pRC((CColliderRect*)FindSceneComponent("FireBody"))
    , m_pLight((CSpriteComponent*)FindSceneComponent("FireLight"))
    , m_pLightBig((CSpriteComponent*)FindSceneComponent("FireLightBig"))
    , m_bOn(fire.m_bOn)
    , m_fScaleSpeed(fire.m_fScaleSpeed)
	, m_bOff(fire.m_bOff)
	, m_fScale(fire.m_fScale)
	, m_fScaleMax(fire.m_fScaleMax)
    , m_pBGM((CSound*)FindSceneComponent("BGM"))
    , m_pEft((CSound*)FindSceneComponent("Effect"))
{
    m_pRC->SetCallBack<CFire>(COLLISION_STATE::INIT, this, &CFire::ColInit);
    m_pRC->SetCallBack<CFire>(COLLISION_STATE::STAY, this, &CFire::ColStay);
    m_pRC->SetCallBack<CFire>(COLLISION_STATE::END, this, &CFire::ColEnd);
}

CFire::~CFire()
{
    m_pBGM->SetSound("BunsenLoop");
    m_pBGM->Stop();
    SAFE_RELEASE(m_pFire);
    SAFE_RELEASE(m_pRC);
    SAFE_RELEASE(m_pLight);
    SAFE_RELEASE(m_pLightBig);
    SAFE_RELEASE(m_pBGM);
    SAFE_RELEASE(m_pEft);
}

bool CFire::Init()
{
    if (!CObj::Init())
        return false;

    m_pFire = CreateComponent<CSpriteComponent>("Fire", m_pLayer);

    m_pFire->SetWorldScale(64.f, 64.f, 0.);
    m_pFire->SetPivot(0.5f, 0.f, 0.f);
    m_pFire->CreateSprite("Idle", "FireBig", LOOP_OPTION::LOOP, 16.f);
    m_pFire->SetWorldPos(464.f, 464.f, 0.f);

    SetRootComponent(m_pFire);

    m_pRC = CreateComponent<CColliderRect>("FireBody", m_pLayer);

    m_pFire->AddChild(m_pRC);

    m_pRC->SetExtent(32.f, 32.f);
    m_pRC->SetRelativePos(0.f, 16.f, 0.f);

    CScene* pScene = GET_SINGLE(CSceneManager)->GetNextScene();

    m_pLight = CreateComponent<CSpriteComponent>("FireLight", pScene->FindLayer("BackDefault"));

    m_pLight->SetWorldScale(64.f, 64.f, 0.);
    m_pLight->SetPivot(0.5f, 0.5f, 0.f);
    m_pLight->CreateSprite("Idle", "FireLight", LOOP_OPTION::LOOP);
    m_pLight->SetInheritScale(false);
    m_pLight->AddRenderState("AlphaBlend");
    m_pLight->AddRenderState("DepthNoWrite");
    m_pLight->AddRelativePos(0.f, 16.f, 0.f);

    CMaterial* pLightMtrl = m_pLight->GetMaterial();

    pLightMtrl->SetDiffuseColor(1.f, 0.5f, 0.15f, 0.5f);

    SAFE_RELEASE(pLightMtrl);
     
    m_pFire->AddChild(m_pLight);

    m_pLightBig = CreateComponent<CSpriteComponent>("FireLightBig", pScene->FindLayer("BackDefault"));

    m_pLightBig->SetWorldScale(128.f, 128.f, 0.);
    m_pLightBig->SetPivot(0.5f, 0.5f, 0.f);
    m_pLightBig->CreateSprite("Idle", "FireLight", LOOP_OPTION::LOOP);
    m_pLightBig->SetInheritScale(false);
    m_pLightBig->AddRenderState("AlphaBlend");
    m_pLightBig->AddRenderState("DepthNoWrite");
    m_pLightBig->AddRelativePos(0.f, 16.f, 0.f);

    CMaterial* pLightBigMtrl = m_pLightBig->GetMaterial();

    pLightBigMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.25f);

    SAFE_RELEASE(pLightBigMtrl);

    m_pFire->AddChild(m_pLightBig);

    m_pRC->SetCallBack<CFire>(COLLISION_STATE::INIT, this, &CFire::ColInit);
    m_pRC->SetCallBack<CFire>(COLLISION_STATE::STAY, this, &CFire::ColStay);
    m_pRC->SetCallBack<CFire>(COLLISION_STATE::END, this, &CFire::ColEnd);

    m_pBGM = CreateComponent<CSound>("BGM", m_pLayer);

    m_pBGM->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

    m_pFire->AddChild(m_pBGM);

    m_pEft = CreateComponent<CSound>("Effect", m_pLayer);

    m_pEft->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

    m_pFire->AddChild(m_pEft);

    return true;
}

void CFire::Start()
{
    CObj::Start();
}

void CFire::Update(float fTime)
{
    CObj::Update(fTime);

    if (m_bOn)
    {
        m_fScale += fTime * m_fScaleSpeed;

        if (m_fScale >= m_fScaleMax)
        {
            m_bOn = false;
            SetWorldScale(32.f, 32.f, 0.f);
            m_pLight->SetWorldScale(32.f, 32.f, 0.f);
            m_pLightBig->SetWorldScale(64.f, 64.f, 0.);
        }

        else
        {
            SetWorldScale(32.f * m_fScale, 32.f * m_fScale, 0.f);
            m_pLight->SetWorldScale(32.f * m_fScale, 32.f * m_fScale, 0.f);
            m_pLightBig->SetWorldScale(64.f * m_fScale, 64.f * m_fScale, 0.);
        }
    }

    else if (m_bOff)
    {
        m_fScale -= fTime * m_fScaleSpeed;

        if (m_fScale <= 0.f)
        {
            m_bOff = false;
            SetWorldScale(0.f, 0.f, 0.f);
            m_pLight->SetWorldScale(0.f, 0.f, 0.f);
            m_pLightBig->SetWorldScale(0.f, 0.f, 0.);
            m_bEnable = false;
        }

        else
        {
            SetWorldScale(32.f * m_fScale, 32.f * m_fScale, 0.f);
            m_pLight->SetWorldScale(32.f * m_fScale, 32.f * m_fScale, 0.f);
            m_pLightBig->SetWorldScale(64.f * m_fScale, 64.f * m_fScale, 0.);
        }
    }
}

void CFire::PostUpdate(float fTime)
{
    CObj::PostUpdate(fTime);
}

void CFire::Collision(float fTime)
{
    CObj::Collision(fTime);
}

void CFire::PreRender(float fTime)
{
    CObj::PreRender(fTime);
}

void CFire::Render(float fTime)
{
    CObj::Render(fTime);
}

void CFire::PostRender(float fTime)
{
    CObj::PostRender(fTime);
}

CFire* CFire::Clone()
{
    return new CFire(*this);
}

void CFire::Save(FILE* pFile)
{
    CObj::Save(pFile);
}

void CFire::Load(FILE* pFile)
{
    CObj::Load(pFile);
}

void CFire::ColInit(CCollider* pSrc, CCollider* pDst, float fTime)
{
    std::string strDst = pDst->GetName();

    if (strDst == "BulletBody")
    {
        ((CBullet*)pDst->GetObj())->SetFire(true);
    }

    else if (strDst == "PlayerBody")
    {
        m_pBGM->SetSound("BunsenLoop");
        m_pBGM->Stop();

        m_pBGM->SetSound("BrainA");
        m_pBGM->Stop();

        m_pBGM->SetSound("BrainB");
        m_pBGM->Stop();
    }
}

void CFire::ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
{
    std::string strDst = pDst->GetName();

    if (strDst == "BulletBody")
    {
        ((CBullet*)pDst->GetObj())->SetFire(true);
    }

    else if (strDst == "PlayerBody")
    {
        m_pBGM->SetSound("BunsenLoop");
        m_pBGM->Stop();

        m_pBGM->SetSound("BrainA");
        m_pBGM->Stop();

        m_pBGM->SetSound("BrainB");
        m_pBGM->Stop();
    }
}

void CFire::ColEnd(CCollider* pSrc, CCollider* pDst, float fTime)
{
}

void CFire::StartFire()
{
    m_bOn = true;
    m_bOff = false;

    m_bEnable = true;

    SetWorldScale(0.f, 0.f, 0.f);

    m_pBGM->SetSoundAndPlay("BunsenLoop");

    m_pEft->SetSoundAndPlay("BunsenIgnite");
}

void CFire::EndFire()
{
    m_bOn = false;
    m_bOff = true;

    m_pBGM->SetSound("BunsenLoop");
    m_pBGM->Stop();

    m_pEft->SetSound("BunsenIgnite");
    m_pEft->Stop();
}
