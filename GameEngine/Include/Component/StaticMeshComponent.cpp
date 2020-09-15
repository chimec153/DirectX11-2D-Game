#include "StaticMeshComponent.h"

CStaticMeshComponent::CStaticMeshComponent()
{
}

CStaticMeshComponent::CStaticMeshComponent(const CStaticMeshComponent& com)	:
	CMeshComponent(com)
{
}

CStaticMeshComponent::~CStaticMeshComponent()
{
}

bool CStaticMeshComponent::Init()
{
	if (!CMeshComponent::Init())
		return false;

	return true;
}

void CStaticMeshComponent::Start()
{
	CMeshComponent::Start();
}

void CStaticMeshComponent::Update(float fTime)
{
	CMeshComponent::Update(fTime);
}

void CStaticMeshComponent::PostUpdate(float fTime)
{
	CMeshComponent::PostUpdate(fTime);
}

void CStaticMeshComponent::Collision(float fTime)
{
	CMeshComponent::Collision(fTime);
}

void CStaticMeshComponent::PreRender(float fTime)
{
	CMeshComponent::PreRender(fTime);
}

void CStaticMeshComponent::Render(float fTime)
{
	CMeshComponent::Render(fTime);
}

void CStaticMeshComponent::PostRender(float fTime)
{
	CMeshComponent::PostRender(fTime);
}
