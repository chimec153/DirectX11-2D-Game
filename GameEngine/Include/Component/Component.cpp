#include "Component.h"
#include "../Object/Obj.h"

CComponent::CComponent()	:
	m_pScene(nullptr),
	m_pLayer(nullptr),
	m_pObj(nullptr),
	m_eType(COMPONENT_TYPE::CT_SCENE),
	m_bStart(false)
{

}

CComponent::CComponent(const CComponent& com)	:
	CRef(com)
{
	m_pScene = com.m_pScene;
	m_pLayer = com.m_pLayer;
	m_pObj = nullptr;
	m_eType = com.m_eType;
	m_bStart = false;
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

COMPONENT_TYPE CComponent::GetType() const
{
	return m_eType;
}

CLayer* CComponent::GetLayer() const
{
	return m_pLayer;
}

bool CComponent::IsStart() const
{
	return m_bStart;
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

void CComponent::Save(FILE* pFile)
{
	CRef::Save(pFile);

	fwrite(&m_eType, 4, 1, pFile);
}

void CComponent::Load(FILE* pFile)
{
	CRef::Load(pFile);

	fread(&m_eType, 4, 1, pFile);

	m_bStart = false;
}
