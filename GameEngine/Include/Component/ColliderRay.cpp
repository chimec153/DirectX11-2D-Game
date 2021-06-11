#include "ColliderRay.h"
#include "../Collision/Collision.h"
#include "ColliderCircle.h"
#include "../Camera/CameraManager.h"
#include "Camera.h"
#include "../Device.h"
#include "ColliderOBB.h"
#include "ColliderTerrain.h"

CColliderRay::CColliderRay()
{
	m_eColType = COLLIDER_TYPE::RAY;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_3D;
	m_eSceneComponentClassType = SCENECOMPONENT_CLASS_TYPE::COLLIDER_RAY;
}

CColliderRay::CColliderRay(const CColliderRay& ray)	:
	CCollider(ray)
	, m_tInfo(ray.m_tInfo)
{
}

CColliderRay::~CColliderRay()
{
}

const RAYINFO& CColliderRay::GetInfo() const
{
	return m_tInfo;
}

const RAYINFO& CColliderRay::GetUIInfo() const
{
	return m_tUIInfo;
}

void CColliderRay::SetDir(const Vector3& vDir)
{
	m_tInfo.vDir = vDir;
	m_tUIInfo.vDir = vDir;
}

void CColliderRay::SetOrigin(const Vector3& vPos)
{
	m_tInfo.vOrigin = vPos;
	m_tUIInfo.vOrigin = vPos;
}

bool CColliderRay::Init()
{
	if(!CCollider::Init())
		return false;

	return true;
}

void CColliderRay::Start()
{
	CCollider::Start();
}

void CColliderRay::Update(float fTime)
{
	CCollider::Update(fTime);
}

void CColliderRay::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);
}

void CColliderRay::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderRay::PreRender(float fTime)
{
	CCollider::PreRender(fTime);
}

void CColliderRay::Render(float fTime)
{
	CCollider::Render(fTime);
}

void CColliderRay::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

CColliderRay* CColliderRay::Clone()
{
	return new CColliderRay(*this);
}

void CColliderRay::Save(FILE* pFile)
{
	CCollider::Save(pFile);

	fwrite(&m_tInfo, sizeof(RAYINFO), 1, pFile);
}

void CColliderRay::Load(FILE * pFile)
{
	CCollider::Load(pFile);

	fread(&m_tInfo, sizeof(RAYINFO), 1, pFile);
}

bool CColliderRay::Collision(CCollider* pDest)
{
	switch (pDest->GetColType())
	{
	case COLLIDER_TYPE::CT_RECT:
		break;
	case COLLIDER_TYPE::CT_SPHERE:
		return CCollision::CollisionRayToSphere(this, static_cast<CColliderCircle*>(pDest));
	case COLLIDER_TYPE::CT_OBB:
		break;
	case COLLIDER_TYPE::CT_PIXEL:
		break;
	case COLLIDER_TYPE::CT_POINT:
		break;
	case COLLIDER_TYPE::CT_LINE:
		break;
	case COLLIDER_TYPE::RAY:
		return CCollision::CollisionRayToRay(this,static_cast<CColliderRay*>(pDest));
	case COLLIDER_TYPE::OBB3D:
		return CCollision::CollisionRayToOBB(this, static_cast<CColliderOBB*>(pDest));
	case COLLIDER_TYPE::TERRAIN:
		return CCollision::CollisionTerrainToRay(static_cast<CColliderTerrain*>(pDest), this);
	}

	return false;
}
