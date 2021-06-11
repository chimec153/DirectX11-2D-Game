#include "UIBar.h"
#include "UI/UIImage.h"
#include "UI/Bar.h"

CUIBar::CUIBar()	:
	m_pBar(nullptr),
	m_pImg(nullptr)
{
}

CUIBar::CUIBar(const CUIBar& ui)	:
	CObj(ui)
{
	if(ui.m_pBar)
		m_pBar = ui.m_pBar->Clone();

	if (ui.m_pImg)
		m_pImg = ui.m_pImg->Clone();
}

CUIBar::~CUIBar()
{
	SAFE_RELEASE(m_pBar);
	SAFE_RELEASE(m_pImg);
}

bool CUIBar::Init()
{
	if (!CObj::Init())
		return false;

	m_pBar = CreateComponent<CBar>("Bar", m_pLayer);
	m_pBar->SetTexture("Bar");
	m_pBar->SetPercent(0.7f);
	m_pBar->SetPivot(0.5f, 0.5f, 0.f);
	m_pBar->SetWorldScale(190.f, 30.f, 0.f);

	SetRootComponent(m_pBar);

	m_pImg = CreateComponent<CUIImage>("Back", m_pLayer);
	m_pImg->SetTexture("Back");
	m_pImg->SetPivot(0.5f, 0.5f, 0.f);
	m_pImg->SetWorldScale(200.f, 40.f, 0.f);
	m_pImg->SetInheritScale(false);
	m_pImg->SetZOrder(1);

	m_pBar->AddChild(m_pImg);

	return true;
}

void CUIBar::Start()
{
	CObj::Start();
}

void CUIBar::Update(float fTime)
{
	CObj::Update(fTime);
}

void CUIBar::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);
}

void CUIBar::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CUIBar::PreRender(float fTime)
{
	CObj::PreRender(fTime);
}

void CUIBar::Render(float fTime)
{
	CObj::Render(fTime);
}

void CUIBar::PostRender(float fTime)
{
	CObj::PostRender(fTime);
}

CUIBar* CUIBar::Clone()
{
	return new CUIBar(*this);
}

void CUIBar::Save(FILE* pFile)
{
	CObj::Save(pFile);
}

void CUIBar::Load(FILE* pFile)
{
	CObj::Load(pFile);
}
