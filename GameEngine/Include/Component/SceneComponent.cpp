#include "SceneComponent.h"
#include "Transform.h"

CSceneComponent::CSceneComponent()
{
	m_eType = COMPONENT_TYPE::CT_SCENE;

	m_pTransform = new CTransform;

	m_pTransform->m_pOwner = this;
}

CSceneComponent::CSceneComponent(const CSceneComponent& com)	:
	CComponent(com)
{
	m_pTransform = com.m_pTransform->Clone();
}

CSceneComponent::~CSceneComponent()
{
	SAFE_DELETE(m_pTransform);
}


bool CSceneComponent::Init()
{
	m_pTransform->m_pScene = m_pScene;

	return true;
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
