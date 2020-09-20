#include "MeshComponent.h"
#include "../Resource/Material.h"

CMeshComponent::CMeshComponent()	:
	m_pMaterial(nullptr)
{
}

CMeshComponent::CMeshComponent(const CMeshComponent& com)	:
	CSceneComponent(com)
{
	m_pMaterial = com.m_pMaterial->Clone();
}

CMeshComponent::~CMeshComponent()
{
	SAFE_RELEASE(m_pMaterial);
}


CMaterial* CMeshComponent::GetMaterial() const
{
	if (m_pMaterial)
		m_pMaterial->AddRef();

	return m_pMaterial;
}

void CMeshComponent::SetMaterial(CMaterial* pMaterial)
{
	SAFE_RELEASE(m_pMaterial);

	m_pMaterial = pMaterial;

	if (m_pMaterial)
		m_pMaterial->AddRef();
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

	if (m_pMaterial)
		m_pMaterial->SetMaterial();
}

void CMeshComponent::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

CMeshComponent* CMeshComponent::Clone()
{
	return new CMeshComponent(*this);
}
