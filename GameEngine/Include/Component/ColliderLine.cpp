#include "ColliderLine.h"
#include "../Resource/ResourceManager.h"
#include "../Object/Obj.h"
#include "../Collision/Collision.h"
#include "ColliderCircle.h"
#include "ColliderOBB2D.h"
#include "ColliderRect.h"
#include "ColliderPixel.h"
#include "ColliderPoint.h"

CColliderLine::CColliderLine()	:
	m_tInfo()
{
	m_eColType = COLLIDER_TYPE::CT_LINE;
	m_eSceneComponentClassType = SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDERLINE;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_2D;

	m_b2D = true;
}

CColliderLine::CColliderLine(const CColliderLine& col)	:
	CCollider(col)
{
	m_tInfo = col.m_tInfo;
}

CColliderLine::~CColliderLine()
{
}

const LineInfo& CColliderLine::GetInfo() const
{
	return m_tInfo;
}

void CColliderLine::SetInfo(const Vector2& vStart, const Vector2& vEnd)
{
	m_tInfo.vStart = vStart;
	m_tInfo.vEnd = vEnd;

	SetWorldScale(vEnd.y - vStart.y, vEnd.y - vStart.y, 1.f);
}

bool CColliderLine::Init()
{
	if (!CCollider::Init())
		return false;

#ifdef _DEBUG
	m_pDebugMesh = (CMesh2D*)GET_SINGLE(CResourceManager)->FindMesh("Collider2DLine");
#endif
	SetInheritScale(false);

	return true;
}

void CColliderLine::Start()
{
	CCollider::Start();
}

void CColliderLine::Update(float fTime)
{
	CCollider::Update(fTime);
}

void CColliderLine::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);

	Vector3 vPos = GetWorldPos();
	Vector3 vAxis = GetWorldAxis(WORLD_AXIS::AXIS_Y);
	Vector3 vScale = GetWorldScale();

	Vector2 vSize = Vector2(vAxis.x, vAxis.y) * Vector2(vScale.x, vScale.y);

	m_tInfo.vStart = Vector2(vPos.x, vPos.y) - vSize / 2.f;
	m_tInfo.vEnd = m_tInfo.vStart + vSize;
}

void CColliderLine::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderLine::PreRender(float fTime)
{
	CCollider::PreRender(fTime);
}

void CColliderLine::Render(float fTime)
{
	CCollider::Render(fTime);
}

void CColliderLine::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

CColliderLine* CColliderLine::Clone()
{
	return new CColliderLine(*this);
}

void CColliderLine::Save(FILE* pFile)
{
	CCollider::Save(pFile);

	fwrite(&m_tInfo, sizeof(m_tInfo), 1, pFile);
}

void CColliderLine::Load(FILE* pFile)
{
	CCollider::Load(pFile);

	fread(&m_tInfo, sizeof(m_tInfo), 1, pFile);

#ifdef _DEBUG
	m_pDebugMesh = (CMesh2D*)GET_SINGLE(CResourceManager)->FindMesh("Collider2DLine");
#endif
}

bool CColliderLine::Collision(CCollider* pDest)
{
	switch (pDest->GetColType())
	{
	case COLLIDER_TYPE::CT_RECT:
		return CCollision::CollisionLineToRect(this, (CColliderRect*)pDest);
	case COLLIDER_TYPE::CT_SPHERE:
		return CCollision::CollisionLineToCircle(this, (CColliderCircle*)pDest);
	case COLLIDER_TYPE::CT_OBB:
		return CCollision::CollisionLineToOBB2D(this, (CColliderOBB2D*)pDest);
	case COLLIDER_TYPE::CT_PIXEL:
		return CCollision::CollisionLineToPixel(this, (CColliderPixel*)pDest);
	case COLLIDER_TYPE::CT_POINT:
		return CCollision::CollisionLineToPoint(this, (CColliderPoint*)pDest);
	case COLLIDER_TYPE::CT_LINE:
		return CCollision::CollisionLineToLine(this, (CColliderLine*)pDest);
	}

	return false;
}
