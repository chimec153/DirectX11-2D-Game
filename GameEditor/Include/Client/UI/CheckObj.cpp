#include "CheckObj.h"
#include "UI/CheckBox.h"
#include "UI/UIFont.h"

CCheckObj::CCheckObj()	:
	m_pBox(nullptr)
{
}

CCheckObj::CCheckObj(const CCheckObj& obj)	:
	CUIObject(obj)
{
	m_pBox = (CCheckBox*)FindSceneComponent("CheckBox");
}

CCheckObj::~CCheckObj()
{
	SAFE_RELEASE(m_pBox);
	SAFE_RELEASE(m_pText);
}

bool CCheckObj::Init()
{
	if (!CUIObject::Init())
		return false;

	m_pBox = CreateComponent<CCheckBox>("CheckBox", m_pLayer);
	m_pBox->SetTexture(BUTTON_STATE::OFF, "Box");
	m_pBox->SetTexture(BUTTON_STATE::ON, "Box");
	m_pBox->SetTexture(BUTTON_STATE::PUSH, "Box");
	m_pBox->SetTexture(BUTTON_STATE::DISABLE, "Box");
	m_pBox->SetColor(BUTTON_STATE::OFF, 1.f, 1.f, 1.f, 1.f);
	m_pBox->SetColor(BUTTON_STATE::ON, 1.f, 1.f, 1.f, 1.f);
	m_pBox->SetColor(BUTTON_STATE::PUSH, 1.f, 1.f, 1.f, 1.f);
	m_pBox->SetChkTex("Check");
	m_pBox->SetSound(BUTTON_STATE::PUSH, "BtnPress");
	m_pBox->SetPivot(0.5f, 0.5f, 0.f);
	m_pBox->SetWorldScale(50.f, 50.f, 0.f);
	m_pBox->SetWorldPos(300.f, 300.f, 0.f);

	SetRootComponent(m_pBox);

	m_pText = CreateComponent<CUIFont>("Text", m_pLayer);
	m_pText->SetShadowColor(0.5f, 0.5f, 0.5f, 1.f);
	m_pText->SetShadowOpacity(0.5f);
	m_pText->SetColor(25, 150, 185, (BYTE)255);
	m_pText->SetShadowOffset(Vector3(2.f, 2.f, 0.f));
	m_pText->SetAlignH(TEXT_ALIGN_HORISONTAL::CENTER);
	m_pText->SetAlignV(TEXT_ALIGN_VERTICAL::MID);

	m_pBox->AddChild(m_pText);

	return true;
}

void CCheckObj::Start()
{
	CUIObject::Start();
}

void CCheckObj::Update(float fTime)
{
	CUIObject::Update(fTime);
}

void CCheckObj::PostUpdate(float fTime)
{
	CUIObject::PostUpdate(fTime);
}

void CCheckObj::Collision(float fTime)
{
	CUIObject::Collision(fTime);
}

void CCheckObj::PreRender(float fTime)
{
	CUIObject::PreRender(fTime);
}

void CCheckObj::Render(float fTime)
{
	CUIObject::Render(fTime);
}

void CCheckObj::PostRender(float fTime)
{
	CUIObject::PostRender(fTime);
}

CCheckObj* CCheckObj::Clone()
{
	return new CCheckObj(*this);
}

void CCheckObj::Save(FILE* pFile)
{
	CUIObject::Save(pFile);
}

void CCheckObj::Load(FILE* pFile)
{
	CUIObject::Load(pFile);
}
