#include "StaticMeshComponent.h"

CStaticMeshComponent::CStaticMeshComponent()
{
	m_eSceneComponentClassType = SCENECOMPONENT_CLASS_TYPE::SCT_STATICMESH;
}

CStaticMeshComponent::CStaticMeshComponent(const CStaticMeshComponent& com)	:
	CSceneComponent(com)
{
}

CStaticMeshComponent::~CStaticMeshComponent()
{
}

bool CStaticMeshComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CStaticMeshComponent::Start()
{
	CSceneComponent::Start();
}

void CStaticMeshComponent::Update(float fTime)
{
	CSceneComponent::Update(fTime);
}

void CStaticMeshComponent::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CStaticMeshComponent::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CStaticMeshComponent::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CStaticMeshComponent::Render(float fTime)
{
	CSceneComponent::Render(fTime);
}

void CStaticMeshComponent::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

CStaticMeshComponent* CStaticMeshComponent::Clone()
{
	return new CStaticMeshComponent(*this);
}

void CStaticMeshComponent::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);
}

void CStaticMeshComponent::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);
}
