#include "UIObject.h"
#include "UIControl.h"

CUIObject::CUIObject()	:
	m_pViewPort(nullptr),
	m_iZOrder(0)
{
}

CUIObject::CUIObject(const CUIObject& ui)
{
	*this = ui;

	m_iRef = 1;

	m_pViewPort = ui.m_pViewPort;

	m_iZOrder = ui.m_iZOrder;
}

CUIObject::~CUIObject()
{
}

int CUIObject::GetZOrder() const
{
	return m_iZOrder;
}


bool CUIObject::Init()
{
	if (!CObj::Init())
		return false;

	return true;
}

void CUIObject::Start()
{
	CObj::Start();
}

void CUIObject::Input(float fTime)
{
	CObj::Input(fTime);
}

void CUIObject::Update(float fTime)
{
	CObj::Update(fTime);
}

void CUIObject::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);
}

void CUIObject::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CUIObject::PreRender(float fTime)
{
	CObj::PreRender(fTime);
}

void CUIObject::Render(float fTime)
{
	CObj::Render(fTime);
}

void CUIObject::PostRender(float fTime)
{
	CObj::PostRender(fTime);
}

CUIObject* CUIObject::Clone()
{
	return new CUIObject(*this);
}

void CUIObject::Save(FILE* pFile)
{
	CRef::Save(pFile);
}

void CUIObject::Load(FILE* pFile)
{
	CRef::Load(pFile);
}
