#include "ColliderPoint.h"
#include "Camera.h"
#include "../Camera/CameraManager.h"
#include "../Scene/Scene.h"
#include "ColliderCircle.h"
#include "ColliderOBB2D.h"
#include "ColliderPixel.h"
#include "ColliderRect.h"
#include "ColliderLine.h"
#include "../Collision/Collision.h"
#include "../Resource/ResourceManager.h"
#include "../Device.h"
#include "../Input.h"

CColliderPoint::CColliderPoint()	:
	m_vPt(),
	m_vUIPt()
{
	m_eColType = COLLIDER_TYPE::CT_POINT;
	
}

CColliderPoint::CColliderPoint(const CColliderPoint& col)	:
	CCollider(col)
{
	m_eColType = col.m_eColType;
}

CColliderPoint::~CColliderPoint()
{
}

const Vector3& CColliderPoint::GetInfo() const
{
	return m_vPt;
}

const Vector3& CColliderPoint::GetUIInfo() const
{
	return m_vUIPt;
}

bool CColliderPoint::Init()
{
	if (!CCollider::Init())
		return false;

#ifdef _DEBUG
	m_pDebugMesh = (CMesh2D*)GET_SINGLE(CResourceManager)->FindMesh("Collider2D");
#endif

	return true;
}

void CColliderPoint::Start()
{
	CCollider::Start();
}

void CColliderPoint::Update(float fTime)
{
	CCollider::Update(fTime);

	m_vUIPt = GetWorldPos();
	m_vUIPt.z = 0.f;

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	Resolution tRS = RESOLUTION;

	m_vPt = m_vUIPt + pCam->GetWorldPos() - Vector3((float)tRS.iWidth, (float)tRS.iHeight, 0.f) * pCam->GetPivot();
	m_vPt.z = 0.f;

	SAFE_RELEASE(pCam);

#ifdef _DEBUG
/*
	char strPos[128] = {};

	sprintf_s(strPos, "UIPos x: %.1f, y: %.1f, WorldPos x: %.1f, y: %.1f\n", m_vUIPt.x, m_vUIPt.y, m_vPt.x, m_vPt.y);

	OutputDebugStringA(strPos);*/

#endif

	SetWorldScale(1.f, 1.f, 1.f);

	m_vMin = m_vPt;
	m_vMax = m_vPt;
}

void CColliderPoint::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);
}

void CColliderPoint::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderPoint::PreRender(float fTime)
{
	CCollider::PreRender(fTime);
}

void CColliderPoint::Render(float fTime)
{
	CCollider::Render(fTime);
}

void CColliderPoint::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

CColliderPoint* CColliderPoint::Clone()
{
	return new CColliderPoint(*this);
}

void CColliderPoint::Save(FILE* pFile)
{
	CCollider::Save(pFile);

	fwrite(&m_vPt, sizeof(Vector3), 1, pFile);
	fwrite(&m_vUIPt, sizeof(Vector3), 1, pFile);
}

void CColliderPoint::Load(FILE* pFile)
{
	CCollider::Load(pFile);

	fread(&m_vPt, sizeof(Vector3), 1, pFile);
	fread(&m_vUIPt, sizeof(Vector3), 1, pFile);

#ifdef _DEBUG
	m_pDebugMesh = (CMesh2D*)GET_SINGLE(CResourceManager)->FindMesh("Collider2D");
#endif
}

bool CColliderPoint::Collision(CCollider* pCol)
{
	switch (pCol->GetColType())
	{
	case COLLIDER_TYPE::CT_RECT:
		return CCollision::CollisionPointToRect(this, (CColliderRect*)pCol);
	case COLLIDER_TYPE::CT_SPHERE:
		return CCollision::CollisionCircleToPoint((CColliderCircle*)pCol, this);
	case COLLIDER_TYPE::CT_OBB:
		return CCollision::CollisionOBB2DToPoint((CColliderOBB2D*)pCol, this);
	case COLLIDER_TYPE::CT_PIXEL:
		return CCollision::CollisionPointToPixel(this, (CColliderPixel*)pCol);
	case COLLIDER_TYPE::CT_POINT:
		return CCollision::CollisionPointToPoint(this, (CColliderPoint*)pCol);
	case COLLIDER_TYPE::CT_LINE:
		return CCollision::CollisionLineToPoint((CColliderLine*)pCol, this);
	}

	return false;
}
