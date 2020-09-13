#include "SceneComponent.h"

CSceneComponent::CSceneComponent()
{
}

CSceneComponent::CSceneComponent(const CSceneComponent& com)
{
}

CSceneComponent::~CSceneComponent()
{
}

CObj* CSceneComponent::GetObj() const
{
	return nullptr;
}

bool CSceneComponent::Init()
{
	return false;
}

void CSceneComponent::Start()
{
}

void CSceneComponent::Update(float fTime)
{
}

void CSceneComponent::PostUpdate(float fTime)
{
}

void CSceneComponent::Collision(float fTime)
{
}

void CSceneComponent::PreRender(float fTime)
{
}

void CSceneComponent::Render(float fTime)
{
}

void CSceneComponent::PostRender(float fTime)
{
}
