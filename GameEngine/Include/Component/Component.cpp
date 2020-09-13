#include "Component.h"
#include "../Object/Obj.h"

CComponent::CComponent()	:
	m_pScene(nullptr),
	m_pObj(nullptr)
{

}

CComponent::CComponent(const CComponent& com)
{
}

CComponent::~CComponent()
{

}

CObj* CComponent::GetObj() const
{
	if (m_pObj)
		m_pObj->AddRef();

	return m_pObj;
}

bool CComponent::Init()
{
	return true;
}

void CComponent::Start()
{
}

void CComponent::Update(float fTime)
{
}

void CComponent::PostUpdate(float fTime)
{
}

void CComponent::Collision(float fTime)
{
}

void CComponent::PreRender(float fTime)
{
}

void CComponent::Render(float fTime)
{
}

void CComponent::PostRender(float fTime)
{
}
