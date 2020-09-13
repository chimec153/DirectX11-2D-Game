#include "MeshComponent.h"

CMeshComponent::CMeshComponent()
{
}

CMeshComponent::CMeshComponent(const CMeshComponent& com)
{
}

CMeshComponent::~CMeshComponent()
{
}

CObj* CMeshComponent::GetObj() const
{
	return nullptr;
}

bool CMeshComponent::Init()
{
	return false;
}

void CMeshComponent::Start()
{
}

void CMeshComponent::Update(float fTime)
{
}

void CMeshComponent::PostUpdate(float fTime)
{
}

void CMeshComponent::Collision(float fTime)
{
}

void CMeshComponent::PreRender(float fTime)
{
}

void CMeshComponent::Render(float fTime)
{
}

void CMeshComponent::PostRender(float fTime)
{
}
