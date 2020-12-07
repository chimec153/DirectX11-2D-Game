#include "ColliderCircle.h"
#include "../Resource/ResourceManager.h"
#include "../Collision/Collision.h"
#include "ColliderRect.h"
#include "ColliderOBB2D.h"
#include "ColliderPixel.h"
#include "ColliderLine.h"
#include "ColliderPoint.h"

CColliderCircle::CColliderCircle()	:
	m_tInfo()
{
	m_eColType = COLLIDER_TYPE::CT_SPHERE;
	m_eSceneComponentClassType = SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDERCIRCLE;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_2D;
	m_eType = COMPONENT_TYPE::CT_SCENE;
	m_b2D = true;
}

CColliderCircle::CColliderCircle(const CColliderCircle& col)	:
	CCollider(col)
{
	m_tInfo = col.m_tInfo;
}

CColliderCircle::~CColliderCircle()
{
}

const CircleInfo& CColliderCircle::GetInfo() const
{
	return m_tInfo;
}

void CColliderCircle::SetRadius(float fRadius)
{
	m_tInfo.fRadius = fRadius;

	SetWorldScale(fRadius * 2.f, fRadius * 2.f, 0.f);
}

bool CColliderCircle::Init()
{
	if (!CCollider::Init())
		return false;

#ifdef _DEBUG
	m_pMesh = GET_SINGLE(CResourceManager)->FindMesh("Circle2D");
#endif

	return true;
}

void CColliderCircle::Start()
{
	CCollider::Start();
}

void CColliderCircle::Update(float fTime)
{
	CCollider::Update(fTime);

	m_tInfo.vCenter = GetWorldPos();

	m_vMin = m_tInfo.vCenter - m_tInfo.fRadius;
	m_vMax = m_tInfo.vCenter + m_tInfo.fRadius;

	SetWorldScale(m_tInfo.fRadius * 2.f, m_tInfo.fRadius * 2.f, 0.f);
}

void CColliderCircle::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);
}

void CColliderCircle::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderCircle::PreRender(float fTime)
{
	CCollider::PreRender(fTime);
}

void CColliderCircle::Render(float fTime)
{
	CCollider::Render(fTime);
}

void CColliderCircle::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

CColliderCircle* CColliderCircle::Clone()
{
	return new CColliderCircle(*this);
}

void CColliderCircle::Save(FILE* pFile)
{
	CCollider::Save(pFile);
}

void CColliderCircle::Load(FILE* pFile)
{
	CCollider::Load(pFile);
}

bool CColliderCircle::Collision(CCollider* pDest)
{
	switch (pDest->GetColType())
	{
	case COLLIDER_TYPE::CT_RECT:
		return CCollision::CollisionCircleToRect(this, (CColliderRect*)pDest);
	case COLLIDER_TYPE::CT_SPHERE:
		return CCollision::CollisionCircleToCircle(this, (CColliderCircle*)pDest);
	case COLLIDER_TYPE::CT_OBB:
		return CCollision::CollisionOBB2DToCircle((CColliderOBB2D*)pDest, this);
	case COLLIDER_TYPE::CT_PIXEL:
		return CCollision::CollisionPixelToCircle((CColliderPixel*)pDest, this);
	case COLLIDER_TYPE::CT_POINT:
		return CCollision::CollisionCircleToPoint(this, (CColliderPoint*)pDest);
	case COLLIDER_TYPE::CT_LINE:
		return CCollision::CollisionLineToCircle((CColliderLine*)pDest, (CColliderCircle*)this);
	}

	return false;
}
