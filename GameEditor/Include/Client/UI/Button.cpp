#include "../UI/Button.h"
#include "UI/UIButton.h"
#include "Component/ColliderRect.h"
#include "Component/Sound.h"
#include "UI/Panel.h"
#include "UI/TitleBar.h"

CClientButton::CClientButton() :
	m_pBtn(nullptr),
	m_pPanel(nullptr)
{
}

CClientButton::CClientButton(const CClientButton& ui)	:
	CObj(ui)
{
	m_pBtn = (CUIButton*)FindSceneComponent("Button");
}

CClientButton::~CClientButton()
{
	SAFE_RELEASE(m_pBtn);
	SAFE_RELEASE(m_pPanel);
}

bool CClientButton::Init()
{
	if (!CObj::Init())
		return false;

	m_pPanel = CreateComponent<CPanel>("Panel", m_pLayer);

	m_pPanel->SetWorldScale(500.f, 400.f, 1.f);

	m_pPanel->SetPivot(0.5f, 0.5f, 0.f);

	m_pPanel->SetWorldPos(600.f, 300.f, 0.f);

	SetRootComponent(m_pPanel);

	m_pBtn = CreateComponent<CUIButton>("Button", m_pLayer);

	m_pBtn->SetColor(BUTTON_STATE::OFF, 0.3f, 0.3f, 0.3f, 1.f);
	m_pBtn->SetColor(BUTTON_STATE::ON, 1.f, 1.f, 1.f, 1.f);
	m_pBtn->SetColor(BUTTON_STATE::PUSH, 1.f, 0.f, 0.f, 1.f);
	m_pBtn->SetColor(BUTTON_STATE::DISABLE, 0.7f, 0.7f, 0.7f, 1.f);

	m_pBtn->SetTexture(BUTTON_STATE::OFF, "Start");
	m_pBtn->SetTexture(BUTTON_STATE::ON, "Start");
	m_pBtn->SetTexture(BUTTON_STATE::PUSH, "Start");
	m_pBtn->SetTexture(BUTTON_STATE::DISABLE, "Start");
	
	m_pBtn->SetWorldScale(100.f, 100, 1.f);

	m_pBtn->SetRelativePos(100.f, 100.f, 0.f);
	m_pBtn->SetPivot(0.5f, 0.5f, 0.f);
	m_pBtn->SetInheritScale(false);

	m_pBtn->SetSound(BUTTON_STATE::PUSH, "BtnPress");

	m_pPanel->AddChild(m_pBtn);

	CTitleBar* pBar = CreateComponent<CTitleBar>("TitleBar", m_pLayer);

	pBar->SetWorldScale(470.f, 30.f, 1.f);
	pBar->SetRelativePos(-30.f, 185.f, 0.f);

	pBar->SetPivot(0.5f, 0.5f, 0.f);
	pBar->SetInheritScale(false);
	pBar->SetExtent(500.f, 30.f);
	pBar->SetTitle(TEXT("Options"));

	pBar->SetTitleRect(-100.f, 10.f, 100.f, 0.f);

	pBar->SetTitleAlignH(TEXT_ALIGN_HORISONTAL::CENTER);
	pBar->SetTitleAlignV(TEXT_ALIGN_VERTICAL::MID);

	m_pPanel->AddChild(pBar);

	SAFE_RELEASE(pBar);

	CUIButton* pExit = CreateComponent<CUIButton>("ExitButton", m_pLayer);

	pExit->SetCallBack<CClientButton>(BUTTON_STATE::PUSH, this, &CClientButton::Close);

	pExit->SetWorldScale(30.f, 30.f, 1.f);
	pExit->SetInheritScale(false);
	pExit->SetRelativePos(235.f, 185.f, 0.f);

	pExit->SetTexture(BUTTON_STATE::OFF, "End");
	pExit->SetTexture(BUTTON_STATE::ON, "End");
	pExit->SetTexture(BUTTON_STATE::PUSH, "End");
	pExit->SetTexture(BUTTON_STATE::DISABLE, "End");

	pExit->SetColor(BUTTON_STATE::OFF, 0.3f, 0.3f, 0.3f, 1.f);
	pExit->SetColor(BUTTON_STATE::ON, 1.f, 1.f, 1.f, 1.f);
	pExit->SetColor(BUTTON_STATE::PUSH, 0.8f, 0.8f, 0.8f, 1.f);
	pExit->SetColor(BUTTON_STATE::DISABLE, 0.2f, 0.2f, 0.2f, 1.f);

	pExit->SetPivot(0.5f, 0.5f, 0.f);

	m_pPanel->AddChild(pExit);

	SAFE_RELEASE(pExit);

	return true;
}

void CClientButton::Start()
{
	CObj::Start();
}

void CClientButton::Update(float fTime)
{
	CObj::Update(fTime);
}

void CClientButton::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);
}

void CClientButton::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CClientButton::PreRender(float fTime)
{
	CObj::PreRender(fTime);
}

void CClientButton::Render(float fTime)
{
	CObj::Render(fTime);
}

void CClientButton::PostRender(float fTime)
{
	CObj::PostRender(fTime);
}

CClientButton* CClientButton::Clone()
{
	return new CClientButton(*this);
}

void CClientButton::Save(FILE* pFile)
{
	CObj::Save(pFile);
}

void CClientButton::Load(FILE* pFile)
{
	CObj::Load(pFile);
}

void CClientButton::Close(float fTime)
{
	Enable(false);
}
