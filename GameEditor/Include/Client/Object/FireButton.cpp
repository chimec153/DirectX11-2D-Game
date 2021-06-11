#include "pch.h"
#include "FireButton.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderRect.h"
#include "Scene/Scene.h"
#include "BrainFreeze.h"
#include "../GameMode/BrainMode.h"
#include "Fire.h"
#include "Component/Sound.h"

CFireButton::CFireButton() :
    m_pButton(nullptr)
    , m_pRC(nullptr)
    , m_bPush(false)
    , m_pEft(nullptr)
{
}

CFireButton::CFireButton(const CFireButton& obj)    :
    CObj(obj)
    , m_pButton((CSpriteComponent*)FindSceneComponent("Button"))
    , m_pRC((CColliderRect*)FindSceneComponent("ButtonBody"))
    , m_bPush(false)
    , m_pEft((CSound*)FindSceneComponent("Effect"))
{
    m_pRC->SetCallBack<CFireButton>(COLLISION_STATE::INIT, this, &CFireButton::ColInit);
    m_pRC->SetCallBack<CFireButton>(COLLISION_STATE::STAY, this, &CFireButton::ColStay);
    m_pRC->SetCallBack<CFireButton>(COLLISION_STATE::END, this, &CFireButton::ColEnd);
}

CFireButton::~CFireButton()
{
    SAFE_RELEASE(m_pButton);
    SAFE_RELEASE(m_pRC);
    SAFE_RELEASE(m_pEft);
}

bool CFireButton::Init()
{
    if (!CObj::Init())
        return false;

    m_pButton = CreateComponent<CSpriteComponent>("Button", m_pScene->FindLayer("Temp"));

    m_pButton->CreateSprite("Idle", "FireButton", LOOP_OPTION::LOOP);
    m_pButton->CreateSprite("Push", "FireButtonPush", LOOP_OPTION::LOOP);
    m_pButton->SetWorldScale(32.f, 32.f, 0.f);
    m_pButton->SetPivot(0.5f, 0.5f, 0.f);
    m_pButton->AddRenderState("DepthNoWrite");

    SetRootComponent(m_pButton);

    m_pRC = CreateComponent<CColliderRect>("ButtonBody", m_pLayer);

    m_pButton->AddChild(m_pRC);

    m_pRC->SetExtent(32.f, 32.f);

    m_pEft = CreateComponent<CSound>("Effect", m_pLayer);

    m_pEft->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

    m_pButton->AddChild(m_pEft);

    return true;
}

void CFireButton::Update(float fTime)
{
    CObj::Update(fTime);
}

void CFireButton::PostUpdate(float fTime)
{
    CObj::PostUpdate(fTime);
}

void CFireButton::Collision(float fTime)
{
    CObj::Collision(fTime);
}

void CFireButton::PreRender(float fTime)
{
    CObj::PreRender(fTime);
}

void CFireButton::Render(float fTime)
{
    CObj::Render(fTime);
}

void CFireButton::PostRender(float fTime)
{
    CObj::PostRender(fTime);
}

CFireButton* CFireButton::Clone()
{
    return new CFireButton(*this);
}

void CFireButton::Save(FILE* pFile)
{
    CObj::Save(pFile);
}

void CFireButton::Load(FILE* pFile)
{
    CObj::Load(pFile);
}

void CFireButton::ColInit(CCollider* pSrc, CCollider* pDst, float fTime)
{
    std::string strDst = pDst->GetName();

    if (strDst == "IceBody")
    {
        if (!m_bPush)
        {
            m_bPush = true;

            m_pButton->ChangeSequence("Push");

            CFire* pFire = ((CBrainMode*)m_pScene->GetGameMode())->GetFire();

            pFire->StartFire();

            SAFE_RELEASE(pFire);

            m_pEft->SetSound("ButtonContact");
            m_pEft->SetVol(1.f);
            m_pEft->Play(fTime);

            m_pEft->SetSound("ButtonPush");
            m_pEft->SetVol(1.f);
            m_pEft->Play(fTime);
        }

    }
}

void CFireButton::ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
{
    std::string strDst = pDst->GetName();

    if (strDst == "IceBody")
    {
        if (!m_bPush)
        {
            m_bPush = true;

            m_pButton->ChangeSequence("Push");

            CFire* pFire = ((CBrainMode*)m_pScene->GetGameMode())->GetFire();

            pFire->StartFire();

            SAFE_RELEASE(pFire);
        }
    }
}

void CFireButton::ColEnd(CCollider* pSrc, CCollider* pDst, float fTime)
{
    std::string strDst = pDst->GetName();

    if (strDst == "IceBody")
    {
        if (m_bPush)
        {
            m_bPush = false;
            m_pButton->ChangeSequence("Idle");

            CFire* pFire = ((CBrainMode*)m_pScene->GetGameMode())->GetFire();

            pFire->EndFire();

            SAFE_RELEASE(pFire);
        }
    }
}
