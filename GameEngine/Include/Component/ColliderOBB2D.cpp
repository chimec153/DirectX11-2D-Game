#include "ColliderOBB2D.h"
#include "../Resource/ResourceManager.h"
#include "../Collision/Collision.h"
#include "ColliderCircle.h"
#include "ColliderRect.h"
#include "ColliderPixel.h"
#include "ColliderLine.h"
#include "ColliderPoint.h"

CColliderOBB2D::CColliderOBB2D()
{
	m_eColType = COLLIDER_TYPE::CT_OBB;
	m_eSceneComponentClassType = SCENECOMPONENT_CLASS_TYPE::COLLIDEROBB2D;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_2D;

	m_b2D = true;
}

CColliderOBB2D::CColliderOBB2D(const CColliderOBB2D& col)	:
	CCollider(col)
{
	m_tInfo = col.m_tInfo;
}

CColliderOBB2D::~CColliderOBB2D()
{
}

const OBBInfo& CColliderOBB2D::GetInfo() const
{
	return m_tInfo;
}

void CColliderOBB2D::SetExtend(float x, float y, float z)
{
	m_tInfo.fLength[(int)WORLD_AXIS::AXIS_X] = x;
	m_tInfo.fLength[(int)WORLD_AXIS::AXIS_Y] = y;
	m_tInfo.fLength[(int)WORLD_AXIS::AXIS_Z] = z;

	SetWorldScale(x * 2.f, y * 2.f, z * 2.f);
}

bool CColliderOBB2D::Init()
{
	if (!CCollider::Init())
		return false;

#ifdef _DEBUG
	m_pMesh =GET_SINGLE(CResourceManager)->FindMesh("Collider2D");
#endif

	return true;
}

void CColliderOBB2D::Start()
{
	CCollider::Start();
}

void CColliderOBB2D::Update(float fTime)
{
	CCollider::Update(fTime);
}

void CColliderOBB2D::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);

	Vector3 vPos = GetWorldPos();
	m_tInfo.vPos = Vector3(vPos.x, vPos.y, 0.f);

	SetWorldScale(m_tInfo.fLength[(int)WORLD_AXIS::AXIS_X] * 2.f, m_tInfo.fLength[(int)WORLD_AXIS::AXIS_Y] * 2.f, m_tInfo.fLength[(int)WORLD_AXIS::AXIS_Z] * 2.f);

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
		m_tInfo.vAxis[i] = GetWorldAxis((WORLD_AXIS)i);

	Vector3 vPt[2] =
	{
		m_tInfo.vAxis[(int)WORLD_AXIS::AXIS_X] * m_tInfo.fLength[(int)WORLD_AXIS::AXIS_X] 
		+ m_tInfo.vAxis[(int)WORLD_AXIS::AXIS_Y] * m_tInfo.fLength[(int)WORLD_AXIS::AXIS_Y],
		m_tInfo.vAxis[(int)WORLD_AXIS::AXIS_X] * m_tInfo.fLength[(int)WORLD_AXIS::AXIS_X] 
		- m_tInfo.vAxis[(int)WORLD_AXIS::AXIS_Y] * m_tInfo.fLength[(int)WORLD_AXIS::AXIS_Y],
	};

	if (vPt[0].x < 0.f)
		vPt[0].x *= -1.f;

	if (vPt[0].y < 0.f)
		vPt[0].y *= -1.f;

	if (vPt[1].x < 0.f)
		vPt[1].x *= -1.f;

	if (vPt[1].y < 0.f)
		vPt[1].y *= -1.f;

	m_vMax.x = vPt[0].x > vPt[1].x ? vPt[0].x : vPt[1].x;
	m_vMax.y = vPt[0].y > vPt[1].y ? vPt[0].y : vPt[1].y;

	m_vMin = -m_vMax;

	m_vMax += vPos;
	m_vMin += vPos;

	m_vMax.z = 0.f;
	m_vMin.z = 0.f;
}

void CColliderOBB2D::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderOBB2D::PreRender(float fTime)
{
	CCollider::PreRender(fTime);
}

void CColliderOBB2D::Render(float fTime)
{
	CCollider::Render(fTime);
}

void CColliderOBB2D::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

CColliderOBB2D* CColliderOBB2D::Clone()
{
	return new CColliderOBB2D(*this);
}

void CColliderOBB2D::Save(FILE* pFile)
{
	CCollider::Save(pFile);
}

void CColliderOBB2D::Load(FILE* pFile)
{
	CCollider::Load(pFile);
}

bool CColliderOBB2D::Collision(CCollider* pDest)
{
	switch (pDest->GetColType())
	{
	case COLLIDER_TYPE::CT_RECT:
		return CCollision::CollisionOBB2DToRect(this, (CColliderRect*)pDest);
	case COLLIDER_TYPE::CT_SPHERE:
		return CCollision::CollisionOBB2DToCircle(this, (CColliderCircle*)pDest);
	case COLLIDER_TYPE::CT_OBB:
		return CCollision::CollisionOBB2DToOBB2D(this, (CColliderOBB2D*)pDest);
	case COLLIDER_TYPE::CT_PIXEL:
		return CCollision::CollisionPixelToOBB2D((CColliderPixel*)pDest, this);
	case COLLIDER_TYPE::CT_POINT:
		return CCollision::CollisionOBB2DToPoint(this, (CColliderPoint*)pDest);
	case COLLIDER_TYPE::CT_LINE:
		return CCollision::CollisionLineToOBB2D((CColliderLine*)pDest, this);
	}

	return false;
}
