#include "Colossus.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderRect.h"

CColossus::CColossus()	:
	m_pHead(nullptr),
	m_pBody(nullptr),
	m_pLeftHand(nullptr),
	m_pRightHand(nullptr)
{
}

CColossus::CColossus(const CColossus& obj)	:
	m_pHead(static_cast<CSpriteComponent*>(FindSceneComponent("Head")))
	, m_pBody(static_cast<CSpriteComponent*>(FindSceneComponent("Body")))
	, m_pLeftHand(static_cast<CSpriteComponent*>(FindSceneComponent("LeftHand")))
	, m_pRightHand(static_cast<CSpriteComponent*>(FindSceneComponent("RightHand")))
{
}

CColossus::~CColossus()
{
	SAFE_RELEASE(m_pHead);
	SAFE_RELEASE(m_pBody);
	SAFE_RELEASE(m_pLeftHand);
	SAFE_RELEASE(m_pRightHand);
}

bool CColossus::Init()
{
	if(!CObj::Init())
		return false;

	m_pBody = CreateComponent<CSpriteComponent>("Body");
	m_pBody->CreateSprite("Idle", "ColossusBodyIdle", true);

	m_pBody->SetPivot(0.5f, 0.5f, 0.f);
	m_pBody->SetWorldScale(96.f, 128.f, 0.f);
	m_pBody->SetInheritScale(false);

	CColliderRect* pRC = CreateComponent<CColliderRect>("ColossusBody");

	pRC->SetExtent(36.f, 12.f);
	pRC->SetRelativePos(0.f, 8.f, 0.f);

	m_pBody->AddChild(pRC);

	SAFE_RELEASE(pRC);

	m_pHead = CreateComponent<CSpriteComponent>("Head");
	m_pHead->CreateSprite("Idle", "ColossusHeadIdle", true);

	m_pHead->SetPivot(0.5f, 0.5f, 0.f);
	m_pHead->SetWorldScale(64.f, 64.f, 0.f);
	m_pHead->SetRelativePos(0.f, 64.f, 0.f);
	m_pHead->SetInheritScale(false);

	m_pBody->AddChild(m_pHead);

	SetRootComponent(m_pBody);

	m_pLeftHand = CreateComponent<CSpriteComponent>("LeftHand");
	m_pLeftHand->CreateSprite("Idle", "ColossusLeftHandIdle", true);

	m_pLeftHand->SetPivot(0.5f, 0.5f, 0.f);
	m_pLeftHand->SetRelativePos(-32.f, -80.f, 0.f);
	m_pLeftHand->SetWorldScale(48.f, 48.f, 0.f);
	m_pLeftHand->SetInheritScale(false);
	
	CColliderRect* pLHRC = CreateComponent<CColliderRect>("LeftHandBody");

	pLHRC->SetExtent(32.f, 32.f);

	m_pLeftHand->AddChild(pLHRC);

	SAFE_RELEASE(pLHRC);

	m_pBody->AddChild(m_pLeftHand);

	m_pRightHand = CreateComponent<CSpriteComponent>("RightHand");
	m_pRightHand->CreateSprite("Idle", "ColossusRightHandIdle", true);

	m_pRightHand->SetPivot(0.5f, 0.5f, 0.f);
	m_pRightHand->SetRelativePos(32.f, -80.f, 0.f);
	m_pRightHand->SetWorldScale(48.f, 48.f, 0.f);
	m_pRightHand->SetInheritScale(false);
	m_pRightHand->SetWorldRotY(180.f);

	m_pBody->AddChild(m_pRightHand);

	CColliderRect* pRHRC = CreateComponent<CColliderRect>("RightHandBody");

	pRHRC->SetExtent(32.f, 32.f);

	m_pRightHand->AddChild(pRHRC);

	SAFE_RELEASE(pRHRC);

	return true;
}

void CColossus::Start()
{
	CObj::Start();
}

void CColossus::Update(float fTime)
{
	CObj::Update(fTime);
}

void CColossus::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);
}

void CColossus::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CColossus::PreRender(float fTime)
{
	CObj::PreRender(fTime);
}

void CColossus::Render(float fTime)
{
	CObj::Render(fTime);
}

void CColossus::PostRender(float fTime)
{
	CObj::PostRender(fTime);
}

CColossus* CColossus::Clone()
{
	return new CColossus(*this);
}

void CColossus::Save(FILE* pFile)
{
	CObj::Save(pFile);
}

void CColossus::Load(FILE* pFile)
{
	CObj::Load(pFile);
}
