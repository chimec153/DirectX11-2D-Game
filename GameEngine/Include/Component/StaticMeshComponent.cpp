#include "StaticMeshComponent.h"

CStaticMeshComponent::CStaticMeshComponent()
{
}

CStaticMeshComponent::CStaticMeshComponent(const CStaticMeshComponent& com)
{
}

CStaticMeshComponent::~CStaticMeshComponent()
{
}

CObj* CStaticMeshComponent::GetObj() const
{
	return nullptr;
}

bool CStaticMeshComponent::Init()
{
	return false;
}

void CStaticMeshComponent::Start()
{
}

void CStaticMeshComponent::Update(float fTime)
{
}

void CStaticMeshComponent::PostUpdate(float fTime)
{
}

void CStaticMeshComponent::Collision(float fTime)
{
}

void CStaticMeshComponent::PreRender(float fTime)
{
}

void CStaticMeshComponent::Render(float fTime)
{
}

void CStaticMeshComponent::PostRender(float fTime)
{
}
