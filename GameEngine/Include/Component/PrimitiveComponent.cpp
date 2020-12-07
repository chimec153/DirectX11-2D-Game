#include "PrimitiveComponent.h"
#include "../RenderManager.h"

CPrimitiveComponent::CPrimitiveComponent()
{
}

CPrimitiveComponent::CPrimitiveComponent(const CPrimitiveComponent& com)	:
	CSceneComponent(com)
{
}

CPrimitiveComponent::~CPrimitiveComponent()
{
}

bool CPrimitiveComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CPrimitiveComponent::Start()
{
	CSceneComponent::Start();
}

void CPrimitiveComponent::Update(float fTime)
{
	CSceneComponent::Update(fTime);
}

void CPrimitiveComponent::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CPrimitiveComponent::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CPrimitiveComponent::PreRender(float fTime)
{
	GET_SINGLE(CRenderManager)->AddComponent(this);

	CSceneComponent::PreRender(fTime);
}

void CPrimitiveComponent::Render(float fTime)
{
	CSceneComponent::Render(fTime);
}

void CPrimitiveComponent::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

void CPrimitiveComponent::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);
}

void CPrimitiveComponent::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);
}
