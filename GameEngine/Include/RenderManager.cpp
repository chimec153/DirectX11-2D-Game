#include "RenderManager.h"
#include "Component/SceneComponent.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()
{

}

CRenderManager::~CRenderManager()
{

}

bool CRenderManager::Init()
{
	return true;
}

bool CRenderManager::AddComponent(CSceneComponent* pComponent)
{
	switch (pComponent->GetSceneComponentType())
	{
	case SCENE_COMPONENT_TYPE::SCT_2D:
		m_vec2DComponent.push_back(pComponent);
		break;
	case SCENE_COMPONENT_TYPE::SCT_3D:
		m_vecComponent.push_back(pComponent);
		break;
	case SCENE_COMPONENT_TYPE::SCT_UI:
		m_vecUIComponent.push_back(pComponent);
		break;
	}

	return true;
}

void CRenderManager::ClearComponent()
{
	m_vec2DComponent.clear();
	m_vecComponent.clear();
	m_vecUIComponent.clear();
}

void CRenderManager::Render(float fTime)
{
	Render2D(fTime);
	Render3D(fTime);
	RenderUI(fTime);
}

void CRenderManager::Render2D(float fTime)
{
	size_t iSize = m_vec2DComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vec2DComponent[i]->Render(fTime);
}

void CRenderManager::Render3D(float fTime)
{
	size_t iSize = m_vecComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecComponent[i]->Render(fTime);
}

void CRenderManager::RenderUI(float fTime)
{
	size_t iSize = m_vecUIComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecUIComponent[i]->Render(fTime);
}
