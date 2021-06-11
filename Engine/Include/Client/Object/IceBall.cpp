#include "IceBall.h"
#include "Component/SpriteComponent.h"
#include "Resource/Material.h"
#include "Component/ColliderCircle.h"

CIceBall::CIceBall()	:
	m_pMesh(nullptr)
{
}

CIceBall::CIceBall(const CIceBall& ball)	:
	CObj(ball)
{
	if (ball.m_pMesh)
		m_pMesh = ball.m_pMesh->Clone();

	SetRootComponent(m_pMesh);

	m_pMesh->SetEndFunc<CIceBall>("ball", this, &CIceBall::Destroy);
}

CIceBall::~CIceBall()
{
	SAFE_RELEASE(m_pMesh);
}

void CIceBall::SetDir(const Vector3& v)
{
	m_vDir = v;
}

bool CIceBall::Init()
{
	if (!CObj::Init())
		return false;

	m_pMesh = CreateComponent<CSpriteComponent>("BallSprite", m_pLayer);

	m_pMesh->SetWorldScale(32.f, 32.f, 0.f);
	m_pMesh->SetPivot(0.5f, 0.5f, 0.f);
	m_pMesh->CreateSprite("ball", "IceBall", LOOP_OPTION::LOOP);

	SetRootComponent(m_pMesh);

	m_pMesh->SetEndFunc<CIceBall>("ball", this, &CIceBall::Destroy);

	CSpriteComponent* pShadow = CreateComponent<CSpriteComponent>("BallShadow", m_pLayer);

	pShadow->SetWorldScale(32.f, 32.f, 0.f);
	pShadow->CreateSprite("Shadow", "IceBallShadow", LOOP_OPTION::LOOP);

	pShadow->SetInheritScale(false);
	pShadow->SetInheritRotX(false);
	pShadow->SetInheritRotY(false);
	pShadow->SetInheritRotZ(false);
	pShadow->SetPivot(0.5f, 0.5, 0.f);

	pShadow->SetRelativePos(0.f, -32.f, 0.f);
	
	m_pMesh->AddChild(pShadow);

	SAFE_RELEASE(pShadow);

	CColliderCircle* pCC = CreateComponent<CColliderCircle>("BallBody", m_pLayer);

	pCC->SetRadius(16.f);

	m_pMesh->AddChild(pCC);

	SAFE_RELEASE(pCC);

	return true;
}

void CIceBall::Start()
{
	CObj::Start();
}

void CIceBall::Update(float fTime)
{
	CObj::Update(fTime);

	AddWorldPos(m_vDir * fTime * 550.f);
}

void CIceBall::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);
}

void CIceBall::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CIceBall::PreRender(float fTime)
{
	CObj::PreRender(fTime);
}

void CIceBall::Render(float fTime)
{
	CObj::Render(fTime);
}

void CIceBall::PostRender(float fTime)
{
	CObj::PostRender(fTime);
}

CIceBall* CIceBall::Clone()
{
	return new CIceBall(*this);
}
