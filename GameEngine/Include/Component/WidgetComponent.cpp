#include "WidgetComponent.h"
#include "../Object/Obj.h"
#include "../Scene/Scene.h"

CWidgetComponent::CWidgetComponent()	:
	m_pUIObj(nullptr)
{
}

CWidgetComponent::CWidgetComponent(const CWidgetComponent& com)	:
	CSceneComponent(com)
{
	m_pUIObj = nullptr;

	if (com.m_pUIObj)
		m_pUIObj = com.m_pUIObj->Clone();

}

CWidgetComponent::~CWidgetComponent()
{
	SAFE_RELEASE(m_pUIObj);
}

bool CWidgetComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CWidgetComponent::Start()
{
	CSceneComponent::Start();
}

void CWidgetComponent::Update(float fTime)
{
	CSceneComponent::Update(fTime);

	if (m_pUIObj)
		m_pUIObj->Update(fTime);
}

void CWidgetComponent::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CWidgetComponent::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);

	if (m_pUIObj)
		m_pUIObj->Collision(fTime);
}

void CWidgetComponent::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);

	if (m_pUIObj)
		m_pUIObj->PreRender(fTime);
}

void CWidgetComponent::Render(float fTime)
{
	CSceneComponent::Render(fTime);

	if (m_pUIObj)
		m_pUIObj->Render(fTime);
}

void CWidgetComponent::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

CWidgetComponent* CWidgetComponent::Clone()
{
	return new CWidgetComponent(*this);
}

void CWidgetComponent::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);

	bool bUI = false;

	if (m_pUIObj)
		bUI = true;

	fwrite(&bUI, 1, 1, pFile);

	if (m_pUIObj)
	{
		int iType = m_pUIObj->GetClassType();

		fwrite(&iType, 4, 1, pFile);

		m_pUIObj->Save(pFile);
	}		
}

void CWidgetComponent::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);

	bool bUI = false;

	fread(&bUI, 1, 1, pFile);

	if (bUI)
	{
		int iType = 0;

		fread(&iType, 4, 1, pFile);

		m_pUIObj = m_pScene->EditorCreate(iType);

		m_pUIObj->Load(pFile);
	}
}
