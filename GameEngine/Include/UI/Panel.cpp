#include "Panel.h"

CPanel::CPanel()
{
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_UI;
	m_eSceneComponentClassType = SCENECOMPONENT_CLASS_TYPE::UI_PANEL;
}

CPanel::CPanel(const CPanel& ui)	:
	CUIControl(ui)
{
}

CPanel::~CPanel()
{
}

bool CPanel::Init()
{
	if (!CUIControl::Init())
		return false;

	return true;
}

void CPanel::Start()
{
	CUIControl::Start();
}

void CPanel::Update(float fTime)
{
	CUIControl::Update(fTime);
}

void CPanel::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CPanel::Collision(float fTime)
{
	CUIControl::Collision(fTime);
}

void CPanel::PreRender(float fTime)
{
	CUIControl::PreRender(fTime);
}

void CPanel::Render(float fTime)
{
	CUIControl::Render(fTime);
}

void CPanel::PostRender(float fTime)
{
	CUIControl::PostRender(fTime);
}

CPanel* CPanel::Clone()
{
	return new CPanel(*this);
}

void CPanel::Save(FILE* pFile)
{
	CUIControl::Save(pFile);
}

void CPanel::Load(FILE* pFile)
{
	CUIControl::Load(pFile);
}
