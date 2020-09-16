#include "Mesh2DComponent.h"
#include "../Resource/ResourceManager.h"

CMesh2DComponent::CMesh2DComponent()
{
}

CMesh2DComponent::CMesh2DComponent(const CMesh2DComponent& com)	:
	CMeshComponent(com)
{
}

CMesh2DComponent::~CMesh2DComponent()
{
}

bool CMesh2DComponent::Init()
{
	if (!CMeshComponent::Init())
		return false;

	m_pMesh = (CMesh*)GET_SINGLE(CResourceManager)->GetDefaultMesh();

	return true;
}

void CMesh2DComponent::Start()
{
	CMeshComponent::Start();
}

void CMesh2DComponent::Update(float fTime)
{
	CMeshComponent::Update(fTime);
}

void CMesh2DComponent::PostUpdate(float fTime)
{
	CMeshComponent::PostUpdate(fTime);
}

void CMesh2DComponent::Collision(float fTime)
{
	CMeshComponent::Collision(fTime);
}

void CMesh2DComponent::PreRender(float fTime)
{
	CMeshComponent::PreRender(fTime);
}

void CMesh2DComponent::Render(float fTime)
{
	CMeshComponent::Render(fTime);
}

void CMesh2DComponent::PostRender(float fTime)
{
	CMeshComponent::PostRender(fTime);
}
