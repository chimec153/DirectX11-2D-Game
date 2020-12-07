#include "ColliderRect.h"
#include "../Object/Obj.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/ResourceManager.h"
#include "../Collision/Collision.h"
#include "ColliderPixel.h"
#include "ColliderOBB2D.h"
#include "ColliderCircle.h"
#include "ColliderPoint.h"
#include "ColliderLine.h"

CColliderRect::CColliderRect()	:
	m_tInfo()
{
	m_eColType = COLLIDER_TYPE::CT_RECT;
	m_eSceneComponentClassType = SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDERRECT;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_2D;
	m_b2D = true;
}

CColliderRect::CColliderRect(const CColliderRect& col)	:
	CCollider(col)
{
	m_tInfo = col.m_tInfo;
	m_tExtent = col.m_tExtent;
}

CColliderRect::~CColliderRect()
{
}

const RectInfo& CColliderRect::GetInfo() const
{
	return m_tInfo;
}

void CColliderRect::SetExtent(float x, float y)
{
	m_tExtent.x = x;
	m_tExtent.y = y;

	SetWorldScale(x, y, 0.f);
}

bool CColliderRect::Init()
{
	if (!CCollider::Init())
		return false;

#ifdef _DEBUG
	m_pMesh = GET_SINGLE(CResourceManager)->FindMesh("Collider2D");
#endif

	SetInheritRotX(false);
	SetInheritRotY(false);
	SetInheritRotZ(false);

	return true;
}

void CColliderRect::Start()
{
	CCollider::Start();
}

void CColliderRect::Update(float fTime)
{
	CCollider::Update(fTime);
}

void CColliderRect::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);

	Vector3 vPos = GetWorldPos();

	m_tInfo.fL = vPos.x - m_tExtent.x/2.f;
	m_tInfo.fR = vPos.x + m_tExtent.x/2.f;
	m_tInfo.fB = vPos.y - m_tExtent.y/2.f;
	m_tInfo.fT = vPos.y + m_tExtent.y/2.f;

	m_vMin = Vector3(m_tInfo.fL, m_tInfo.fB, 0.f);
	m_vMax = Vector3(m_tInfo.fR, m_tInfo.fT, 0.f);

	SetWorldScale(m_tExtent.x, m_tExtent.y, 0.f);
}

void CColliderRect::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderRect::PreRender(float fTime)
{
	CCollider::PreRender(fTime);
}

void CColliderRect::Render(float fTime)
{
	CCollider::Render(fTime);
}

void CColliderRect::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

CColliderRect* CColliderRect::Clone()
{
	return new CColliderRect(*this);
}

void CColliderRect::Save(FILE* pFile)
{
	CCollider::Save(pFile);
}

void CColliderRect::Load(FILE* pFile)
{
	CCollider::Load(pFile);
}

bool CColliderRect::Collision(CCollider* pDest)
{
	switch (pDest->GetColType())
	{
	case COLLIDER_TYPE::CT_RECT:
		return CCollision::CollisionRectToRect(this, (CColliderRect*)pDest);
	case COLLIDER_TYPE::CT_SPHERE:
		return CCollision::CollisionCircleToRect((CColliderCircle*)pDest, this);
	case COLLIDER_TYPE::CT_OBB:
		return CCollision::CollisionOBB2DToRect((CColliderOBB2D*)pDest, this);
	case COLLIDER_TYPE::CT_PIXEL:
		return CCollision::CollisionPixelToRect((CColliderPixel*)pDest, this);
	case COLLIDER_TYPE::CT_POINT:
		return CCollision::CollisionPointToRect((CColliderPoint*)pDest, this);
	case COLLIDER_TYPE::CT_LINE:
		return CCollision::CollisionLineToRect((CColliderLine*)pDest, this);
	}

	return false;
}
