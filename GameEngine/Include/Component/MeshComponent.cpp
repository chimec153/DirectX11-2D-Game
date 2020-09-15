#include "MeshComponent.h"
#include "../Resource/Mesh.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CMeshComponent::CMeshComponent() :
	m_pMesh(nullptr)
{
}

CMeshComponent::CMeshComponent(const CMeshComponent& com)	:
	CSceneComponent(com)
{
	m_pMesh = com.m_pMesh;

	if (m_pMesh)
		m_pMesh->AddRef();
}

CMeshComponent::~CMeshComponent()
{
	SAFE_RELEASE(m_pMesh);
}

CMesh* CMeshComponent::GetMesh() const
{
	if (m_pMesh)
		m_pMesh->AddRef();

	return m_pMesh;
}

void CMeshComponent::SetMesh(const std::string& strName)
{
	SAFE_RELEASE(m_pMesh);

	m_pMesh = m_pScene->GetResource()->FindMesh(strName);
}

void CMeshComponent::SetMesh(CMesh* pMesh)
{
	SAFE_RELEASE(m_pMesh);

	m_pMesh = pMesh;

	if (m_pMesh)
		m_pMesh->AddRef();
}

bool CMeshComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CMeshComponent::Start()
{
	CSceneComponent::Start();
}

void CMeshComponent::Update(float fTime)
{
	CSceneComponent::Update(fTime);
}

void CMeshComponent::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CMeshComponent::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CMeshComponent::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CMeshComponent::Render(float fTime)
{
	CSceneComponent::Render(fTime);
}

void CMeshComponent::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}
