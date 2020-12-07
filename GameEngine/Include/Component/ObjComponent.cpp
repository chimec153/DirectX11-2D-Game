#include "ObjComponent.h"

CObjComponent::CObjComponent()
{
	m_eType = COMPONENT_TYPE::CT_OBJECT;
}

CObjComponent::CObjComponent(const CObjComponent& com)
{
}

CObjComponent::~CObjComponent()
{
}

void CObjComponent::SetScene(CScene* pScene)
{
	m_pScene = pScene;
}

CObjComponent* CObjComponent::Clone()
{
	return new CObjComponent(*this);
}

void CObjComponent::Save(FILE* pFile)
{
	CComponent::Save(pFile);
}

void CObjComponent::Load(FILE* pFile)
{
	CComponent::Load(pFile);
}
