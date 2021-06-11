#include "ColliderOBB.h"
#ifdef _DEBUG
#include "../Resource/ResourceManager.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/Material.h"
#include "../Resource/Shader.h"
#endif
#include "../Collision/Collision.h"
#include "ColliderRay.h"
#include "ColliderCircle.h"

CColliderOBB::CColliderOBB()	:
	m_tInfo()
{
	m_eColType = COLLIDER_TYPE::OBB3D;
	m_eSceneComponentClassType = SCENECOMPONENT_CLASS_TYPE::COLLIDER_OBB;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_3D;
}

CColliderOBB::CColliderOBB(const CColliderOBB& obb)	:
	CCollider(obb)
	, m_tInfo(obb.m_tInfo)
{
}

CColliderOBB::~CColliderOBB()
{
}

void CColliderOBB::SetExtent(float x, float y, float z)
{
	m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_X)] = x;
	m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_Y)] = y;
	m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_Z)] = z;
}

void CColliderOBB::SetExtent(const Vector3& vExtent)
{
	memcpy_s(&m_tInfo.fLength[0], 12, &vExtent.x, 12);
}

const OBBInfo& CColliderOBB::GetInfo() const
{
	return m_tInfo;
}

bool CColliderOBB::Init()
{
	if(!CCollider::Init())
		return false;

#ifdef _DEBUG
	m_pDebugMesh = static_cast<CMesh2D*>(GET_SINGLE(CResourceManager)->FindMesh("Cube"));
	SAFE_RELEASE(m_pDebugShader);
	m_pDebugShader = GET_SINGLE(CShaderManager)->FindShader("Single3DShader");
#endif

	return true;
}

void CColliderOBB::Start()
{
	CCollider::Start();
}

