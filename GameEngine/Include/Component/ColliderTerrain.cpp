#include "ColliderTerrain.h"
#ifdef _DEBUG
#include "../Resource/ResourceManager.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/Material.h"
#include "../Resource/Shader.h"
#endif
#include "../Collision/Collision.h"
#include "ColliderRay.h"

CColliderTerrain::CColliderTerrain()
{
	m_eSceneComponentClassType = SCENECOMPONENT_CLASS_TYPE::COLLIDER_TERRAIN;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_3D;
	m_eColType = COLLIDER_TYPE::TERRAIN;
}

CColliderTerrain::CColliderTerrain(const CColliderTerrain& col)	:
	CCollider(col)
	, m_tInfo(col.m_tInfo)
{	
}

CColliderTerrain::~CColliderTerrain()
{
}

void CColliderTerrain::SetPos(const std::vector<Vector3>& vecPos)
{
	m_tInfo.vecPos = vecPos;
}

void CColliderTerrain::SetInfo(int iWidth, int iHeight)
{
	m_tInfo.iWidth = iWidth;
	m_tInfo.iHeight = iHeight;
}

const TERRAININFO& CColliderTerrain::GetInfo() const
{
	return m_tInfo;
}
#ifdef _DEBUG
void CColliderTerrain::SetDebugMesh(const std::string& strMesh)
{
	m_pDebugMesh = static_cast<CMesh2D*>(GET_SINGLE(CResourceManager)->FindMesh(strMesh));
}
CMesh* CColliderTerrain::GetDebugMesh() const
{
	if (m_pDebugMesh)
	{
		m_pDebugMesh->AddRef();
	}

	return m_pDebugMesh;
}
#endif

bool CColliderTerrain::Init()
{
	if(!CCollider::Init())
		return false;

#ifdef _DEBUG
	SAFE_RELEASE(m_pDebugShader);
	m_pDebugShader = GET_SINGLE(CShaderManager)->FindShader("Single3DShader");
#endif

	return true;
}

void CColliderTerrain::Start()
{
	CCollider::Start();
}

void CColliderTerrain::Update(float fTime)
{
	CCollider::Update(fTime);

	m_tInfo.vCenter = GetWorldPos();
	m_tInfo.vScale = GetWorldScale();
}

void CColliderTerrain::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);
}

void CColliderTerrain::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderTerrain::PreRender(float fTime)
{
	CCollider::PreRender(fTime);
}

void CColliderTerrain::Render(float fTime)
{
	CCollider::Render(fTime);
}

void CColliderTerrain::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

CColliderTerrain* CColliderTerrain::Clone()
{
	return new CColliderTerrain(*this);
}

void CColliderTerrain::Save(FILE* pFile)
{
	CCollider::Save(pFile);
}

void CColliderTerrain::Load(FILE* pFile)
{
	CCollider::Load(pFile);
}

bool CColliderTerrain::Collision(CCollider* pDest)
{
	switch (pDest->GetColType())
	{
	case COLLIDER_TYPE::CT_RECT:
		break;
	case COLLIDER_TYPE::CT_SPHERE:
		break;
	case COLLIDER_TYPE::CT_OBB:
		break;
	case COLLIDER_TYPE::CT_PIXEL:
		break;
	case COLLIDER_TYPE::CT_POINT:
		break;
	case COLLIDER_TYPE::CT_LINE:
		break;
	case COLLIDER_TYPE::RAY:
		return CCollision::CollisionTerrainToRay(this, static_cast<CColliderRay*>(pDest));
	case COLLIDER_TYPE::OBB3D:
		break;
	case COLLIDER_TYPE::TERRAIN:
		break;
	}
	return false;
}

bool CColliderTerrain::MouseCollision(const Vector2& vPos)
{
	return false;
}
