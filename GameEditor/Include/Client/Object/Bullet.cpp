#include "Bullet.h"
#include "Component/Mesh2DComponent.h"
#include "Resource/Material.h"

CBullet::CBullet()	:
	m_fDist(0.f),
	m_fLimitDist(600.f),
	m_fSpeed(800.f),
	m_pMesh(nullptr)
{
}

CBullet::CBullet(const CBullet& bullet)	:
	CObj(bullet)
{
	m_fDist = bullet.m_fDist;
	m_fLimitDist = bullet.m_fLimitDist;
	m_fSpeed = bullet.m_fSpeed;
	m_pMesh = bullet.m_pMesh->Clone();
}

CBullet::~CBullet()
{
	SAFE_RELEASE(m_pMesh);
}

bool CBullet::Init()
{
	m_pMesh = CreateComponent<CMesh2DComponent>("Mesh");

	SetRootComponent(m_pMesh);

	m_pMesh->SetRelativeScale(50.f, 50.f, 1.f);
	m_pMesh->SetPivot(0.5f, 0.5f, 0.f);

	CMaterial* pMaterial = m_pMesh->GetMaterial();

	pMaterial->SetDiffuseColor(0.f, 1.f, 1.f, 1.f);

	SAFE_RELEASE(pMaterial);

	return true;
}

void CBullet::Start()
{
}

void CBullet::Update(float fTime)
{
	CObj::Update(fTime);

	AddRelativePos(m_pMesh->GetRelativeAxis(AXIS_Y) * m_fSpeed * fTime);

	m_fDist += fTime * m_fSpeed;

	if (m_fDist >= m_fLimitDist)
	{
		Destroy();
	}
}

void CBullet::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);
}

void CBullet::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CBullet::PreRender(float fTime)
{
	CObj::PreRender(fTime);
}

void CBullet::Render(float fTime)
{
	CObj::Render(fTime);
}

void CBullet::PostRender(float fTime)
{
	CObj::PostRender(fTime);
}

CBullet* CBullet::Clone()
{
	return new CBullet(*this);
}