void CColliderOBB::Update(float fTime)
{
	CCollider::Update(fTime);

	m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_X)] = GetWorldAxis(WORLD_AXIS::AXIS_X);
	m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_Y)] = GetWorldAxis(WORLD_AXIS::AXIS_Y);
	m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_Z)] = GetWorldAxis(WORLD_AXIS::AXIS_Z);

	m_tInfo.vPos = GetWorldPos();

	m_vMin.x = FLT_MAX;
	m_vMin.y = FLT_MAX;
	m_vMin.z = FLT_MAX;
	m_vMax.x = -FLT_MAX;
	m_vMax.y = -FLT_MAX;
	m_vMax.z = -FLT_MAX;

	Vector3 vLTN = m_tInfo.vPos
		- m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_X)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_X)]
		+ m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_Y)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_Y)]
		- m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_Z)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_Z)];
	Vector3 vLTF = m_tInfo.vPos
		- m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_X)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_X)]
		+ m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_Y)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_Y)]
		+ m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_Z)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_Z)];
	Vector3 vRTN = m_tInfo.vPos
		+ m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_X)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_X)]
		+ m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_Y)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_Y)]
		- m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_Z)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_Z)];
	Vector3 vRTF = m_tInfo.vPos
		+ m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_X)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_X)]
		+ m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_Y)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_Y)]
		+ m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_Z)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_Z)];
	Vector3 vLBN = m_tInfo.vPos
		- m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_X)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_X)]
		- m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_Y)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_Y)]
		- m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_Z)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_Z)];
	Vector3 vLBF = m_tInfo.vPos
		- m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_X)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_X)]
		- m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_Y)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_Y)]
		+ m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_Z)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_Z)];
	Vector3 vRBN = m_tInfo.vPos
		+ m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_X)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_X)]
		- m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_Y)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_Y)]
		- m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_Z)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_Z)];
	Vector3 vRBF = m_tInfo.vPos
		+ m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_X)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_X)]
		- m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_Y)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_Y)]
		+ m_tInfo.vAxis[static_cast<int>(WORLD_AXIS::AXIS_Z)] * m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_Z)];

	if (m_vMin.x > vLTN.x)
		m_vMin.x = vLTN.x;
	if (m_vMin.y > vLTN.y)
		m_vMin.y = vLTN.y;
	if (m_vMin.z > vLTN.z)
		m_vMin.z = vLTN.z;
	if (m_vMax.x < vLTN.x)
		m_vMax.x = vLTN.x;
	if (m_vMax.y < vLTN.y)
		m_vMax.y = vLTN.y;
	if (m_vMax.z < vLTN.z)
		m_vMax.z = vLTN.z;

	if (m_vMin.x > vLTF.x)
		m_vMin.x = vLTF.x;
	if (m_vMin.y > vLTF.y)
		m_vMin.y = vLTF.y;
	if (m_vMin.z > vLTF.z)
		m_vMin.z = vLTF.z;
	if (m_vMax.x < vLTF.x)
		m_vMax.x = vLTF.x;
	if (m_vMax.y < vLTF.y)
		m_vMax.y = vLTF.y;
	if (m_vMax.z < vLTF.z)
		m_vMax.z = vLTF.z;

	if (m_vMin.x > vRTN.x)
		m_vMin.x = vRTN.x;
	if (m_vMin.y > vRTN.y)
		m_vMin.y = vRTN.y;
	if (m_vMin.z > vRTN.z)
		m_vMin.z = vRTN.z;
	if (m_vMax.x < vRTN.x)
		m_vMax.x = vRTN.x;
	if (m_vMax.y < vRTN.y)
		m_vMax.y = vRTN.y;
	if (m_vMax.z < vRTN.z)
		m_vMax.z = vRTN.z;

	if (m_vMin.x > vRTF.x)
		m_vMin.x = vRTF.x;
	if (m_vMin.y > vRTF.y)
		m_vMin.y = vRTF.y;
	if (m_vMin.z > vRTF.z)
		m_vMin.z = vRTF.z;
	if (m_vMax.x < vRTF.x)
		m_vMax.x = vRTF.x;
	if (m_vMax.y < vRTF.y)
		m_vMax.y = vRTF.y;
	if (m_vMax.z < vRTF.z)
		m_vMax.z = vRTF.z;

	if (m_vMin.x > vLBN.x)
		m_vMin.x = vLBN.x;
	if (m_vMin.y > vLBN.y)
		m_vMin.y = vLBN.y;
	if (m_vMin.z > vLBN.z)
		m_vMin.z = vLBN.z;
	if (m_vMax.x < vLBN.x)
		m_vMax.x = vLBN.x;
	if (m_vMax.y < vLBN.y)
		m_vMax.y = vLBN.y;
	if (m_vMax.z < vLBN.z)
		m_vMax.z = vLBN.z;
					 
	if (m_vMin.x > vLBF.x)
		m_vMin.x = vLBF.x;
	if (m_vMin.y > vLBF.y)
		m_vMin.y = vLBF.y;
	if (m_vMin.z > vLBF.z)
		m_vMin.z = vLBF.z;
	if (m_vMax.x < vLBF.x)
		m_vMax.x = vLBF.x;
	if (m_vMax.y < vLBF.y)
		m_vMax.y = vLBF.y;
	if (m_vMax.z < vLBF.z)
		m_vMax.z = vLBF.z;
					 
	if (m_vMin.x > vRBN.x)
		m_vMin.x = vRBN.x;
	if (m_vMin.y > vRBN.y)
		m_vMin.y = vRBN.y;
	if (m_vMin.z > vRBN.z)
		m_vMin.z = vRBN.z;
	if (m_vMax.x < vRBN.x)
		m_vMax.x = vRBN.x;
	if (m_vMax.y < vRBN.y)
		m_vMax.y = vRBN.y;
	if (m_vMax.z < vRBN.z)
		m_vMax.z = vRBN.z;
					 
	if (m_vMin.x > vRBF.x)
		m_vMin.x = vRBF.x;
	if (m_vMin.y > vRBF.y)
		m_vMin.y = vRBF.y;
	if (m_vMin.z > vRBF.z)
		m_vMin.z = vRBF.z;
	if (m_vMax.x < vRBF.x)
		m_vMax.x = vRBF.x;
	if (m_vMax.y < vRBF.y)
		m_vMax.y = vRBF.y;
	if (m_vMax.z < vRBF.z)
		m_vMax.z = vRBF.z;

	SetWorldScale(m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_X)] * 2.f, m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_Y)] * 2.f, m_tInfo.fLength[static_cast<int>(WORLD_AXIS::AXIS_Z)] * 2.f);
}

void CColliderOBB::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);
}

void CColliderOBB::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderOBB::PreRender(float fTime)
{
	CCollider::PreRender(fTime);
}

void CColliderOBB::Render(float fTime)
{
	CCollider::Render(fTime);
}

void CColliderOBB::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

CColliderOBB* CColliderOBB::Clone()
{
	return new CColliderOBB(*this);
}

void CColliderOBB::Save(FILE* pFile)
{
	CCollider::Save(pFile);
}

void CColliderOBB::Load(FILE* pFile)
{
	CCollider::Load(pFile);
}

bool CColliderOBB::Collision(CCollider* pDest)
{
	switch (pDest->GetColType())
	{
	case COLLIDER_TYPE::CT_RECT:
		break;
	case COLLIDER_TYPE::CT_SPHERE:
		return CCollision::CollisionOBBToSphere(this, static_cast<CColliderCircle*>(pDest));
	case COLLIDER_TYPE::CT_OBB:
		break;
	case COLLIDER_TYPE::CT_PIXEL:
		break;
	case COLLIDER_TYPE::CT_POINT:
		break;
	case COLLIDER_TYPE::CT_LINE:
		break;
	case COLLIDER_TYPE::RAY:
		return CCollision::CollisionRayToOBB(static_cast<CColliderRay*>(pDest), this);
	case COLLIDER_TYPE::OBB3D:
		return CCollision::CollisionOBBToOBB(this, static_cast<CColliderOBB*>(pDest));
	}

	return false;
}

bool CColliderOBB::MouseCollision(const Vector2& vPos)
{
	return false;
}
